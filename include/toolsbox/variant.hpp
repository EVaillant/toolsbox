#ifndef TOOLSBOX_VARIANT_HPP
# define TOOLSBOX_VARIANT_HPP

# include <toolsbox/variant/variant.hpp>
# include <toolsbox/variant/variant_io.hpp>
# include <toolsbox/variant/variant_hash.hpp>

namespace toolsbox
{
  template <class ... Types> using variant = ::toolsbox::variant_detail::variant<Types...>;
}

#endif
