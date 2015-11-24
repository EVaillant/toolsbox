#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/any.hpp>

#include <string>

BOOST_TEST_DONT_PRINT_LOG_VALUE(nullptr_t)

BOOST_AUTO_TEST_CASE( any_01 )
{
  toolsbox::any a;
  BOOST_CHECK(a.empty());
  BOOST_CHECK(!a.is<int>());
  BOOST_CHECK(!a.is<std::string>());
}

BOOST_AUTO_TEST_CASE( any_02 )
{
  toolsbox::any a(6);
  toolsbox::any b(6);
  toolsbox::any c(7);
  toolsbox::any d(std::string("eee"));

  BOOST_CHECK(!a.empty());
  BOOST_CHECK(!b.empty());
  BOOST_CHECK(!c.empty());
  BOOST_CHECK(!d.empty());
  BOOST_CHECK( a.is<int>());
  BOOST_CHECK(!a.is<std::string>());
  BOOST_CHECK( a.as<int>() == 6);
  BOOST_CHECK( b.is<int>());
  BOOST_CHECK(!b.is<std::string>());
  BOOST_CHECK( b.as<int>() == 6);
  BOOST_CHECK( c.is<int>());
  BOOST_CHECK(!c.is<std::string>());
  BOOST_CHECK( c.as<int>() == 7);
  BOOST_CHECK(!d.is<int>());
  BOOST_CHECK( d.is<std::string>());
  BOOST_CHECK( d.as<std::string>() == "eee");

  BOOST_CHECK( a == 6);
  BOOST_CHECK(! (a != 6));
  BOOST_CHECK( a == b);
  BOOST_CHECK(! (a != b));
  BOOST_CHECK( a <= 6);
  BOOST_CHECK(! (a > 6));
  BOOST_CHECK( a <  7);
  BOOST_CHECK(! (a >=  7));
  BOOST_CHECK( a <= 7);
  BOOST_CHECK(! (a > 7));
  BOOST_CHECK( a <= c);
  BOOST_CHECK(! (a > c));
  BOOST_CHECK( a <  c);
  BOOST_CHECK(! (a >=  c));
  BOOST_CHECK( a != c);
  BOOST_CHECK(! (a == c));
  BOOST_CHECK( a <  d);
  BOOST_CHECK(! (a >=  d));
  BOOST_CHECK( a != d);
  BOOST_CHECK(! (a == d));
  BOOST_CHECK( a <= d);
  BOOST_CHECK(! (a > d));
}

BOOST_AUTO_TEST_CASE( any_03 )
{
  toolsbox::any v1 (5);
  toolsbox::any v2 (std::string("rrr"));
  toolsbox::any v3;

  BOOST_CHECK( v3 <  v1 );
  BOOST_CHECK( v3 <  v2 );
  BOOST_CHECK( v3 <= v1 );
  BOOST_CHECK( v3 <= v2 );
  BOOST_CHECK( v3 <   1 );
  BOOST_CHECK( v3 <=  1 );
  BOOST_CHECK( 1  >  v3 );
  BOOST_CHECK( 1  >= v3 );


  BOOST_CHECK( v1 >  v3 );
  BOOST_CHECK( v2 >  v3 );
  BOOST_CHECK( v1 >= v3 );
  BOOST_CHECK( v2 >= v3 );
}

BOOST_AUTO_TEST_CASE( any_04 )
{
  toolsbox::any v1 (5);
  toolsbox::any v2 (std::string("rrr"));
  toolsbox::any v3;

  std::hash<toolsbox::any> any_hash;

  BOOST_CHECK( any_hash(v1) != 0 );
  BOOST_CHECK( any_hash(v2) != 0 );
  BOOST_CHECK( any_hash(v3) == 0 );
}

BOOST_AUTO_TEST_CASE( any_05 )
{
  toolsbox::any v (5);

  BOOST_CHECK_NE(    std::get<int>(v),         nullptr);
  BOOST_CHECK_EQUAL( std::get<std::string>(v), nullptr);
  BOOST_CHECK_EQUAL( *std::get<int>(v),        5);
}


