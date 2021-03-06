#ifndef TOOLSBOX_ANY_HPP
# define TOOLSBOX_ANY_HPP

# include <toolsbox/any/any_value.hpp>

# include <cassert>

namespace toolsbox
{
  class any
  {
    public:
      typedef any_detail::id_type id_type;

      inline any()
      {
      }

      inline any(const any& a)
      {
        *this = a;
      }

      inline any(any&& a)
      {
        *this = std::forward<any&&>(a);
      }

      template <class T, class V = typename std::enable_if<!std::is_same<std::decay_t<T>, any>::value>::type> any(T&& v)
      {
        *this = std::forward<T&&>(v);
      }

      inline bool empty() const
      {
        return value_.get() == nullptr;
      }

      inline void reset()
      {
        value_.reset();
      }

      template<class T> bool is() const
      {
        return (!empty() && value_->get_id() == toolsbox::type_uid::get<T>());
      }

      id_type get_id() const
      {
        assert(!empty());
        return value_->get_id();
      }

      template <class T> T& as()
      {
        typedef typename any_detail::add_ref_wrapper<T>::type type_id;
        assert(is<T>());
        any_detail::any_value_impl<type_id>* impl = static_cast<any_detail::any_value_impl<type_id>* >(value_.get());
        return impl->get_value();
      }

      template <class T> const T& as() const
      {
        typedef typename any_detail::add_ref_wrapper<T>::type type_id;
        assert(is<T>());
        const any_detail::any_value_impl<type_id>* impl = static_cast<any_detail::any_value_impl<type_id>* >(value_.get());
        return impl->get_value();
      }

      inline any& operator=(const any& a)
      {
        if(a.empty())
        {
          reset();
        }
        else if (this != &a)
        {
          value_ = a.value_->copy();
        }
        return *this;
      }

      inline any& operator=(any&& a)
      {
        if (this != &a)
        {
          value_ = std::move(a.value_);
        }
        return *this;
      }

      template <class T, class V = typename std::enable_if<!std::is_same<std::decay_t<T>, any>::value>::type> any& operator=(T&& v)
      {
        typedef std::decay_t<T> store_type;
        value_.reset(new any_detail::any_value_impl<store_type>(std::forward<T>(v)));
        return *this;
      }

    private:
      typedef std::unique_ptr<any_detail::any_value> value_type;
      value_type value_;
  };
}

# include <toolsbox/any/any_io.hpp>

#endif

