#ifndef TOOLSBOX_ANY_ANY_HPP
# define TOOLSBOX_ANY_ANY_HPP

# include <toolsbox/any/any_value_impl.hpp>

# include <cassert>

namespace toolsbox
{
  namespace any_detail
  {
    class any
    {
      public:
        inline any()
        {
        }

        inline any(const any& a)
        {
          value_ = a.value_->clone();
        }

        inline any(any&& a)
          : value_(std::move(a.value_))
        {
        }

        template <class T> any(const T& t)
        {
          *this = t;
        }

        template <class T> any(T&& t)
        {
          *this = std::forward<T>(t);
        }

        inline void reset()
        {
          value_.reset();
        }

        inline bool empty() const
        {
          return value_.get() == nullptr;
        }

        any& operator=(const any& a)
        {
          value_ = a.value_->clone();
          return *this;
        }

        template <class T> any& operator=(const T& t)
        {
          value_.reset(new any_detail::any_value_impl<T>(t, type_uid::get<T>()));
          return *this;
        }

        template <class T> any& operator=(T&& t)
        {
          value_.reset(new any_detail::any_value_impl<T>(std::forward<T>(t), type_uid::get<T>()));
          return *this;
        }

        bool operator==(const any &a) const
        {
          return cmp_<std::equal_to>(a);
        }

        template <class T> bool operator==(const T& t) const
        {
          return cmp_<T, std::equal_to>(t);
        }

        bool operator!=(const any &a) const
        {
          return cmp_<std::not_equal_to>(a);
        }

        template <class T> bool operator!=(const T& t) const
        {
          return cmp_<T, std::not_equal_to>(t);
        }

        bool operator<(const any &a) const
        {
          return cmp_<std::less>(a);
        }

        template <class T> bool operator<(const T& t) const
        {
          return cmp_<T, std::less>(t);
        }

        bool operator<=(const any &a) const
        {
          return cmp_<std::less_equal>(a);
        }

        template <class T> bool operator<=(const T& t) const
        {
          return cmp_<T, std::less_equal>(t);
        }

        bool operator>(const any &a) const
        {
          return cmp_<std::greater>(a);
        }

        template <class T> bool operator>(const T& t) const
        {
          return cmp_<T, std::greater>(t);
        }

        bool operator>=(const any &a) const
        {
          return cmp_<std::greater_equal>(a);
        }

        template <class T> bool operator>=(const T& t) const
        {
          return cmp_<T, std::greater_equal>(t);
        }

        template<class T> bool is() const
        {
          return (!empty()&& (type_uid::get<T>() == value_->get_id()));
        }

        template<class T> const T& as() const
        {
          assert(is<T>());
          const any_detail::any_value_impl<T>* impl = static_cast< const any_detail::any_value_impl<T>* >(value_.get());
          return impl->get_value();
        }

        template<class T> T& as()
        {
          assert(is<T>());
          any_detail::any_value_impl<T>* impl = static_cast<any_detail::any_value_impl<T>* >(value_.get());
          return impl->get_value();
        }

        friend std::ostream& operator << (std::ostream& stream , const any &a);
        friend class std::hash<any>;

      protected:
        typedef any_detail::any_value::ptr_type any_value_ptr_type;

        template <template <class D> class Comp> bool cmp_(const any& a) const
        {
          bool ret =  false;
          if(empty() || a.empty())
          {
            ret = Comp<bool>()(a.empty(), empty());
          }
          else if(value_->get_id() == a.value_->get_id())
          {
            ret = Comp<any_value>()(*value_, *a.value_);
          }
          else
          {
            ret = Comp<any_value::id_type>()(value_->get_id(), a.value_->get_id());
          }
          return ret;
        }

        template <class T, template <class D> class Comp> bool cmp_(const T& a) const
        {
          bool ret =  false;
          if(empty())
          {
            ret = Comp<bool>()(false, empty());
          }
          else if(is<T>())
          {
            ret = Comp<T>()(as<T>(), a);
          }
          else
          {
            ret = Comp<any_value::id_type>()(value_->get_id(), type_uid::get<T>());
          }
          return ret;
        }

      private:
        any_value_ptr_type value_;
    };

    template <class T> bool operator<(const T& t, const any& v)
    {
     return (v > t);
    }

    template <class T> bool operator<=(const T& t, const any& v)
    {
     return (v >= t);
    }

    template <class T> bool operator>(const T& t, const any& v)
    {
     return (v < t);
    }

    template <class T> bool operator>=(const T& t, const any& v)
    {
     return (v <= t);
    }

    template <class T> bool operator==(const T& t, const any& v)
    {
     return (v == t);
    }

    template <class T> bool operator!=(const T& t, const any& v)
    {
     return (v != t);
    }
  }
}

namespace std
{
  template <class T> T* get(toolsbox::any_detail::any& data) noexcept
  {
    return (data.is<T>() ? &data.as<T>() : nullptr);
  }

  template <class T> const T* get(const toolsbox::any_detail::any& data) noexcept
  {
    return (data.is<T>() ? &data.as<T>() : nullptr);
  }
}

#endif

