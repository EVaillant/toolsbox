# Tools Box

It is c++ toolsbox library :
* only header
* no depend
* without rtti
* without exception

## What can i do with this library ?

* [any](./doc/any.md)
* [variant](./doc/variant.md)
* [lockfree](./doc/lockfree.md)
* [sig](./doc/sig.md)
* [for_each](./doc/for_each.md)
* [zip](./doc/zip.md)
* [zip_tuple](./doc/zip_tuple.md)
* [print_type](./doc/print_type.md)
* [static_map](./doc/static_map.md)
* [pool](./doc/pool.md)

## Required

* c++ compiler with support c++11 / c++14
* [cmake](https://cmake.org/) 3.0 or highter
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
CMAKE_INSTALL_PREFIX | path | /usr/local | Prefix installation
ENABLE_PERF_APPL_BUILD | ON or OFF | OFF | Build Perf Tools
DISABLE_UNITTEST | ON or OFF | OFF | Disable unittest

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
