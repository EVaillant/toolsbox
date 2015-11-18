#ifndef TOOLSBOX_VARIANT_DETAIL_VARIANT_VISITOR_HPP
# define TOOLSBOX_VARIANT_DETAIL_VARIANT_VISITOR_HPP

# include <utility>
# include <cstdlib>

namespace toolsbox
{
  namespace variant_detail
  {
    //
    // variant_visitor
    template <class Ret, class ... Types> struct variant_visitor
    {
    };
    template <class Ret, class Type, class ... Types> struct variant_visitor<Ret, Type, Types...>
    {
      template<class Visitor, class Variant> static Ret apply(Visitor &&visitor, Variant &&variant, size_t current, size_t wanted)
      {
        if(current == wanted)
        {
          return visitor(variant.template as<Type>());
        }
        else
        {
          return variant_visitor<Ret, Types...>::apply(std::forward<Visitor>(visitor), std::forward<Variant>(variant), current + 1, wanted);
        }
      }
    };
    template <class Ret> struct variant_visitor<Ret>
    {
      template<class Visitor, class Variant> static Ret apply(Visitor &&, Variant &&, size_t, size_t)
      {
        typedef typename std::remove_const<typename std::remove_reference<Ret>::type>::type locate_type;
        static locate_type r = locate_type();
        return r;
      }
    };
    template <> struct variant_visitor<void>
    {
      template<class Visitor, class Variant> static void apply(Visitor &&, Variant &&, size_t, size_t)
      {
      }
    };
  }
}

#endif

