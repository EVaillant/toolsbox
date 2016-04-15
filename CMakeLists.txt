cmake_minimum_required(VERSION 3.0)
project(toolsbox VERSION 0.0.1)

set(PROJECT_DESCRIPTION "C++ Tools Box Header Library (c++11/14)")

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb -std=c++1y -march=native -pthread")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(NOT ENABLE_PERF_APPL_BUILD)
  SET(ENABLE_PERF_APPL_BUILD OFF CACHE BOOL "Enable perf application build" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING  "Choose the type of build, options are: Debug Release" FORCE)
endif()

if(NOT DISABLE_UNITTEST)
  set(DISABLE_UNITTEST OFF CACHE BOOL "Disable unittest" FORCE)
endif()

enable_testing()

#
# Boost
include(FindBoost)
find_package(
  Boost
  1.50.0
  COMPONENTS unit_test_framework
)
if (Boost_FOUND)
  include_directories(
    ${Boost_INCLUDE_DIR}
  )
endif (Boost_FOUND)

include_directories(
    include
)

#
# doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
  )
  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE}
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif(DOXYGEN_FOUND)

#
# subdir
add_subdirectory(tests)

#
# pc file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/toolsbox.pc.in ${CMAKE_CURRENT_BINARY_DIR}/toolsbox.pc @ONLY)


#
# install
install(DIRECTORY include/ DESTINATION include)
install(FILES "${CMAKE_BINARY_DIR}/toolsbox.pc" DESTINATION lib/pkgconfig)
