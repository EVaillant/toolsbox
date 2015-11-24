#ifndef TOOLSBOX_MPL_STATIC_MAP_HPP
# define TOOLSBOX_MPL_STATIC_MAP_HPP

# include <tuple>
# include <utility>

namespace toolsbox
{
  namespace mpl
  {
    namespace static_map_detail
    {
      template <std::size_t I, class T, class ... > struct select_type
      {
      };

      template <std::size_t I, class T, class Key, class ... Keys, class Value, class ... Values> struct select_type<I, T, std::pair<Key, Value>, std::pair<Keys, Values>...>
      {
        static const std::size_t id = select_type<I, T, std::pair<Key, Value>>::id == 0 ? select_type<I+1, T, std::pair<Keys, Values>...>::id : select_type<I, T, std::pair<Key, Value>>::id;
      };

      template <std::size_t I, class T, class Key, class Value> struct select_type<I, T, std::pair<Key,Value>>
      {
        static const std::size_t id = 0;
      };

      template <std::size_t I, class T, class Value> struct select_type<I, T, std::pair<T,Value>>
      {
        static const std::size_t id = I;
      };
    }

    template <class ... > struct static_map
    {
    };

    template <class ... Keys, class ... Values> class static_map<std::pair<Keys, Values>...>
    {
      public:
        template <class T> using select_type = static_map_detail::select_type<1, T, std::pair<Keys, Values>...>;
        using data_type = std::tuple<Values...>;

        static_map()
        {
        }

        static_map(Values&& ... values)
          : data_(std::forward<Values>(values)...)
        {
        }

        template <class T> auto& as()
        {
          static_assert(select_type<T>::id != 0, "invalid type");
          return std::get<select_type<T>::id-1>(data_);
        }

        template <class T> const auto& as() const
        {
          static_assert(select_type<T>::id != 0, "invalid type");
          return std::get<select_type<T>::id-1>(data_);
        }

      private:
        data_type data_;
    };

    template <class ... Keys, class Value> class static_map<std::tuple<Keys...>, Value> : public static_map<std::pair<Keys, Value>...>
    {
      public:
        typedef static_map<std::pair<Keys, Value>...> inherent_type;
        using inherent_type::inherent_type;
    };
  }
}

namespace std
{
  template <class T, class ... Types> T& get(toolsbox::mpl::static_map<Types...>& data) noexcept
  {
    return data.template as<T>();
  }

  template <class T, class ... Types> const T& get(const toolsbox::mpl::static_map<Types...>& data) noexcept
  {
    return data.template as<T>();
  }
}

#endif
