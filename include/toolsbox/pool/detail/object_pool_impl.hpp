#ifndef TOOLSBOX_POOL_DETAIL_OBJECT_POOL_IMPL_HPP
# define TOOLSBOX_POOL_DETAIL_OBJECT_POOL_IMPL_HPP

# include <cstdint>
# include <utility>
# include <memory>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T, class U> class basic_object_pool_impl
      {
        public:
          typedef T                                             type;
          typedef U                                             underlying_type;
          typedef basic_object_pool_impl<type, underlying_type> self_type;
          typedef typename underlying_type::ptr_type            ptr_type;

          template <class ... ARGS> ptr_type acquire(ARGS&& ... args)
          {
            void* data = nullptr;

            ptr_type ptr = underlying_.acquire(data);
            ptr.reset(new(data) type(std::forward<ARGS>(args)...));

            return ptr;
          }

          std::size_t capacity() const
          {
            return underlying_.capacity();
          }

          std::size_t size() const
          {
            return underlying_.size();
          }

          bool full() const
          {
            return underlying_.full();
          }

        private:
          underlying_type underlying_;
      };

      template <class T> class none_object_pool_impl
      {
        public:
          typedef T                           type;
          typedef none_object_pool_impl<type> self_type;
          typedef std::unique_ptr<type>       ptr_type;

          template <class ... ARGS> ptr_type acquire(ARGS&& ... args)
          {
            ptr_type ptr(new type(std::forward<ARGS>(args)...));
            return ptr;
          }

          std::size_t capacity() const
          {
            return 0;
          }

          std::size_t size() const
          {
            return 0;
          }

          bool full() const
          {
            return false;
          }
      };
    }
  }
}

#endif

