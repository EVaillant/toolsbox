# Hello world

prog :
```c++

#include <iostream>
#include <toolsbox/mpl/print_type.hpp>

int main()
{
  std::cout << toolsbox::mpl::print_type<std::map<std::string, int>>::name() << std::endl;
  std::cout << toolsbox::mpl::print_type<std::tuple<std::string, int, double>>::name() << std::endl;

  return 0;
}

```

prog :
```shell
map(string, int)
tuple(string, int, double)
```
