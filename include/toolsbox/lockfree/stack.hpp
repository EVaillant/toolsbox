#ifndef TOOLSBOX_LOCKFREE_STACK_HPP
# define TOOLSBOX_LOCKFREE_STACK_HPP

# include <toolsbox/lockfree/detail/pointer_tag.hpp>

# include <atomic>

namespace toolsbox
{
  namespace lockfree
  {
    template <class T> class stack
    {
      public:
        typedef T value_type;

        stack()
          : counter_(0)
        {
        }

        ~stack()
        {
          node_type head = head_.load();
          while(head)
          {
            delete head.data;
            head = head->next;
          }
        }

        stack(const stack&) = delete;
        stack(stack&&)      = delete;

        bool empty() const
        {
          return !head_.load();
        }

        template <class ValueType> bool push(ValueType&& value)
        {
          node_type old_head = head_.load();
          node_type new_head(new node);
          new_head->value  = std::move(value);
          new_head.counter = counter_++;
          do
          {
            new_head->next   = old_head;
          } while(!head_.compare_exchange_weak(old_head, new_head));
          return true;
        }

        bool pop(value_type& value)
        {
          node_type old_head = head_.load();
          node_type new_head;
          do
          {
            if(!old_head)
            {
              break;
            }
            new_head = old_head->next;
          } while(!head_.compare_exchange_weak(old_head, new_head));

          if(old_head)
          {
            value = std::move(old_head->value);
            delete old_head.data;
          }

          return old_head;
        }

      protected:
        struct node;
        typedef detail::pointer_tag<node>	node_type;
        struct node
        {
          node_type  next;
          value_type value;
        };

      private:
        std::atomic<node_type> head_;
        std::atomic<typename node_type::counter_type> counter_;
    };
  }
}

#endif

