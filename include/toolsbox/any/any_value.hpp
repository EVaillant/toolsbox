#ifndef TOOLSBOX_ANY_ANY_VALUE_HPP
# define TOOLSBOX_ANY_ANY_VALUE_HPP

# include <toolsbox/type_uid.hpp>

# include <memory>
# include <ostream>
# include <functional>

namespace toolsbox
{
  namespace any_detail
  {
    typedef toolsbox::type_uid::id_type  id_type;

    template <class T> struct add_ref_wrapper
    {
      typedef T type;
    };

    template <class T> struct add_ref_wrapper<T&>
    {
      typedef std::reference_wrapper<T> type;
    };

    template <class T> struct remove_ref_wrapper
    {
      typedef T type;
    };

    template <class T> struct remove_ref_wrapper<std::reference_wrapper<T>>
    {
      typedef T& type;
    };

    class any_value
    {
      public:
        typedef std::unique_ptr<any_value>   ptr_type;

        inline virtual ~any_value() {}

        virtual id_type  get_id() const = 0;
        virtual ptr_type copy()   const = 0;
    };

    template <class T> class any_value_impl : public any_value
    {
      public:
        typedef T                    type;
        typedef any_value_impl<type> self_type;

        template <class V> any_value_impl(V && value)
          : value_(std::forward<V>(value))
        {
        }

        virtual id_type  get_id() const override
        {
          return toolsbox::type_uid::get<typename remove_ref_wrapper<type>::type>();
        }

        virtual ptr_type copy() const override
        {
          return std::make_unique<self_type>(value_);
        }

        type& get_value()
        {
          return value_;
        }

        const type& get_value() const
        {
          return value_;
        }

      private:
        type    value_;
    };
  }
}

#endif

