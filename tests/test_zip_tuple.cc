#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/zip_tuple.hpp>
#include <toolsbox/mpl/print_type.hpp>

BOOST_AUTO_TEST_CASE( zip_tuple_01 )
{
  auto tuple = toolsbox::make_zip_tuple(std::make_tuple(2, 4), std::make_tuple(0.5, 9.6));
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<decltype(tuple)>::name(), "tuple(tuple(int, double), tuple(int, double))");
  BOOST_CHECK_EQUAL(std::get<0>(std::get<0>(tuple)), 2);
  BOOST_CHECK_EQUAL(std::get<1>(std::get<0>(tuple)), 0.5);
  BOOST_CHECK_EQUAL(std::get<0>(std::get<1>(tuple)), 4);
  BOOST_CHECK_EQUAL(std::get<1>(std::get<1>(tuple)), 9.6);
}

BOOST_AUTO_TEST_CASE( zip_tuple_02 )
{
  auto tuple = toolsbox::make_zip_tuple(std::tuple<int, double>(), std::tuple<std::string, char>(), std::tuple<bool, char>());
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<decltype(tuple)>::name(), "tuple(tuple(int, string, bool), tuple(double, char, char))");
}

BOOST_AUTO_TEST_CASE( zip_tuple_03 )
{
  std::tuple<int, double>       t1;
  std::tuple<std::string, char> t2;
  std::tuple<bool, char>        t3;
  auto tuple = toolsbox::make_zip_tuple(t1, t2, t3);
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<decltype(tuple)>::name(), "tuple(tuple(int, string, bool), tuple(double, char, char))");
}

