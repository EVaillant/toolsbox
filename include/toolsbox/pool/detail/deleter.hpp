#ifndef TOOLSBOX_POOL_DETAIL_DELETER_HPP
# define TOOLSBOX_POOL_DETAIL_DELETER_HPP

# include <cassert>

namespace toolsbox
{
  namespace pool
  {
    namespace detail
    {
      template <class T, class O, class ID> class deleter
      {
        public:
          typedef O   owner_type;
          typedef ID  id_type;
          typedef T   type;

          deleter()
            : owner_(nullptr)
            , id_(-1)
          {
          }

          void init(owner_type& owner, id_type id)
          {
            owner_ = &owner;
            id_     = id;
          }

          void operator()(type* t)
          {
            assert(owner_);
            t->~T();
            owner_->release(id_, t);
          }

        private:
          owner_type* owner_;
          id_type     id_;
      };
    }
  }
}

#endif

