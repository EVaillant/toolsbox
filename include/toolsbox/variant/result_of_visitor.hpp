#ifndef VARIANT_DETAIL_RESULT_OF_VISITOR_HPP
# define VARIANT_DETAIL_RESULT_OF_VISITOR_HPP

# include <utility>

namespace toolsbox
{
  namespace variant_detail
  {
    //
    // check_result_of_visitor
    template <class ... Types> struct check_result_of_visitor
    {
    };

    template <class Type, class ... Types> struct check_result_of_visitor<Type, Type, Types...>
    {
      typedef typename check_result_of_visitor<Type, Types...>::type type;
    };

    template <class Type> struct check_result_of_visitor<Type>
    {
      typedef Type type;
    };

    //
    // result_of_visitor
    template <class F, class ... Types> struct result_of_visitor
    {
      typedef typename toolsbox::variant_detail::check_result_of_visitor<decltype(std::declval<F>()(std::declval<Types>()))...>::type type;
    };
  }
}

#endif

