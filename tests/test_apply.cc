#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/apply.hpp>

#include <string>

namespace
{
  int counter = 0;

  void fct1()
  {
    ++counter;
  }

  void fct2(int i)
  {
    counter += i;
  }

  int fct3(int i)
  {
    return i;
  }

  void fct4(const std::string& y, std::string& r)
  {
    r = y;
  }
}

BOOST_AUTO_TEST_CASE( apply_01 )
{
  counter = 0;
  toolsbox::apply(fct1, std::make_tuple());
  BOOST_CHECK_EQUAL(counter, 1);
}

BOOST_AUTO_TEST_CASE( apply_02 )
{
  counter = 0;
  toolsbox::apply(fct2, std::make_tuple(5));
  BOOST_CHECK_EQUAL(counter, 5);
}

BOOST_AUTO_TEST_CASE( apply_03 )
{
  int ret = toolsbox::apply(fct3, std::make_tuple(5));
  BOOST_CHECK_EQUAL(ret, 5);
}

BOOST_AUTO_TEST_CASE( apply_04 )
{
  auto tuple = std::make_tuple(5);
  int ret = toolsbox::apply(fct3, tuple);
  BOOST_CHECK_EQUAL(ret, 5);
}

BOOST_AUTO_TEST_CASE( apply_05 )
{
  const auto tuple = std::make_tuple(5);
  int ret = toolsbox::apply(fct3, tuple);
  BOOST_CHECK_EQUAL(ret, 5);
}

BOOST_AUTO_TEST_CASE( apply_06 )
{
  std::string ret;
  toolsbox::apply(fct4, std::make_tuple("e", std::ref(ret)));
  BOOST_CHECK_EQUAL(ret, "e");
}

BOOST_AUTO_TEST_CASE( apply_07 )
{
  std::string oo = "rr";
  std::string ret;
  toolsbox::apply(fct4, std::make_tuple(std::cref(oo), std::ref(ret)));
  BOOST_CHECK_EQUAL(ret, oo);
  BOOST_CHECK_EQUAL(oo,"rr");
}

BOOST_AUTO_TEST_CASE( apply_08 )
{
  std::string oo = "rr";
  std::string ret;
  std::function<void (const std::string&, std::string&)> fct = [](const std::string&y, std::string&r) { r = y; };
  toolsbox::apply(fct, std::make_tuple(std::cref(oo), std::ref(ret)));
  BOOST_CHECK_EQUAL(ret, oo);
  BOOST_CHECK_EQUAL(oo,"rr");
}


