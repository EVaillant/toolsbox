#ifndef TOOLSBOX_VARIANT_DETAIL_VARIANT_HPP
# define TOOLSBOX_VARIANT_DETAIL_VARIANT_HPP

# include <toolsbox/mpl/static_size.hpp>

# include <toolsbox/variant/result_of_visitor.hpp>
# include <toolsbox/variant/select_type.hpp>
# include <toolsbox/variant/variant_visitor.hpp>
# include <toolsbox/variant/visitor_helper.hpp>

# include <type_traits>
# include <utility>

namespace toolsbox
{
  namespace variant_detail
  {
    template <class ... Types> class variant
    {
      public:
        typedef size_t index_type;

        template <class T> using select_type       = toolsbox::variant_detail::select_type<T, Types...>;
        template <class R> using variant_visitor   = toolsbox::variant_detail::variant_visitor<R, Types...>;
        template <class F> using result_of_visitor = toolsbox::variant_detail::result_of_visitor<F, Types&...>;

        static const size_t max_size = toolsbox::mpl::static_max<sizeof (Types)...>::max;
        static const size_t max_alig = toolsbox::mpl::static_max<alignof(Types)...>::max;
        static const index_type first_index   = 1;
        static const index_type invalid_index = 0;

        typedef typename std::aligned_storage<max_size, max_alig>::type storage_type;

        variant()
          : index_(invalid_index)
        {
        }

        variant(const variant& v)
          : index_(v.index_)
        {
          if(index_ != invalid_index)
          {
            apply_visitor(visitor_copy<variant>(v, true));
          }
        }

        variant(variant&& v)
          : index_(v.index_)
        {
          if(index_ != invalid_index)
          {
            apply_visitor(visitor_move<variant>(v, true));
            v.index_ = invalid_index;
          }
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> explicit variant(const T& value)
          : index_(select_type<T>::id)
        {
          new(&storage_) T (value);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> explicit variant(T && value)
          : index_(select_type<T>::id)
        {
          new(&storage_) T (std::forward<T>(value));
        }

        ~variant()
        {
          reset();
        }

        template <class T, class ... Args> void emplace(Args&& ... args)
        {
          reset();
          index_ = select_type<T>::id;
          static_assert(select_type<T>::id != invalid_index, "invalid type");
          new(&storage_) T (std::forward<Args>(args)...);
        }

        void reset()
        {
          if(!empty())
          {
            apply_visitor(visitor_delete(*this));
          }
        }

        template <class T> bool is() const
        {
          return select_type<T>::id == index_;
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> T& as()
        {
          return *reinterpret_cast<T*>(&storage_);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> const T& as() const
        {
          return *reinterpret_cast<const T*>(&storage_);
        }

        bool empty() const
        {
          return index_ == invalid_index;
        }

        variant& operator=(const variant& v)
        {
          return copy_(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> variant& operator=(const T& v)
        {
          typedef wrapper<const T> local_wrapper;
          return copy_(local_wrapper(v));
        }

        variant& operator=(variant&& v)
        {
          return move_(std::forward<variant>(v));
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> variant& operator=(T&& v)
        {
          return move_(wrapper<T>(v));
        }

        bool operator==(const variant& v) const
        {
          return cmp_<variant, std::equal_to>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator==(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::equal_to>(local_wrapper(v));
        }

        bool operator!=(const variant& v) const
        {
          return cmp_<variant, std::not_equal_to>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator!=(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::not_equal_to>(local_wrapper(v));
        }

        bool operator<(const variant& v) const
        {
          return cmp_<variant, std::less>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator<(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::less>(local_wrapper(v));
        }

        bool operator<=(const variant& v) const
        {
          return cmp_<variant, std::less_equal>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator<=(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::less_equal>(local_wrapper(v));
        }

        bool operator>(const variant&v) const
        {
          return cmp_<variant, std::greater>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator>(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::greater>(local_wrapper(v));
        }

        bool operator>=(const variant& v) const
        {
          return cmp_<variant, std::greater_equal>(v);
        }

        template <class T, class D = typename std::enable_if<select_type<T>::id != invalid_index>::type> bool operator>=(const T& v) const
        {
          typedef wrapper<const T> local_wrapper;
          return cmp_<local_wrapper, std::greater_equal>(local_wrapper(v));
        }

        template <class F> auto apply_visitor(F&& f)
        {
          typedef typename result_of_visitor<F>::type result_type;
          return variant_visitor<result_type>::apply(std::forward<F>(f), *this, first_index, index_);
        }

        template <class F> auto apply_visitor(F&& f) const
        {
          typedef typename result_of_visitor<F>::type result_type;
          return variant_visitor<result_type>::apply(std::forward<F>(f), *this, first_index, index_);
        }

      protected:
        template <class V> variant& move_(V&& v)
        {
          if(index_ == v.index_)
          {
            if(! v.empty())
            {
              apply_visitor(visitor_move<V>(v, false));
            }
          }
          else
          {
            reset();
            if(!v.empty())
            {
              index_ = v.index_;
              apply_visitor(visitor_move<V>(v, true));
            }
          }
          return *this;
        }

        template <class V> variant& copy_(const V& v)
        {
          if(index_ == v.index_)
          {
            if(! v.empty())
            {
              apply_visitor(visitor_copy<V>(v, false));
            }
          }
          else
          {
            reset();
            if(!v.empty())
            {
              index_ = v.index_;
              apply_visitor(visitor_copy<V>(v, true));
            }
          }

          return *this;
        }

        template <class Variant, template <class D> class Comp> bool cmp_(const Variant&v) const
        {
          bool ret = false;
          if(v.index_ == index_)
          {
            if(empty())
            {
              ret = Comp<bool>()(empty(), v.empty());
            }
            else
            {
              ret = apply_visitor(visitor_cmp<Variant, Comp>(v));
            }
          }
          else
          {
            ret = Comp<index_type>()(index_, v.index_);
          }
          return ret;
        }

        template <class D> struct wrapper
        {
          typedef typename std::remove_const<D>::type type;
          static_assert(select_type<type>::id != invalid_index, "invalid type");

          wrapper(D& value)
            : value_(value)
            , index_(select_type<type>::id)
          {
          }

          bool empty() const
          {
            return false;
          }

          template <class U, class Z = typename std::enable_if<std::is_same<U, type>::value>::type> const type& as() const
          {
            return value_;
          }

          template <class U, class Z = typename std::enable_if<!std::is_same<U, type>::value>::type> const U& as() const
          {
            static U local = U();
            return local;
          }

          template <class U, class Z = typename std::enable_if<std::is_same<U, type>::value>::type> type& as()
          {
            return value_;
          }

          template <class U, class Z = typename std::enable_if<!std::is_same<U, type>::value>::type> U& as()
          {
            static U local = U();
            return local;
          }

          D&           value_;
          index_type   index_;
        };

        struct visitor_delete
        {
          visitor_delete(variant& self)
            : self_(self)
          {
          }

          template <class T> void operator()(T& data)
          {
            self_.index_ = invalid_index;
            data.~T();
          }

          variant& self_;
        };

        template <class Variant> struct visitor_copy
        {
          visitor_copy(const Variant& cpy, bool ctr)
            : cpy_(cpy)
            , ctr_(ctr)
          {
          }

          template <class T> void operator()(T& data)
          {
            if(ctr_)
            {
              new (&data) T(cpy_.as<T>());
            }
            else
            {
              data = cpy_.as<T>();
            }
          }

          const Variant& cpy_;
          bool           ctr_;
        };

        template <class Variant> struct visitor_move
        {
          visitor_move(Variant& mv, bool ctr)
            : mv_(mv)
            , ctr_(ctr)
          {
          }

          template <class T> void operator()(T& data)
          {
            mv_.index_ = invalid_index;
            if(ctr_)
            {
              new (&data) T(std::move(mv_.as<T>()));
            }
            else
            {
              data = std::move(mv_.as<T>());
            }
          }

          Variant& mv_;
          bool     ctr_;
        };

      private:
        storage_type storage_;
        index_type   index_;
    };
  }
}

namespace std
{
  template <class T, class ... Types> T* get(toolsbox::variant_detail::variant<Types...>& data) noexcept
  {
    return (data.template is<T>() ? &data.template as<T>() : nullptr);
  }

  template <class T, class ... Types> const T* get(const toolsbox::variant_detail::variant<Types...>& data) noexcept
  {
    return (data.template is<T>() ? &data.template as<T>() : nullptr);
  }
}

#endif

