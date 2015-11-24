# Tools Box

It is c++ toolsbox library :
* only header
* no depend
* without rtti
* without exception

## What can i do with this library ?

* [any](https://github.com/EVaillant/toolsbox/wiki/any)
* [variant](https://github.com/EVaillant/toolsbox/wiki/variant)
* [lockfree](https://github.com/EVaillant/toolsbox/wiki/lockfree)
* [sig](https://github.com/EVaillant/toolsbox/wiki/sig)
* [for_each](https://github.com/EVaillant/toolsbox/wiki/for_each)
* [zip](https://github.com/EVaillant/toolsbox/wiki/zip)
* [zip_tuple](https://github.com/EVaillant/toolsbox/wiki/zip_tuple)
* [print_type](https://github.com/EVaillant/toolsbox/wiki/print_type)

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
