#ifndef TOOLSBOX_ANY_ANY_IO_HPP
# define TOOLSBOX_ANY_ANY_IO_HPP

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
}

#endif

