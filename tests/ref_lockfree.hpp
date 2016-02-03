#ifndef TESTS_REF_LOCKFREE_HPP
# define TESTS_REF_LOCKFREE_HPP

# include <thread>
# include <mutex>
# include <list>
# include <array>

namespace ref
{
  template <class T> class stack
  {
    public:
      typedef T value_type;

      bool empty() const
      {
        std::lock_guard<std::mutex> guard(mutex_);
        bool v = values_.empty();
        return v;
      }

      template <class ValueType> bool push(ValueType&& value)
      {
        std::lock_guard<std::mutex> guard(mutex_);
        values_.push_front(std::move(value));
        return true;
      }

      bool pop(value_type& value)
      {
        std::lock_guard<std::mutex> guard(mutex_);
        if(values_.empty())
        {
          return false;
        }
        else
        {
          value = values_.front();
          values_.pop_front();
          return true;
        }
      }

    private:
      std::list<value_type> values_;
      mutable std::mutex		mutex_;
  };

  template <class T> class queue
  {
    public:
      typedef T value_type;

      bool empty() const
      {
        std::lock_guard<std::mutex> guard(mutex_);
        bool v = values_.empty();
        return v;
      }

      template <class ValueType> bool push(ValueType&& value)
      {
        std::lock_guard<std::mutex> guard(mutex_);
        values_.push_front(std::move(value));
        return true;
      }

      bool pop(value_type& value)
      {
        std::lock_guard<std::mutex> guard(mutex_);
        if(values_.empty())
        {
          return false;
        }
        else
        {
          value = std::move(values_.back());
          values_.pop_back();
          return true;
        }
      }

    private:
      std::list<value_type> values_;
      mutable std::mutex		mutex_;
  };

  template <class T, size_t S> class fix_size_queue
  {
    public:
      typedef T value_type;
      static const size_t max_size = S;

      static_assert(S%8 == 0, "fix_size_queue must be multiple of 8");

      fix_size_queue()
        : head_(0)
        , tail_(0)
      {
      }

      bool empty() const
      {
        return is_empty_(head_, tail_);
      }

      bool full() const
      {
        return is_full_(head_, tail_);
      }

      size_t size() const
      {
        return size_(head_, tail_);
      }

      template <class ValueType> bool push(ValueType&& value)
      {
        std::lock_guard<std::mutex> guard(tail_mutex_);
        if(!full())
        {
          size_t idx = (tail_++) & (max_size-1);
          data_[idx] = std::move(value);
          return true;
        }
        return false;
      }

      bool pop(value_type& value)
      {
        std::lock_guard<std::mutex> guard(head_mutex_);
        if(!empty())
        {
          size_t idx = (head_++) & (max_size-1);
          value = std::move(data_[idx]);
          return true;
        }
        return false;
      }

    protected:
      static size_t size_(size_t h, size_t t)
      {
        return (t >= h ? t - h : t + max_size - (h & (max_size-1)));
      }

      static bool is_full_(size_t h, size_t t)
      {
        return (size_(h, t) == max_size);
      }

      static bool is_empty_(size_t h, size_t t)
      {
        return h == t;
      }

    private:
      std::array<T, S> data_;

      size_t head_;
      size_t tail_;

      mutable std::mutex		head_mutex_;
      mutable std::mutex		tail_mutex_;
  };
}

#endif

