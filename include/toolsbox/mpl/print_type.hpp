#ifndef TOOLSBOX_MPL_PRINT_TYPE_HPP
# define TOOLSBOX_MPL_PRINT_TYPE_HPP

# include <string>
# include <tuple>
# include <vector>
# include <list>
# include <set>
# include <map>

namespace toolsbox
{
  namespace mpl
  {
    template <class T> struct print_type
    {
      static std::string name()
      {
        static_assert(sizeof(T) == 0, "unknwon type");
        return "unknwon type";
      }
    };

    template <> struct print_type<std::string>
    {
      static std::string name()
      {
        return "string";
      }
    };

    template <> struct print_type<double>
    {
      static std::string name()
      {
        return "double";
      }
    };

    template <> struct print_type<int>
    {
      static std::string name()
      {
        return "int";
      }
    };

    template <class T> struct print_type<std::vector<T>>
    {
      static std::string name()
      {
        return "vector(" + print_type<T>::name() + ")";
      }
    };

    template <class T> struct print_type<std::list<T>>
    {
      static std::string name()
      {
        return "list(" + print_type<T>::name() + ")";
      }
    };

    template <class T> struct print_type<std::set<T>>
    {
      static std::string name()
      {
        return "set(" + print_type<T>::name() + ")";
      }
    };

    template <class K, class V> struct print_type<std::map<K, V>>
    {
      static std::string name()
      {
        return "map(" + print_type<K>::name() + ", "  + print_type<V>::name() + ")";
      }
    };

    template <class T> struct print_type<T&>
    {
      static std::string name()
      {
        return print_type<T>::name() + "&";
      }
    };

    template <class T> struct print_type<T&&>
    {
      static std::string name()
      {
        return print_type<T>::name() + "&&";
      }
    };

    template <class T> struct print_type<T*>
    {
      static std::string name()
      {
        return print_type<T>::name() + "*";
      }
    };

    template <class T> struct print_type<const T>
    {
      static std::string name()
      {
        return "const " + print_type<T>::name();
      }
    };

    template <class ... T> struct print_type<std::tuple<T...>>
    {
      static std::string name()
      {
        std::string ret = "tuple(";
        std::vector<std::string> sub_types {print_type<T>::name()...};
        bool first = true;
        for(const std::string& type : sub_types)
        {
          if(!first)
          {
            ret += ", ";
          }
          ret += type;
          first = false;
        }
        ret += ")";
        return ret;
      }
    };
  }
}

#endif

