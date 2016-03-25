#ifndef TOOLSBOX_ANY_ANY_IO_HPP
# define TOOLSBOX_ANY_ANY_IO_HPP

# include <toolsbox/any/any.hpp>
# include <ostream>

namespace toolsbox
{
  namespace any_detail
  {
    inline std::ostream& operator << (std::ostream& stream , const toolsbox::any_detail::any &a)
    {
      if(a.value_)
      {
        a.value_->to_string(stream);
      }
      return stream;
    }
  }
}

#endif

