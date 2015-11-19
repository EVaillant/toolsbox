#ifndef TOOLSBOX_LOCKFREE_FIX_SIZE_QUEUE_HPP
# define TOOLSBOX_LOCKFREE_FIX_SIZE_QUEUE_HPP

# include <toolsbox/lockfree/detail/pointer_tag.hpp>
# include <toolsbox/lockfree/detail/helper.hpp>

# include <atomic>
# include <array>

namespace toolsbox
{
  namespace lockfree
  {
    template <class T, size_t S> class fix_size_queue
    {
      public:
        typedef T value_type;
        static const size_t max_size = S;

        static_assert(S%8 == 0, "fix_size_queue must be multiple of 8");

        fix_size_queue()
          : head_read_(0)
          , head_write_(0)
          , tail_read_(0)
          , tail_write_(0)
        {
          for(std::atomic_bool& ato : check_read_)
          {
            ato = false;
          }

          for(std::atomic_bool& ato : check_write_)
          {
            ato = false;
          }
        }

        ~fix_size_queue()
        {
        }

        fix_size_queue(const fix_size_queue&) = delete;
        fix_size_queue(fix_size_queue&&)      = delete;

        bool empty() const
        {
          return is_empty_(head_read_, tail_read_);
        }

        bool full() const
        {
          return is_full_(head_read_, tail_read_);
        }

        size_t size() const
        {
          return size_(head_read_, tail_read_);
        }

        template <class ValueType> bool push(ValueType&& value)
        {
          size_t idx = tail_write_;
          while(!is_full_(head_write_, idx))
          {
            if(tail_write_.compare_exchange_weak(idx, idx + 1))
            {
              size_t array_idx = array_idx_(idx);
              data_[array_idx] = std::move(value);
              check_write_[array_idx] = true;
              while(idx == tail_read_ && data_is_ready_(check_write_[array_idx_(idx)]))
              {
                ++tail_read_;
                ++idx;
              }
              return true;
            }
            else detail::little_sleep();
          }
          return false;
        }

        bool pop(value_type& value)
        {
          size_t idx = head_read_;
          while(!is_empty_(idx, tail_read_))
          {
            if(head_read_.compare_exchange_weak(idx, idx + 1))
            {
              size_t array_idx = array_idx_(idx);
              value = std::move(data_[array_idx]);
              check_read_[array_idx] = true;
              while(idx == head_write_ && data_is_ready_(check_read_[array_idx_(idx)]))
              {
                ++head_write_;
                ++idx;
              }
              return true;
            }
            else detail::little_sleep();
          }
          return false;
        }

      protected:
        static size_t size_(size_t h, size_t t)
        {
          return (t >= h ? t - h : t + max_size - array_idx_(h));
        }

        static bool is_full_(size_t h, size_t t)
        {
          return (size_(h, t) == max_size);
        }

        static bool is_empty_(size_t h, size_t t)
        {
          return h == t;
        }

        static size_t array_idx_(size_t idx)
        {
          return idx & (max_size-1);
        }

        static bool data_is_ready_(std::atomic_bool& check)
        {
          bool available = check;
          bool first = true;
          bool ret   = true;

          do
          {
            if(!available)
            {
              ret = false;
              break;
            }
            if(!first) detail::little_sleep();
            first = false;
          } while (!check.compare_exchange_weak(available, false));
          return ret;
        }

      private:
        typedef std::array<T, S> data_type;
        typedef std::array<std::atomic_bool, S> data_available_type;

        data_type data_;

        std::atomic_size_t head_read_;
        std::atomic_size_t head_write_;

        std::atomic_size_t tail_read_;
        std::atomic_size_t tail_write_;

        data_available_type check_read_;
        data_available_type check_write_;
    };
  }
}

#endif

