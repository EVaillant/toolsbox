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
    };
  }
}

#endif

