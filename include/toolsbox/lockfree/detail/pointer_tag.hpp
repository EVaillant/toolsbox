#ifndef TOOLSBOX_LOCKFREE_DETAIL_POINTER_TAG_HPP
# define TOOLSBOX_LOCKFREE_DETAIL_POINTER_TAG_HPP

# include <type_traits>
# include <cstdint>

namespace toolsbox
{
  namespace lockfree
  {
    namespace detail
    {
      template <class D, class C = void> struct pointer_tag
      {
      };

      template <class D> struct pointer_tag<D, typename std::enable_if<sizeof(D*) == sizeof(uint32_t)>::type>
      {
        typedef uint32_t counter_type;
        typedef D        data_type;

        pointer_tag(data_type* d = nullptr, counter_type c = 0) noexcept
          : data(d)
          , counter(c)
        {
        }

        operator bool () noexcept
        {
          return data;
        }

        inline data_type* operator->() const throw()
        {
          return data;
        }

        data_type*   data;
        counter_type counter;
      } __attribute__((aligned(8)));

      template <class D> struct pointer_tag<D, typename std::enable_if<sizeof(D*) == sizeof(uint64_t)>::type>
      {
        typedef uint64_t counter_type;
        typedef D        data_type;

        pointer_tag(data_type* d = nullptr, counter_type c = 0) noexcept
          : data(d)
          , counter(c)
        {
        }

        operator bool () noexcept
        {
          return data;
        }

        inline data_type* operator->() const throw()
        {
          return data;
        }

        data_type*   data;
        counter_type counter;
      } __attribute__((aligned(16)));
    }
  }
}

#endif

