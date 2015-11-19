#ifndef TOOLSBOX_LOCKFREE_DETAIL_HELPER_HPP
# define TOOLSBOX_LOCKFREE_DETAIL_HELPER_HPP

# include <chrono>
# include <thread>

namespace toolsbox
{
  namespace lockfree
  {
    namespace detail
    {
      inline void little_sleep()
      {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
      }
    }
  }
}

#endif

