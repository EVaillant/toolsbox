#ifndef TOOLSBOX_ANY_ANY_VALUE_HPP
# define TOOLSBOX_ANY_ANY_VALUE_HPP

# include <toolsbox/type_uid.hpp>

# include <memory>
# include <ostream>

# include <boost/type_traits/has_left_shift.hpp>

namespace toolsbox
{
  namespace any_detail
  {
    class any_value
    {
      public:
        typedef toolsbox::type_uid::id_type  id_type;
        typedef std::unique_ptr<any_value>   ptr_type;

        inline virtual ~any_value() {}

        virtual id_type  get_id() const = 0;
        virtual ptr_type copy()   const = 0;
        virtual void to_stream(std::ostream& stream) const = 0;
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
          return toolsbox::type_uid::get<type>();
        }

        virtual ptr_type copy() const override
        {
          return std::make_unique<self_type>(value_);
        }

        virtual void to_stream(std::ostream& stream) const override
        {
          to_stream_(stream, value_);
        }

        type& get_value()
        {
          return value_;
        }

        const type& get_value() const
        {
          return value_;
        }

      protected:
        template <class U> typename std::enable_if<boost::has_left_shift<std::ostream, U>::value>::type to_stream_(std::ostream& stream, const U& value) const
        {
          stream << value;
        }

        template <class U> void to_stream_(std::ostream& stream, const std::reference_wrapper<U>& ref) const
        {
          to_stream_(stream, ref.get());
        }

        template <class U> typename std::enable_if<!boost::has_left_shift<std::ostream, U>::value>::type to_stream_(std::ostream&, const U&) const
        {
        }

      private:
        type    value_;
    };
  }
}

#endif

