#ifndef TOOLSBOX_MPL_STATIC_SIZE_HPP
# define TOOLSBOX_MPL_STATIC_SIZE_HPP

# include <cstdlib>

namespace toolsbox
{
  namespace mpl
  {
    template <size_t Value, size_t ... Values> struct static_max
    {
      static const size_t max = (Value > static_max<Values...>::max ? Value : static_max<Values...>::max);
    };

    template <size_t Value> struct static_max<Value>
    {
      static const size_t max = Value;
    };
  }
}

#endif
