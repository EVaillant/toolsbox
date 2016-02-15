#ifndef TOOLSBOX_APPLY_HPP
# define TOOLSBOX_APPLY_HPP

# include <utility>
# include <tuple>

namespace toolsbox
{
  namespace apply_detail
  {
    template <class F, class Tuple, std::size_t ... I> auto apply_impl(F&& functor, Tuple&& tuple, std::index_sequence<I...>)
    {
      return functor(std::get<I>(tuple)...);
    }
  }

  template <class F, class ... Args> auto apply(F&& functor, std::tuple<Args...>& args)
  {
    return apply_detail::apply_impl(std::forward<F>(functor), std::forward<std::tuple<Args...>>(args), std::index_sequence_for<Args...>{});
  }
}

#endif

