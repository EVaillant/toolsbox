#ifndef TOOLSBOX_ZIP_HPP
# define TOOLSBOX_ZIP_HPP

# include <type_traits>
# include <tuple>
# include <utility>
# include <iterator>

namespace toolsbox
{
  namespace zip_detail
  {
    template<class F, class Tuple, std::size_t ... I> auto make_iterator(F&& functor, Tuple&& tuple, std::index_sequence<I...>)
    {
      return std::make_tuple(functor(std::get<I>(tuple))...);
    }

#define functor_helper(name, fct)               \
    struct name                                 \
    {                                           \
      template <class T> auto operator()(T& t)  \
      {                                         \
        return fct(t);                          \
      }                                         \
    };                                          \
                                                \
    template<class ... E> auto name##_iterator(const std::tuple<E...>& tuple)  \
    {                                                                          \
      return make_iterator(name(), tuple, std::index_sequence_for<E...>{});     \
    }                                                                          \
    template<class ... E> auto name##_iterator(std::tuple<E...>& tuple)        \
    {                                                                          \
      return make_iterator(name(), tuple, std::index_sequence_for<E...>{});    \
    }

    functor_helper(begin, std::begin)
    functor_helper(end,   std::end)
    functor_helper(prev,  std::prev)
    functor_helper(next,  std::next)
#undef functor_helper

    template<class Tuple, std::size_t ... I> bool common_iterator(const Tuple& left, const Tuple& right, std::index_sequence<I...>)
    {
      bool cmp[] = {(std::get<I>(left) == std::get<I>(right))...};
      return std::find(std::begin(cmp), std::end(cmp), true) != std::end(cmp);
    }

    template<class ... Types> bool common_iterator(const std::tuple<Types...>& left, const std::tuple<Types...>& right)
    {
      return common_iterator(left, right, std::index_sequence_for<Types...>{});
    }

    template <class Iterator, class Data> class zip_iterator
    {
      public:
        typedef Iterator                   inner_iterator_type;
        typedef const inner_iterator_type  const_inner_iterator_type;
        typedef Data                       data_type;

        zip_iterator(data_type& data, inner_iterator_type && ii)
          : data_(data)
          , inner_iterator_(std::move(ii))
        {
        }

        zip_iterator& operator++()
        {
          secure_next_();
          return *this;
        }

        zip_iterator operator++(int)
        {
          inner_iterator_type tmp = inner_iterator_;
          zip_iterator ret(data_, std::move(tmp));
          secure_next_();
          return ret;
        }

        zip_iterator& operator--()
        {
          inner_iterator_ = prev_iterator(inner_iterator_);
          return *this;
        }

        zip_iterator operator--(int)
        {
          inner_iterator_type tmp = inner_iterator_;
          zip_iterator ret(data_, std::move(tmp));
          inner_iterator_ = prev_iterator(inner_iterator_);
          return ret;
        }

        bool operator==(const zip_iterator& other) const
        {
          return inner_iterator_ == other.inner_iterator_;
        }

        bool operator!=(const zip_iterator& other) const
        {
          return ! (*this == other);
        }

        zip_iterator& operator*()
        {
          return *this;
        }

        zip_iterator& operator->()
        {
          return *this;
        }

        const zip_iterator& operator*() const
        {
          return *this;
        }

        const zip_iterator& operator->() const
        {
          return *this;
        }

        template <size_t I> auto& get()
        {
          return *std::get<I>(inner_iterator_);
        }

        template <size_t I> const auto& get() const
        {
          return *std::get<I>(inner_iterator_);
        }

      protected:
        void secure_next_()
        {
          inner_iterator_ = next_iterator(inner_iterator_);
          inner_iterator_type end_inner_iterator = end_iterator(data_);
          if(common_iterator(inner_iterator_, end_inner_iterator))
          {
            inner_iterator_ = std::move(end_inner_iterator);
          }
        }

      private:
        data_type&          data_;
        inner_iterator_type inner_iterator_;
    };

    template <class ... Container> class zip
    {
      public:
        typedef std::tuple<Container...> data_t;

      private:
        data_t data_;

      public:
        typedef decltype( begin_iterator(data_) )                  inner_iterator_t;
        typedef decltype( begin_iterator((const data_t) data_) )   inner_const_iterator_t;

        typedef zip_iterator<inner_iterator_t,data_t>              iterator;
        typedef zip_iterator<inner_const_iterator_t, const data_t> const_iterator;

        zip(Container && ... containers)
          : data_(std::forward<Container>(containers)...)
        {
        }

        iterator begin()
        {
          return iterator(data_, begin_iterator(data_));
        }

        const_iterator begin() const
        {
          return const_iterator(data_, begin_iterator(data_));
        }

        iterator end()
        {
          return iterator(data_, end_iterator(data_));
        }

        const_iterator end() const
        {
          return const_iterator(data_, end_iterator(data_));
        }
    };

    template <class T> using zip_convert_t = typename std::conditional<std::is_reference<T>::value, typename std::add_lvalue_reference<T>::type, T>::type;
  }

  template <class ... Container> auto make_zip(Container&& ... containers)
  {
    return zip_detail::zip<zip_detail::zip_convert_t<Container>...>(std::forward<Container>(containers)...);
  }
}

namespace std
{
  template <size_t I, class A, class B> auto& get(toolsbox::zip_detail::zip_iterator<A, B>& data) noexcept
  {
    return data.get<I>();
  }

  template <size_t I, class A, class B> auto& get(const toolsbox::zip_detail::zip_iterator<A, B>& data) noexcept
  {
    return data.get<I>();
  }
}

#endif

