#ifndef TOOLSBOX_TYPE_UID_HPP
# define TOOLSBOX_TYPE_UID_HPP

# include <cstdint>

namespace toolsbox
{
  class type_uid
  {
    public:
      typedef uint32_t id_type;
      typedef id_type  counter_id_type;

      template <class T> static id_type get()
      {
        static id_type id = counter_();
        return id;
      }

    private:
      static counter_id_type counter_()
      {
        static counter_id_type counter = 0;
        return counter++;
      }
  };
}

#endif

