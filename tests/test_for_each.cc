#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/for_each.hpp>

#include <string>

BOOST_AUTO_TEST_CASE( for_each_01 )
{
  struct test01
  {
    void operator()(int a)
    {
      value += a;
    }

    int value = 0;
  };

  toolsbox::for_each(test01(), 5, 6);

  test01 t;
  toolsbox::for_each(t, 5, 6);
  BOOST_CHECK_EQUAL(t.value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_02 )
{
  int value = 0;
  toolsbox::for_each([&value](int a) { value += a; }, 5, 6);
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_03 )
{
  int value = 0;
  toolsbox::for_each([&value](int a) -> bool { value += a; return false; }, 5, 6);
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_04 )
{
  int value = 0;
  int       input1 = 5;
  const int input2 = 6;
  toolsbox::for_each([&value](int a) -> bool { value += a; return false; }, input1, input2);
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_by_tuple_01 )
{
  int value = 0;
  toolsbox::for_each_by_tuple([&value](int a) -> bool { value += a; return false; }, std::make_tuple(5, 6));
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_by_tuple_02 )
{
  int  value = 0;
  auto tuple = std::make_tuple(5, 6);
  toolsbox::for_each_by_tuple([&value](int a) -> bool { value += a; return false; }, tuple);
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_by_tuple_03 )
{
  int  value = 0;
  auto tuple = std::make_tuple(5, 6);
  std::function<bool (int)> fct = [&value](int a) -> bool { value += a; return false; };
  toolsbox::for_each_by_tuple(fct, tuple);
  BOOST_CHECK_EQUAL(value, 11);
}

BOOST_AUTO_TEST_CASE( for_each_by_tuple_04 )
{
  int  value = 0;
  const auto tuple = std::make_tuple(5, 6);
  toolsbox::for_each_by_tuple([&value](int a) -> bool { value += a; return false; }, tuple);
  BOOST_CHECK_EQUAL(value, 11);
}


