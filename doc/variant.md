# Hello world

prog :
```c++
#include <iostream>
#include <string>
#include <toolsbox/variant.hpp>

struct visitor
{
  void operator()(const int&)
  {
    std::cout << "it is always int :)" << std::endl;
  }
  
  void operator()(const std::string&)
  {
    std::cout << "it is string :(" << std::endl;
  }
};

int main()
{
  toolsbox::variant<int, std::string> v(5);
  
  // like any
  if(a.is<int>())
  {
    std::cout << "it is int :)" << std::endl;
    int v = a.as<int>();
    std::cout << v << " " << a << std::endl;
  }
  
  // with visitor
  v.apply_visitor(visitor());
  
  return 0;
}
```

out :
```shell
it is int :)
5 5
it is always int :)
```
