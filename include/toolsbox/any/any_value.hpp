#ifndef TOOLSBOX_ANY_ANY_VALUE_HPP
# define TOOLSBOX_ANY_ANY_VALUE_HPP

# include <toolsbox/type_uid.hpp>

# include <memory>
# include <ostream>

namespace toolsbox
{
  namespace any_detail
  {
    class any_value
    {
      public:
        typedef type_uid::id_type          id_type;
        typedef std::unique_ptr<any_value> ptr_type;

        virtual inline ~any_value()
        {
        }

        virtual id_type get_id() const = 0;
        virtual ptr_type clone() const = 0;
        virtual bool equal_to(const any_value& a) const = 0;
        virtual bool not_equal_to(const any_value& a) const = 0;
        virtual bool less(const any_value& a) const = 0;
        virtual bool greater(const any_value& a) const = 0;
        virtual bool less_equal(const any_value& a) const = 0;
        virtual bool greater_equal(const any_value& a) const = 0;
        virtual void to_string(std::ostream& stream) const = 0;
        virtual std::size_t hash() const = 0;
    };
  }
}

#define override_opt_any_value(opt) \
  template <> struct opt <toolsbox::any_detail::any_value>                                                            \
  {                                                                                                                   \
    bool operator()( const toolsbox::any_detail::any_value& lhs, const toolsbox::any_detail::any_value& rhs ) const   \
    {                                                                                                                 \
      return lhs.opt(rhs);                                                                                            \
    }                                                                                                                 \
  }

namespace std
{
  override_opt_any_value(equal_to);
  override_opt_any_value(not_equal_to);
  override_opt_any_value(less);
  override_opt_any_value(greater);
  override_opt_any_value(less_equal);
  override_opt_any_value(greater_equal);
}

#undef override_opt_any_value

#endif

