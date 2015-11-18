#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/type_uid.hpp>

#include <string>

BOOST_AUTO_TEST_CASE( type_uid_01 )
{
  toolsbox::type_uid::id_type id_int    = toolsbox::type_uid::get<int>();
  toolsbox::type_uid::id_type id_string = toolsbox::type_uid::get<std::string>();

  BOOST_CHECK_EQUAL(id_int,    toolsbox::type_uid::get<int>());
  BOOST_CHECK_NE(id_int,       toolsbox::type_uid::get<std::string>());

  BOOST_CHECK_NE(id_string,    toolsbox::type_uid::get<int>());
  BOOST_CHECK_EQUAL(id_string, toolsbox::type_uid::get<std::string>());
}

