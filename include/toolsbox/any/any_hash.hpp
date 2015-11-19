#ifndef TOOLSBOX_ANY_ANY_HASH_HPP
# define TOOLSBOX_ANY_ANY_HASH_HPP

# include <toolsbox/any/any.hpp>

namespace std
{
  template<> struct hash<toolsbox::any_detail::any>
  {
    typedef toolsbox::any_detail::any argument_type;
    typedef std::size_t               result_type;

    result_type operator()(argument_type const& s) const
    {
      return (s.value_) ? s.value_->hash() : 0;
    }
  };
}

#endif // ANY_HASH_HPP

