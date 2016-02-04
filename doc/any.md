# Hello world

prog :
```c++
#include <iostream>
#include <toolsbox/any.hpp>

int main()
{
  toolsbox::any a = 5;
  if(a.is<int>())
  {
    std::cout << "it is int :)" << std::endl;
    int v = a.as<int>();
    std::cout << v << " " << a << std::endl;
  }
  return 0;
}
```

out :
```shell
it is int :)
5 5
```
