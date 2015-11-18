#ifndef TOOLSBOX_VARIANT_DETAIL_VARIANT_IO_HPP
# define TOOLSBOX_VARIANT_DETAIL_VARIANT_IO_HPP

# include <toolsbox/variant/variant.hpp>
# include <toolsbox/variant/visitor_helper.hpp>

template <class ... Types> std::ostream& operator << (std::ostream& stream, const toolsbox::variant_detail::variant<Types...> &v)
{
  v.apply_visitor(toolsbox::variant_detail::visitor_io(stream));
  return stream;
}

#endif

