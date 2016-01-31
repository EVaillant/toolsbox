#ifndef TOOLSBOX_POOL_DETAIL_HELPER_HPP
# define TOOLSBOX_POOL_DETAIL_HELPER_HPP

# include <cstdint>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T> std::size_t find_first_bit(T num)
      {
        std::size_t ret = 0;
        static char nibblebits[] = {0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1};
        for(;num != 0;ret += 4)
        {
          unsigned char t = num & 0x0f;
          if(t)
          {
            ret += nibblebits[t];
            break;
          }
          num >>= 4;
        }
        return ret;
      }

      template <class T> T get_bit(T mask)
      {
        static T hitbits[] = {0, 1, 2, 1, 4, 1, 2, 1, 8, 1, 2, 1, 4, 1, 2, 1};
        for(char i = 0 ;mask; ++i)
        {
          unsigned char t = mask & 0x0f;
          if(t)
          {
            return hitbits[t] << 4*i;
          }
          mask >>= 4;
        }
        return 0;
      }

      template <class T> std::size_t count_bit(T mask)
      {
        static char nibblebits[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        std::size_t ret = 0;
        while(mask != 0)
        {
          unsigned char t = mask & 0x0f;
          ret += nibblebits[t];
          mask >>= 4;
        }
        return ret;
      }
    }
  }
}

#endif

