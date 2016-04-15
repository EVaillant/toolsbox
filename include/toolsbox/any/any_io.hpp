#ifndef TOOLSBOX_ANY_ANY_IO_HPP
# define TOOLSBOX_ANY_ANY_IO_HPP

# include <ostream>

namespace toolsbox
{
   inline std::ostream& operator << (std::ostream& stream , const toolsbox::any &a)
   {
    if(a.value_)
    {
      a.value_->to_stream(stream);
    }
    return stream;
  }
}

namespace std
{
  template <class T> T* get(toolsbox::any& data) noexcept
  {
    return (data.is<T>() ? &data.as<T>() : nullptr);
  }

  template <class T> T* get(toolsbox::any&& data) noexcept
  {
    return (data.is<T>() ? &data.as<T>() : nullptr);
  }

  template <class T> const T* get(const toolsbox::any& data) noexcept
  {
    return (data.is<T>() ? &data.as<T>() : nullptr);
  }

  inline std::string to_string(const toolsbox::any&a)
  {
    std::ostringstream stream;
    stream << a;
    return stream.str();
  }
}

#endif

