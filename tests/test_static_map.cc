#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/mpl/static_map.hpp>

BOOST_AUTO_TEST_CASE( static_map_01 )
{
  toolsbox::mpl::static_map<std::pair<int, int>, std::pair<std::string, std::string>> v;
  v.as<int>() = 5;
  std::get<std::string>(v) = "ff";

  BOOST_CHECK_EQUAL(v.as<std::string>() , "ff");
  BOOST_CHECK_EQUAL(std::get<int>(v),      5);
}

BOOST_AUTO_TEST_CASE( static_map_02 )
{
  toolsbox::mpl::static_map<std::pair<int, int>, std::pair<std::string, std::string>> v(5, "ff");
  BOOST_CHECK_EQUAL(v.as<std::string>() , "ff");
  BOOST_CHECK_EQUAL(std::get<int>(v),      5);
}

