#ifndef TOOLSBOX_POOL_DETAIL_TREE_NODE_HPP
# define TOOLSBOX_POOL_DETAIL_TREE_NODE_HPP

# include <toolsbox/pool/detail/segment.hpp>
# include <toolsbox/pool/detail/deleter.hpp>

# include <cstdint>
# include <array>
# include <memory>
# include <atomic>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T, std::size_t N = 64> class tree_node
      {
        public:
          typedef T                                                type;
          static const std::size_t                                 nb_child_by_node = N;
          typedef tree_node<type, nb_child_by_node>                self_type;
          typedef segment<type>                                    segment_type;
          typedef typename segment_type::id_type                   id_type;
          typedef deleter<type, self_type, id_type>                deleter_type;
          typedef std::unique_ptr<type, deleter_type>              ptr_type;
          typedef std::atomic<self_type*>                          atomic_node_type;
          typedef std::array<atomic_node_type, nb_child_by_node>   array_atomic_node_type;
          typedef std::array<std::atomic_size_t, nb_child_by_node> size_child_type;
          typedef std::array<std::atomic_size_t, nb_child_by_node> capacity_child_type;

          tree_node(self_type* owner = nullptr, std::size_t id = 0)
            : childs_index_(0)
            , owner_(owner)
            , id_child_(id)
          {
            for(std::size_t i = 0; i < nb_child_by_node ; ++i)
            {
              childs_[i] = nullptr;
              size_[i]         = 0;
              capacity_[i]     = 0;
            }
          }

          ~tree_node()
          {
            for(atomic_node_type& node : childs_)
            {
              delete node.load();
              node = nullptr;
            }
          }

          ptr_type acquire(void*& data)
          {
            self_type* node = nullptr;
            id_type      id;

            acquire_(node, data, id);
            ptr_type ptr;
            ptr.get_deleter().init(*node, id);
            return ptr;
          }

          void release(id_type id, void*)
          {
            segment_.release(id);
            inc_size_(1);
          }

          std::size_t capacity() const
          {
            std::size_t ret = segment_.capacity();
            for(const std::atomic_size_t& c : capacity_)
            {
              ret += c.load();
            }
            return ret;
          }

          std::size_t size() const
          {
            std::size_t ret = segment_.size();
            for(const std::atomic_size_t& s : size_)
            {
              ret += s.load();
            }
            return ret;
          }

          bool full() const
          {
            bool ret = segment_.full();
            for(const std::atomic_size_t& s : size_)
            {
              ret = ret && (s.load() == 0);
            }
            return ret;
          }

        protected:
          void init_child_()
          {
            inc_size_(segment_.capacity());
            inc_cap_ (segment_.capacity());
          }

          void inc_size_(std::size_t val)
          {
            if(owner_) owner_->inc_size_(id_child_, val);
          }

          void inc_size_(std::size_t id, std::size_t val)
          {
            size_[id] += val;
            inc_size_(val);
          }

          void inc_cap_(std::size_t val)
          {
            if(owner_) owner_->inc_cap_(id_child_, val);
          }

          void inc_cap_(std::size_t id, std::size_t val)
          {
            capacity_[id] += val;
            inc_cap_(val);
          }

          void acquire_(self_type*& node, void*& data, id_type& id)
          {
            if(segment_.acquire(data, id))
            {
              node = this;
            }
            else
            {
              get_or_create_node_().acquire_(node, data, id);
            }
          }

          std::size_t find_child_id_()
          {
            std::size_t id;
            while(!get_free_slot_(id))
            {
              create_node_();
            }
            return id;
          }

          bool get_free_slot_(std::size_t& slot)
          {
            std::size_t index      = childs_index_;
            for(std::size_t i = 0 ; i < nb_child_by_node ; ++i)
            {
              std::size_t id = (index + i) & (nb_child_by_node -1);
              std::atomic_size_t& atomic_old_size = size_[id];
              std::size_t                old_size = atomic_old_size.load();
              while(old_size > 0)
              {
                std::size_t new_size = old_size -1;
                if(atomic_old_size.compare_exchange_weak(old_size, new_size))
                {
                  childs_index_ = id;
                  slot = id;
                  return true;
                }
              }
            }
            return false;
          }

          void create_node_()
          {
            if(owner_ != nullptr && !segment_.full())
            {
              return;
            }

            std::size_t min_cap    = -1;
            std::size_t id_min_cap = -1;
            for(std::size_t i = 0 ; i < nb_child_by_node ; ++i)
            {
              std::size_t tmp_cap = capacity_[i];
              if(tmp_cap < min_cap)
              {
                min_cap    = tmp_cap;
                id_min_cap = i;
              }
              if(size_[i] > 0)
              {
                return;
              }
            }

            self_type& self = get_or_create_node_(id_min_cap);
            self.create_node_();
          }

          self_type& get_or_create_node_()
          {
            return get_or_create_node_(find_child_id_());
          }

          self_type& get_or_create_node_(std::size_t id)
          {
            atomic_node_type &atomic_child = childs_[id];
            self_type*               child = atomic_child.load();
            std::unique_ptr<self_type> new_child;
            if(child == nullptr)
            {
              new_child.reset(new self_type(this, id));
              do
              {
                if(atomic_child.compare_exchange_weak(child, new_child.get()))
                {
                  child = new_child.release();
                  child->init_child_();
                }
              } while(child == nullptr);
            }

            return *child;
          }

        private:
          segment_type            segment_;
          array_atomic_node_type  childs_;
          std::size_t             childs_index_;
          self_type*              owner_;
          std::size_t             id_child_;
          size_child_type         size_;
          capacity_child_type     capacity_;
      };
    }
  }
}

#endif

