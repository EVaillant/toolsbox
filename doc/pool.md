# Hello world

```c++
# include <toolsbox/pool/object_pool.hpp>

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

# Interface
```c++
namespace toolsbox
{
  namespace pool
  {
    template <class T> class obect_pool
    {
      public:
        typedef T                                 type;
        typedef std::unique_ptr<T,/* somthing*/>  ptr_type;

        template <class ... ARGS> ptr_type acquire(ARGS&& ... args);
       
        std::size_t capacity() const;
        std::size_t size() const;
        bool full() const;
    };
  }
}
```

# Links
* [benchmark](pool_benchmark.md)
