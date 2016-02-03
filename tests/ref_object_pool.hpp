#ifndef TESTS_REF_OBJECT_POOL_HPP
# define TESTS_REF_OBJECT_POOL_HPP

# include <toolsbox/pool/detail/object_pool_impl.hpp>
# include <toolsbox/pool/detail/deleter.hpp>
# include <toolsbox/pool/detail/segment.hpp>

# include <cstdint>
# include <list>
# include <array>
# include <memory>
# include <atomic>
# include <thread>
# include <mutex>

namespace ref
{
  template <class T> class basic_mutex_stack
  {
    public:
      typedef T                                                                 type;
      typedef basic_mutex_stack<type>                                           self_type;
      typedef typename std::aligned_storage<sizeof(type), alignof(type)>::type  data_type;
      typedef char                                                              id_type;
      typedef toolsbox::pool::detail::deleter<type, self_type, id_type>         deleter_type;
      typedef std::unique_ptr<type, deleter_type>                               ptr_type;

      basic_mutex_stack()
      {
      }

      ~basic_mutex_stack()
      {
        std::lock_guard<std::mutex> guard(mutex_);
        data_.clear();
      }

      ptr_type acquire(void*& data)
      {
        data = acquire_();
        ptr_type ptr;
        ptr.get_deleter().init(*this, 0);
        return ptr;
      }

      void release(id_type, void* data)
      {
        std::lock_guard<std::mutex> guard(mutex_);
        data_.push_back(std::unique_ptr<data_type>(reinterpret_cast<data_type*>(data)));
      }

      std::size_t capacity() const
      {
        return size();
      }

      std::size_t size() const
      {
        std::lock_guard<std::mutex> guard(mutex_);
        return data_.size();
      }

      bool full() const
      {
        std::lock_guard<std::mutex> guard(mutex_);
        return data_.empty();
      }

    protected:
      void* acquire_()
      {
        std::lock_guard<std::mutex> guard(mutex_);
        if(data_.empty())
        {
          return new data_type;
        }
        else
        {
          std::unique_ptr<data_type> d = std::move(data_.front());
          data_.pop_front();
          return d.release();
        }
      }

    private:
      mutable std::mutex                     mutex_;
      std::list<std::unique_ptr<data_type>>  data_;
  };

  template <class T, std::size_t N = 64> class old_tree_node
  {
    public:
      typedef T                                                         type;
      static const std::size_t                                          nb_child_by_node = N;
      typedef old_tree_node<type, nb_child_by_node>                     self_type;
      typedef toolsbox::pool::detail::segment<type>                     segment_type;
      typedef typename segment_type::id_type                            id_type;
      typedef toolsbox::pool::detail::deleter<type, self_type, id_type> deleter_type;
      typedef std::unique_ptr<type, deleter_type>                       ptr_type;
      typedef std::atomic<self_type*>                                   atomic_node_type;
      typedef std::array<atomic_node_type, nb_child_by_node>            array_atomic_node_type;

      old_tree_node()
        : childs_index_(0)
      {
        for(atomic_node_type& node : childs_)
        {
          node = nullptr;
        }
      }

      ~old_tree_node()
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
      }

      std::size_t capacity() const
      {
        std::size_t cap = segment_.capacity();
        for(const atomic_node_type& node : childs_)
        {
          self_type* c = node.load();
          if(c)
          {
            cap += c->capacity();
          }
        }
        return cap;
      }

      std::size_t size() const
      {
        std::size_t size = segment_.capacity();
        for(const atomic_node_type& node : childs_)
        {
          self_type* c = node.load();
          if(c)
          {
            size += c->size();
          }
        }
        return size;
      }

      bool full() const
      {
        bool is_full = segment_.full();
        for(const atomic_node_type& node : childs_)
        {
          self_type* c = node.load();
          if(c)
          {
            is_full = is_full && c->full();
          }
        }
        return is_full;
      }

    protected:
      void acquire_(self_type*& node, void*& data, id_type& id)
      {
        if(segment_.acquire(data, id))
        {
          node = this;
        }
        else
        {
          get_next_node_().acquire_(node, data, id);
        }
      }

      self_type& get_next_node_()
      {
        atomic_node_type &atomic_child = get_next_atomic_child_();
        self_type*               child = atomic_child.load();
        std::unique_ptr<self_type> new_child;
        if(child == nullptr)
        {
          new_child.reset(new self_type());
          do
          {
            if(atomic_child.compare_exchange_weak(child, new_child.get()))
            {
              child = new_child.release();
            }
          } while(child == nullptr);
        }

        return *child;
      }

      atomic_node_type& get_next_atomic_child_()
      {
        std::size_t       index = childs_index_++;
        std::size_t       id    = ( index ) & (nb_child_by_node - 1);
        atomic_node_type& child = childs_[id];
        return child;
      }
    private:
      segment_type            segment_;
      array_atomic_node_type  childs_;
      std::size_t             childs_index_;
  };

  template<class T> using mutex_object_pool    = toolsbox::pool::detail::basic_object_pool_impl<T, basic_mutex_stack<T>>;
  template<class T> using old_tree_object_pool = toolsbox::pool::detail::basic_object_pool_impl<T, old_tree_node<T>>;
}

#endif

