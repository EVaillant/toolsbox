#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/variant.hpp>

#include <string>
#include <sstream>

typedef toolsbox::variant<int, std::string> my_variant;
BOOST_TEST_DONT_PRINT_LOG_VALUE(my_variant)

BOOST_AUTO_TEST_CASE( variant_01 )
{
  my_variant v(5);
  BOOST_CHECK(  v.is<int>() );
  BOOST_CHECK( !v.is<std::string>() );
  BOOST_CHECK( !v.is<double>() );
  BOOST_CHECK( !v.empty() );
  BOOST_CHECK_EQUAL( v.as<int>(), 5 );
  v.as<int>() = 8;
  BOOST_CHECK_EQUAL( v.as<int>(), 8 );
  std::stringstream stream;
  stream << v;
  BOOST_CHECK_EQUAL(stream.str(), "8");
  v.reset();
  BOOST_CHECK( v.empty() );
}

BOOST_AUTO_TEST_CASE( variant_02 )
{
  static const std::string str1 = "eeeee";
  static const std::string str2 = "aaaaa";
  my_variant v(str1);
  BOOST_CHECK( !v.is<int>() );
  BOOST_CHECK(  v.is<std::string>() );
  BOOST_CHECK( !v.is<double>() );
  BOOST_CHECK( !v.empty() );
  BOOST_CHECK_EQUAL(  v.as<std::string>(), str1 );
  v.as<std::string>() = str2;
  BOOST_CHECK_EQUAL(  v.as<std::string>(), str2 );
  std::stringstream stream;
  stream << v;
  BOOST_CHECK_EQUAL(stream.str(), str2);
  v.reset();
  BOOST_CHECK( v.empty() );
}

BOOST_AUTO_TEST_CASE( variant_03 )
{
  my_variant v;
  std::stringstream stream;
  stream << v;
  BOOST_CHECK_EQUAL(stream.str(), "");
}

BOOST_AUTO_TEST_CASE( variant_04 )
{
  my_variant v1;
  my_variant v2;

  v1.emplace<int>(5);
  BOOST_CHECK(  v1.is<int>() );
  BOOST_CHECK( !v1.empty() );

  v2 = v1;

  BOOST_CHECK(  v2.is<int>() );
  BOOST_CHECK( !v2.empty() );
  BOOST_CHECK(  v1.is<int>() );
  BOOST_CHECK( !v1.empty() );
}

BOOST_AUTO_TEST_CASE( variant_05 )
{
  my_variant v1;
  my_variant v2;

  v1.emplace<int>(5);
  BOOST_CHECK(  v1.is<int>() );
  BOOST_CHECK( !v1.empty() );

  v2 = std::move(v1);

  BOOST_CHECK(  v2.is<int>() );
  BOOST_CHECK( !v2.empty() );
  BOOST_CHECK( !v1.is<int>() );
  BOOST_CHECK(  v1.empty() );
}

BOOST_AUTO_TEST_CASE( variant_06 )
{
  my_variant v1 (5);
  my_variant v2 (std::move(v1));

  BOOST_CHECK(  v2.is<int>() );
  BOOST_CHECK( !v2.empty() );
  BOOST_CHECK( !v1.is<int>() );
  BOOST_CHECK(  v1.empty() );
}

BOOST_AUTO_TEST_CASE( variant_07 )
{
  my_variant v1 (5);
  my_variant v2 (8);
  my_variant v3 (std::string("rrr"));
  my_variant v4 (5);

  BOOST_CHECK(!( v1 >  v1 ));
  BOOST_CHECK(!( v1 <  v1 ));

  BOOST_CHECK_EQUAL( v1, 5 );
  BOOST_CHECK_NE(    v1, v2 );
  BOOST_CHECK_LT(    v1, v2 );
  BOOST_CHECK_GT(    v2, v1 );
  BOOST_CHECK_LE(    v1, v2 );
  BOOST_CHECK_GE(    v2, v1 );
  BOOST_CHECK_EQUAL( v1, v4 );
  BOOST_CHECK_LE(    v1, v4 );
  BOOST_CHECK_GE(    v1, v4 );

  BOOST_CHECK_LT(    v1, v3  );
  BOOST_CHECK_LT(    v2, v3  );
  BOOST_CHECK_LT(    v4, v3  );
  BOOST_CHECK_GT(    v3, v1  );
  BOOST_CHECK_GT(    v3, v2  );
  BOOST_CHECK_GT(    v3, v4  );
}


BOOST_AUTO_TEST_CASE( variant_08 )
{
  my_variant v1;
  my_variant v2;

  v1 = 5;
  v2 = std::string("rrr");

  BOOST_CHECK( v1.is<int>() );
  BOOST_CHECK_EQUAL( v1.as<int>(), 5 );
  BOOST_CHECK( v2.is<std::string>() );
  BOOST_CHECK_EQUAL( v2.as<std::string>(), "rrr" );
}

BOOST_AUTO_TEST_CASE( variant_09 )
{
  my_variant v1;
  my_variant v2;

  int vi = 5;
  std::string vs = "rrr";

  v1 = vi;
  v2 = vs;

  BOOST_CHECK( v1.is<int>() );
  BOOST_CHECK_EQUAL( v1.as<int>(), vi );
  BOOST_CHECK( v2.is<std::string>() );
  BOOST_CHECK_EQUAL( v2.as<std::string>(), vs );
}

