#ifndef VARIANT_DETAIL_SELECT_TYPE_HPP
# define VARIANT_DETAIL_SELECT_TYPE_HPP

# include <utility>
# include <cstdlib>

namespace toolsbox
{
  namespace variant_detail
  {
    //
    // select_type
    template <class FindType, class Type, class ... Types> struct select_type
    {
      static const size_t id = (select_type<FindType, Types...>::id == 0 ? (select_type<FindType, Type>::id) : (select_type<FindType, Types...>::id + 1));
    };

    template <class FindType> struct select_type<FindType, FindType>
    {
      static const size_t id = 1;
    };

    template <class FindType, class Type> struct select_type<FindType, Type>
    {
      static const size_t id = 0;
    };
  }
}

#endif

