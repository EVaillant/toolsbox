#ifndef TOOLSBOX_POOL_DETAIL_LINEAR_NODE_HPP
# define TOOLSBOX_POOL_DETAIL_LINEAR_NODE_HPP

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
      template <class T> class linear_node
      {
        public:
          typedef T                                                type;
          typedef linear_node<type>                                self_type;
          typedef segment<type>                                    segment_type;
          typedef typename segment_type::id_type                   id_type;
          typedef deleter<type, self_type, id_type>                deleter_type;
          typedef std::unique_ptr<type, deleter_type>              ptr_type;
          typedef std::atomic<self_type*>                          atomic_node_type;

          linear_node()
          {
            next_ = nullptr;
          }

          ~linear_node()
          {
            delete next_.load();
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
            std::size_t ret = segment_.capacity();
            self_type* next = next_.load();
            if(next)
            {
              ret += next->capacity();
            }
            return ret;
          }

          std::size_t size() const
          {
            std::size_t ret = segment_.size();
            self_type* next = next_.load();
            if(next)
            {
              ret += next->size();
            }
            return ret;
          }

          bool full() const
          {
            bool ret = segment_.full();
            self_type* next = next_.load();
            return ret && (next ? next->full() : true);
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
              get_or_create_node_().acquire_(node, data, id);
            }
          }

          self_type& get_or_create_node_()
          {
            self_type*               next = next_.load();
            std::unique_ptr<self_type> new_child;
            if(next == nullptr)
            {
              new_child.reset(new self_type());
              do
              {
                if(next_.compare_exchange_weak(next, new_child.get()))
                {
                  next = new_child.release();
                }
              } while(next == nullptr);
            }

            return *next;
          }

        private:
          segment_type     segment_;
          atomic_node_type next_;
      };
    }
  }
}

#endif

