# Hello world

prog :
```c++
#include <iostream>
#include <toolsbox/for_each.hpp>
#include <tuple>

struct functor
{
  template <class T> void operator()(const T& value)
  {
    std::cout << value << std::endl; 
  }
};

int main()
{  
  toolsbox::for_each(functor(), 1, 2.5, "bla");
  toolsbox::for_each_by_tuple(functor(), std::make_tuple(1, 2.5, "bla"));

  return 0;
}
```

out:
```shell
1
2.5
bla
1
2.5
bla
```
