#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/mpl/print_type.hpp>

BOOST_AUTO_TEST_CASE(print_type_01)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<int>::name(),         "int");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<double>::name(),      "double");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<std::string>::name(), "string");
}

BOOST_AUTO_TEST_CASE(print_type_02)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<int&>::name(),         "int&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<double&>::name(),      "double&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<std::string&>::name(), "string&");
}

BOOST_AUTO_TEST_CASE(print_type_03)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const int&>::name(),         "const int&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const double&>::name(),      "const double&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const std::string&>::name(), "const string&");
}

BOOST_AUTO_TEST_CASE(print_type_04)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<int*>::name(),         "int*");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<double*>::name(),      "double*");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<std::string*>::name(), "string*");
}

BOOST_AUTO_TEST_CASE(print_type_05)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const int*>::name(),         "const int*");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const double*>::name(),      "const double*");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const std::string*>::name(), "const string*");
}

BOOST_AUTO_TEST_CASE(print_type_06)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<int&&>::name(),         "int&&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<double&&>::name(),      "double&&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<std::string&&>::name(), "string&&");
}

BOOST_AUTO_TEST_CASE(print_type_07)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const int&&>::name(),         "const int&&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const double&&>::name(),      "const double&&");
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<const std::string&&>::name(), "const string&&");
}

BOOST_AUTO_TEST_CASE(print_type_08)
{
  BOOST_CHECK_EQUAL(toolsbox::mpl::print_type<std::list<int>>::name(),                         "list(int)");
  BOOST_CHECK_EQUAL((toolsbox::mpl::print_type<std::map<std::string, int>>::name()),           "map(string, int)");
  BOOST_CHECK_EQUAL((toolsbox::mpl::print_type<std::tuple<std::string, int, double>>::name()), "tuple(string, int, double)");
}


