#ifndef TOOLSBOX_POOL_OBJECT_POOL_HPP
# define TOOLSBOX_POOL_OBJECT_POOL_HPP

# include <toolsbox/pool/detail/tree_node.hpp>
# include <toolsbox/pool/detail/linear_node.hpp>
# include <toolsbox/pool/detail/object_pool_impl.hpp>

namespace toolsbox
{
  namespace pool
  {
    template<class T> using tree_object_pool   = detail::basic_object_pool_impl<T, detail::tree_node<T>>;
    template<class T> using linear_object_pool = detail::basic_object_pool_impl<T, detail::linear_node<T>>;
    template<class T> using none_object_pool   = detail::none_object_pool_impl<T>;

    template<class T> using object_pool        = linear_object_pool<T>;
  }
}

#endif

