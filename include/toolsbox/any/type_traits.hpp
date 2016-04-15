#ifndef TOOLSBOX_ANY_TYPE_TRAITS_HPP
# define TOOLSBOX_ANY_TYPE_TRAITS_HPP

# include <functional>

namespace toolsbox
{
  namespace any_detail
  {
    template <class T> struct any_type_id
    {
      typedef T type;
    };

    template <class T> struct any_type_id<T&>
    {
      typedef std::reference_wrapper<T> type;
    };
  }
}

#endif
