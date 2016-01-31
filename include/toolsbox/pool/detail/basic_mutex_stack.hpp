#ifndef TOOLSBOX_POOL_DETAIL_BASIC_MUTEX_STACK_HPP
# define TOOLSBOX_POOL_DETAIL_BASIC_MUTEX_STACK_HPP

# include <toolsbox/pool/detail/deleter.hpp>

# include <cstdint>
# include <list>
# include <memory>
# include <atomic>
# include <thread>
# include <mutex>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T> class basic_mutex_stack
      {
        public:
          typedef T                                                                 type;
          typedef basic_mutex_stack<type>                                           self_type;
          typedef typename std::aligned_storage<sizeof(type), alignof(type)>::type  data_type;
          typedef char                                                              id_type;
          typedef deleter<type, self_type, id_type>                                 deleter_type;
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
    }
  }
}

#endif

