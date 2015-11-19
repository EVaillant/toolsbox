#ifndef TOOLSBOX_ANY_ANY_VALUE_IMPL_HPP
# define TOOLSBOX_ANY_ANY_VALUE_IMPL_HPP

# include <toolsbox/any/any_value.hpp>
# include <cassert>

namespace toolsbox
{
  namespace any_detail
  {
    template <class T> class any_value_impl : public any_value
    {
      public:
        typedef any_value_impl<T> self_type;

        any_value_impl(const T &t, id_type id)
          : value_(t)
          , id_(id)
        {
        }

        any_value_impl(T &&t, id_type id)
          : value_(std::forward<T>(t))
          , id_(id)
        {
        }

        virtual id_type get_id() const override
        {
          return id_;
        }

        virtual ptr_type clone() const override
        {
          return ptr_type(new any_value_impl(value_, id_));
        }

        virtual bool equal_to(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ == impl_a->value_);
        }

        virtual bool not_equal_to(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ != impl_a->value_);
        }

        virtual bool less(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ < impl_a->value_);
        }

        virtual bool greater(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ > impl_a->value_);
        }

        virtual bool less_equal(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ <= impl_a->value_);
        }

        virtual bool greater_equal(const any_value& a) const override
        {
          assert(a.get_id() == get_id());
          const any_value_impl* impl_a = static_cast<const any_value_impl*>(&a);
          return (value_ >= impl_a->value_);
        }

        virtual std::size_t hash() const override
        {
          return std::hash<T>()(value_);
        }

        virtual void to_string(std::ostream& stream) const override
        {
          stream << value_;
        }

        T& get_value()
        {
          return value_;
        }

        const T& get_value() const
        {
          return value_;
        }

      private:
        T value_;
        id_type id_;
    };
  }
}

#endif

