# Tools Box

It is c++ toolsbox library :
* only header
* no depend
* without rtti
* without exception

## What can i do with this library ?

### any

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

### variant

prog :
```c++
#include <iostream>
#include <string>
#include <toolsbox/variant.hpp>

struct visitor1
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

struct visitor2
{
  std::string operator()(const int&)
  {
    return "always :)";
  }
  
  std::string operator()(const std::string&)
  {
    return ":(";
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
  v.apply_visitor(visitor1());
  
  // with visitor and return value
  std::cout << v.apply_visitor(visitor2()) << std::endl;
  
  return 0;
}
```

out :
```shell
it is int :)
5 5
it is always int :)
always :)
```


## Required

* c++ compiler with support c++11 and some part of c++14
* [cmake](https://cmake.org/) 2.6 or highter
* boost / unit test (not mandatory, to run unit test)
* boost / other headers lib (not mandatory, to run perf application)

## Build (Unix)

By default :
* unit test is builded (if there are boost unit test)
* in debug mode
* perf tools application are not builded

Common cmake option (to add on common line) :

 Option | Value | Default | Description
--------| ------|---------|------------
CMAKE_BUILD_TYPE | Debug or Release | Debug | Select build type
ENABLE_PERF_APPL_BUILD | ON or OFF | OFF | Build Perf Tools
MAKE_INSTALL_PREFIX | path | /usr/local | Prefix installation


run cmake :

```shell
toolsbox $ mkdir build && cd build
build    $ cmake ..
```

build :

```shell
build    $ make
```

run unit test :

```shell
build    $ make test
```

install :

```shell
build    $ make install
```

## Licence

cf [Boost Licence](http://www.boost.org/LICENSE_1_0.txt)
