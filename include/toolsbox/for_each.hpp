#ifndef TOOLSBOX_FOR_EACH_HPP
# define TOOLSBOX_FOR_EACH_HPP

# include <utility>
# include <tuple>
# include <type_traits>

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

  template <class F, class Tuple> void for_each_by_tuple(F&& f, Tuple&& t)
  {
    for_each_detail::for_each_by_tuple_impl(std::forward<F>(f), std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>{});
  }
}

#endif

