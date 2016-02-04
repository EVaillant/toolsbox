# Hello world

prog :
```c++
#include <vector>
#include <string>
#include <toolsbox/zip.hpp>

int main()
{
  std::vector<int>         input1 = {2, 6, 9};
  std::vector<std::string> input2 = {"trac", "blo", "bla"};

  for(auto& e : toolsbox::make_zip(input1, input2))
  {
    std::cout << std::get<0>(e) << ":" << std::get<1>(e) << std::endl;
  }

  return 0;
}
```

out :
```shell
2:trac
6:blo
9:bla
```
