#ifndef TOOLSBOX_ZIP_TUPLE_HPP
# define TOOLSBOX_ZIP_TUPLE_HPP

# include <tuple>
# include <utility>

namespace toolsbox
{
  namespace zip_tuple_detail
  {
    template <std::size_t I, class ... Tuples> auto zip_tuple_helper_once(Tuples&&... tuples)
    {
      return std::make_tuple(std::get<I>(tuples)...);
    }

    template <class ... Tuples, std::size_t ... I> auto zip_tuple_helper(std::index_sequence<I...>, Tuples&&... tuples)
    {
      return std::make_tuple(zip_tuple_helper_once<I>(std::forward<Tuples>(tuples)...)...);
    }
  }

  template <class ... Types, class ... Tuples> auto make_zip_tuple(std::tuple<Types...>&& tuple, Tuples&&... tuples)
  {
    return zip_tuple_detail::zip_tuple_helper(std::index_sequence_for<Types...>{}, std::forward<std::tuple<Types...>>(tuple), std::forward<Tuples>(tuples)...);
  }

  template <class ... Types, class ... Tuples> auto make_zip_tuple(std::tuple<Types...>&  tuple, Tuples&&... tuples)
  {
    return zip_tuple_detail::zip_tuple_helper(std::index_sequence_for<Types...>{}, std::forward<std::tuple<Types...>>(tuple), std::forward<Tuples>(tuples)...);
  }
}


#endif

