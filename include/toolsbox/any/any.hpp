#ifndef TOOLSBOX_ANY_ANY_HPP
# define TOOLSBOX_ANY_ANY_HPP

# include <toolsbox/any/any_value_impl.hpp>

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

        template<class T> bool is() const
        {
          return (!empty()&& (type_uid::get<T>() == value_->get_id()));
        }

        template<class T> const T* as() const
        {
          const T* ret = nullptr;
          if(is<T>())
          {
            const any_detail::any_value_impl<T>* impl = static_cast< const any_detail::any_value_impl<T>* >(value_.get());
            ret = &impl->get_value();
          }
          return ret;
        }

        template<class T> T* as()
        {
          T* ret = nullptr;
          if(is<T>())
          {
            any_detail::any_value_impl<T>* impl = static_cast<any_detail::any_value_impl<T>* >(value_.get());
            ret = &impl->get_value();
          }
          return ret;
        }

        friend std::ostream& operator << (std::ostream& stream , const any &a);

      protected:
        typedef any_detail::any_value::ptr_type any_value_ptr_type;

      private:
        any_value_ptr_type value_;
    };
  }
}

#endif

