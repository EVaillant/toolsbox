#ifndef TOOLSBOX_POOL_DETAIL_SEGMENT_HPP
# define TOOLSBOX_POOL_DETAIL_SEGMENT_HPP

# include <toolsbox/pool/detail/helper.hpp>

# include <array>
# include <atomic>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T, class D = void> struct segment_info
      {
        static_assert(sizeof(T) > 0, "only specialize must be instancied");
      };

      template <class T> struct segment_info<T, typename std::enable_if<sizeof(T*) == sizeof(uint32_t)>::type>
      {
        typedef uint32_t         flag_type;
        static const std::size_t size = 32;
      };

      template <class T> struct segment_info<T, typename std::enable_if<sizeof(T*) == sizeof(uint64_t)>::type>
      {
        typedef uint64_t         flag_type;
        static const std::size_t size = 64;
      };

      template <class T> class segment
      {
        public:
          typedef T                                                                 type;
          typedef segment_info<type>                                                info_type;
          typedef std::atomic<typename info_type::flag_type>                        flag_type;
          typedef typename info_type::flag_type                                     id_type;
          static const std::size_t                                                  nb_element = info_type::size;
          typedef segment<type>                                                     self_type;
          typedef typename std::aligned_storage<sizeof(type), alignof(type)>::type  data_type;
          typedef std::array<data_type, nb_element>                                 array_data_type;

          segment()
            : flag_(-1)
          {
          }

          bool acquire(void*& data, id_type &id)
          {
            id_type flag = flag_.load();
            id_type new_flag;
            do
            {
              if(flag == 0)
              {
                break;
              }
              id = get_bit(flag);
              new_flag = flag & ~id;
            } while(! flag_.compare_exchange_weak(flag, new_flag));
            if(flag != 0)
            {
              data = &data_[find_first_bit(id) - 1];
            }
            else
            {
              data = nullptr;
            }
            return data != nullptr;
          }

          void release(id_type id)
          {
            id_type flag = flag_.load();
            id_type new_flag;
            do
            {
              new_flag = flag | id;
            } while(! flag_.compare_exchange_weak(flag, new_flag));
          }

          std::size_t capacity() const
          {
            return nb_element;
          }

          std::size_t size() const
          {
            id_type flag = flag_.load();
            return count_bit(flag);
          }

          bool full() const
          {
            id_type flag = flag_.load();
            return flag == 0;
          }

        private:
          flag_type       flag_;
          array_data_type data_;
      };
    }
  }
}

#endif

