#ifndef TOOLSBOX_VARIANT_DETAIL_VISITOR_HELPER_HPP
# define TOOLSBOX_VARIANT_DETAIL_VISITOR_HELPER_HPP

# include <utility>
# include <cstdlib>
# include <ostream>

namespace toolsbox
{
  namespace variant_detail
  {
    //
    // visitor helper
    template <class Variant, template <class T> class Comp> struct visitor_cmp
    {
      visitor_cmp(const Variant& v)
        : v_(v)
      {
      }

      template <class T> bool operator()(const T& data)
      {
        return Comp<T>()(data, v_.template as<T>());
      }

      const Variant& v_;
    };

    struct visitor_io
    {
      visitor_io(std::ostream& stream)
        : stream_(stream)
      {
      }

      template <class T> void operator()(const T& data)
      {
        stream_ << data;
      }

      std::ostream& stream_;
    };
  }
}

#endif

