#ifndef TOOLSBOX_FOR_EACH_HPP
# define TOOLSBOX_FOR_EACH_HPP

# include <utility>
# include <tuple>

namespace toolsbox
{
  template <class F, class ... Types> void for_each(F&& functor, Types&&...values);

  namespace for_each_detail
  {
    template <class F, class Tuple, std::size_t ... I> void for_each_by_tuple_impl(F&& functor, Tuple&& tuple, std::index_sequence<I...>)
    {
      toolsbox::for_each(std::forward<F>(functor), std::get<I>(tuple)...);
    }
  }

  template <class F, class ... Types> void for_each(F&& functor, Types&&...values)
  {
    int tmp[] = {(functor(std::forward<Types>(values)),0)...};
    (void)tmp;
  }

  template <class F, class ... Types> void for_each_by_tuple(F&& functor, std::tuple<Types...> && tuple)
  {
    for_each_detail::for_each_by_tuple_impl(std::forward<F>(functor), std::forward<std::tuple<Types...>>(tuple), std::index_sequence_for<Types...>{});
  }
}

#endif

