#ifndef TOOLSBOX_VARIANT_DETAIL_VARIANT_HASH_HPP
# define TOOLSBOX_VARIANT_DETAIL_VARIANT_HASH_HPP

# include <toolsbox/variant/variant.hpp>
# include <toolsbox/variant/visitor_helper.hpp>

# include <functional>

namespace std
{
  template<class ... Types> struct hash<toolsbox::variant_detail::variant<Types...>>
  {
    typedef toolsbox::variant_detail::variant<Types...> argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const
    {
      return s.apply_visitor(toolsbox::variant_detail::visitor_hash());
    }
  };
}

#endif

