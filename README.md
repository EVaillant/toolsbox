# Tools Box

It is full c++ header library

## what can i do with this header library ?

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

run cmake :

```shell
toolsbox $ mkdir build && cd build
build    $ cmake ..
```

if you want to :
* compile in release mode, add *-DCMAKE_BUILD_TYPE=Release* on cmake command line
* build perf tools, add *-DENABLE_PERF_APPL_BUILD=ON* on cmake command line

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

if you want to :
* select your install prefix, add *-DCMAKE_INSTALL_PREFIX=/whatever* on cmake command line

## Licence

cf [Boost Licence](http://www.boost.org/LICENSE_1_0.txt)