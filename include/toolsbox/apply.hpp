#ifndef TOOLSBOX_APPLY_HPP
# define TOOLSBOX_APPLY_HPP

# include <utility>
# include <tuple>
# include <type_traits>

namespace toolsbox
{
  namespace apply_detail
  {
    template <class F, class Tuple, std::size_t ... I> auto apply_impl(F&& functor, Tuple&& tuple, std::index_sequence<I...>)
    {
      return functor(std::get<I>(tuple)...);
    }
  }

  template <class F, class Tuple> auto apply(F&& f, Tuple&& t)
  {
    return apply_detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>{});
  }
}

#endif

