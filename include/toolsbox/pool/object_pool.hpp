#ifndef TOOLSBOX_POOL_OBJECT_POOL_HPP
# define TOOLSBOX_POOL_OBJECT_POOL_HPP

# include <toolsbox/pool/detail/tree_node.hpp>
# include <toolsbox/pool/detail/basic_tree_node.hpp>
# include <toolsbox/pool/detail/basic_mutex_stack.hpp>

# include <toolsbox/pool/detail/object_pool_impl.hpp>

namespace toolsbox
{
  namespace pool
  {
    template<class T> using old_tree_object_pool = detail::basic_object_pool_impl<T, detail::basic_tree_node<T>>;
    template<class T> using tree_object_pool     = detail::basic_object_pool_impl<T, detail::tree_node<T>>;
    template<class T> using linear_object_pool   = detail::basic_object_pool_impl<T, detail::basic_tree_node<T, 1>>;
    template<class T> using mutex_object_pool    = detail::basic_object_pool_impl<T, detail::basic_mutex_stack<T>>;
    template<class T> using none_object_pool     = detail::none_object_pool_impl<T>;

    template<class T> using object_pool          = linear_object_pool<T>;
  }
}

#endif

