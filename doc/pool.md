# object_pool

```c++
#include <toolsbox/pool/object_pool.hpp>

struct Data
{
  Data();
  Data(const std::string&);
};

typedef toolsbox::pool::object_pool<Data> data_obj_pool;

int main()
{
  data_obj_pool pool;
  auto d1 = pool.acquire();
  auto d2 = pool.acquire("ddd");

  return 0;
}
```

## Interface
```c++
namespace toolsbox
{
  namespace pool
  {
    template <class T> class obect_pool
    {
      public:
        typedef T                                 type;
        typedef std::unique_ptr<T,/* something*/>  ptr_type;

        template <class ... ARGS> ptr_type acquire(ARGS&& ... args);
       
        std::size_t capacity() const;
        std::size_t size() const;
        bool full() const;
    };
  }
}
```

## thread_pool

```c++
#include <toolsbox/pool/thread_pool.hpp>
#include <toolsbox/lockfree/fix_size_queue.hpp>

int worker(const std::string& name, int arg)
{
  // do lot of stuff
  return 1;
}

template <class T> using queue = typedef toolsbox::lockfree::fix_size_queue<T, 64>;
typedef toolsbox::pool::thread_pool<int (const std::string&, int), queue> thread_pool; 

int main()
{
  thread_pool pool;
  pool.bind_all(worker);
  pool.start(5);

  std::future<int> f = pool.push("rrr", 6);
  f.wait();
  assert(f.get() == 1);

  std::string uuu = "kikik";
  pool.push(std::cref(uuu), 6);

  return 0;
}

```

## Interface

```c++
namespace toolsbox
{
  namespace pool
  {
    template <class, template <class> class> class thread_pool;
    template <template <class> class Queue, class R, class ... Args> class thread_pool<R(Args...), Queue>
    {
      public:
        typedef R                                           return_type;
        typedef std::future<return_type>                    future_type;

        thread_pool();
        ~thread_pool();
        void bind_all(const target_type& target);
        void bind_by_thread(std::size_t id, const target_type& target);
        void start(std::size_t nb);
        void stop()
        template <class ... LArgs> future_type push(LArgs && ... args);
    };
  }
}
```

# Links
* [object pool benchmark](pool_benchmark.md)

