#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/any.hpp>
#include <string>

BOOST_AUTO_TEST_CASE( any_01 )
{
  toolsbox::any a;

  BOOST_CHECK(a.empty());
  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());

  BOOST_CHECK(std::to_string(a).empty());
}

BOOST_AUTO_TEST_CASE( any_02 )
{
  int v = 5;

  toolsbox::any        a(v);
  const toolsbox::any& b = a;

  BOOST_CHECK(a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<int>());

  BOOST_CHECK(a.as<int>() == v);
  BOOST_CHECK(b.as<int>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));
}

BOOST_AUTO_TEST_CASE( any_03 )
{
  int o = 5;
  int *v = &o;

  toolsbox::any        a(v);
  const toolsbox::any& b = a;

  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<int*>());

  BOOST_CHECK(a.as<int*>() == v);
  BOOST_CHECK(b.as<int*>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));
}

BOOST_AUTO_TEST_CASE( any_04 )
{
  int o = 5;
  const int *v = &o;

  toolsbox::any        a(v);
  const toolsbox::any& b = a;

  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<const int*>());

  BOOST_CHECK(a.as<const int*>() == v);
  BOOST_CHECK(b.as<const int*>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));
}

BOOST_AUTO_TEST_CASE( any_05 )
{
  int v = 5;
  toolsbox::any        a(std::ref(v));
  const toolsbox::any& b = a;

  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<int&>());

  BOOST_CHECK(a.as<int&>() == v);
  BOOST_CHECK(b.as<int&>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));

  ++v;

  BOOST_CHECK(a.as<int&>() == v);
  BOOST_CHECK(b.as<int&>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));
}

BOOST_AUTO_TEST_CASE( any_06 )
{
  int v = 5;
  toolsbox::any a(std::cref(v));
  const toolsbox::any& b = a;
  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<const int&>());

  BOOST_CHECK(a.as<const int&>() == v);
  BOOST_CHECK(b.as<const int&>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));

  ++v;

  BOOST_CHECK(a.as<const int&>() == v);
  BOOST_CHECK(b.as<const int&>() == v);

  BOOST_CHECK(std::to_string(a) == std::to_string(v));
  BOOST_CHECK(std::to_string(b) == std::to_string(v));
}

BOOST_AUTO_TEST_CASE( any_07 )
{
  toolsbox::any b;
  toolsbox::any a(b);
  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());

  BOOST_CHECK(std::to_string(a).empty());
  BOOST_CHECK(std::to_string(b).empty());
}

BOOST_AUTO_TEST_CASE( any_08 )
{
  toolsbox::any        b(5);
  toolsbox::any        a(b);
  const toolsbox::any& c = a;

  BOOST_CHECK(a.is<int>());
  BOOST_CHECK(!a.is<const int>());
  BOOST_CHECK(!a.is<int*>());
  BOOST_CHECK(!a.is<const int*>());
  BOOST_CHECK(!a.is<int&>());
  BOOST_CHECK(!a.is<const int&>());
  BOOST_CHECK(a.get_id() == toolsbox::type_uid::get<int>());

  BOOST_CHECK(a.as<int>() == 5);
  BOOST_CHECK(c.as<int>() == 5);

  BOOST_CHECK(std::to_string(a) == "5");
  BOOST_CHECK(std::to_string(c) == "5");
}

BOOST_AUTO_TEST_CASE( any_09 )
{
  const std::string omsg = "popo";
  std::string        msg = omsg;
  toolsbox::any        a = msg;

  BOOST_CHECK(msg == omsg);

  a = msg;

  BOOST_CHECK(msg == omsg);
}

