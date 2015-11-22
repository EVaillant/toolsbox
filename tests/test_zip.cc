#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/zip.hpp>

#include <string>
#include <list>
#include <vector>
#include <sstream>

namespace
{
  class Data
  {
    public:
      static int default_ctor;
      static int copy_ctor;
      static int move_ctor;
      static int default_dtor;

      Data()
      {
        ++default_ctor;
      }

      Data(const Data&)
      {
        ++copy_ctor;
      }

      Data(Data&&)
      {
        ++move_ctor;
      }

      ~Data()
      {
        ++default_dtor;
      }
  };

  int Data::default_ctor = 0;
  int Data::copy_ctor = 0;
  int Data::move_ctor = 0;
  int Data::default_dtor = 0;

  void reset_couner()
  {
    Data::default_ctor = 0;
    Data::copy_ctor = 0;
    Data::move_ctor = 0;
    Data::default_dtor = 0;
  }
}

BOOST_AUTO_TEST_CASE( zip_01 )
{
  std::list<int> oo = {1, 2, 3};
  std::vector<std::string> pp = {"e", "r", "u"};

  std::ostringstream stream;

  for(auto &i : toolsbox::make_zip(oo, pp))
  {
    int &b = std::get<0>(i);
    std::string &h = std::get<1>(i);
    stream << b << " " << h << std::endl;
  }

  BOOST_CHECK_EQUAL(stream.str(), "1 e\n2 r\n3 u\n");
}

BOOST_AUTO_TEST_CASE( zip_02 )
{
  std::list<int> oo = {1, 2};
  std::vector<std::string> pp = {"e", "r", "u"};

  std::ostringstream stream;

  for(auto &i : toolsbox::make_zip(oo, pp))
  {
    int &b = std::get<0>(i);
    std::string &h = std::get<1>(i);
    stream << b << " " << h << std::endl;
  }

  BOOST_CHECK_EQUAL(stream.str(), "1 e\n2 r\n");
}

BOOST_AUTO_TEST_CASE( zip_03 )
{
  reset_couner();

  {
    std::list<int> oo = {1, 2, 3};
    std::vector<std::string> pp = {"e", "r", "u"};
    std::vector<Data> ii;
    ii.resize(3);

    BOOST_CHECK_EQUAL(3, Data::default_ctor);
    BOOST_CHECK_EQUAL(0, Data::copy_ctor);
    BOOST_CHECK_EQUAL(0, Data::move_ctor);
    BOOST_CHECK_EQUAL(0, Data::default_dtor);

    for(auto &i : toolsbox::make_zip(oo, pp, std::move(ii)))
    {
      std::get<0>(i);
      std::get<1>(i);
      std::get<2>(i);
    }

    BOOST_CHECK_EQUAL(3, Data::default_ctor);
    BOOST_CHECK_EQUAL(0, Data::copy_ctor);
    BOOST_CHECK_EQUAL(0, Data::move_ctor);
    BOOST_CHECK_EQUAL(3, Data::default_dtor);
  }

  BOOST_CHECK_EQUAL(3, Data::default_ctor);
  BOOST_CHECK_EQUAL(0, Data::copy_ctor);
  BOOST_CHECK_EQUAL(0, Data::move_ctor);
  BOOST_CHECK_EQUAL(3, Data::default_dtor);
}

BOOST_AUTO_TEST_CASE( zip_04 )
{
  reset_couner();

  {
    std::list<int> oo = {1, 2, 3};
    std::vector<std::string> pp = {"e", "r", "u"};
    std::vector<Data> ii;
    ii.resize(3);

    BOOST_CHECK_EQUAL(3, Data::default_ctor);
    BOOST_CHECK_EQUAL(0, Data::copy_ctor);
    BOOST_CHECK_EQUAL(0, Data::move_ctor);
    BOOST_CHECK_EQUAL(0, Data::default_dtor);

    for(auto &i : toolsbox::make_zip(oo, pp, ii))
    {
      std::get<0>(i);
      std::get<1>(i);
      std::get<2>(i);
    }

    BOOST_CHECK_EQUAL(3, Data::default_ctor);
    BOOST_CHECK_EQUAL(0, Data::copy_ctor);
    BOOST_CHECK_EQUAL(0, Data::move_ctor);
    BOOST_CHECK_EQUAL(0, Data::default_dtor);
  }

  BOOST_CHECK_EQUAL(3, Data::default_ctor);
  BOOST_CHECK_EQUAL(0, Data::copy_ctor);
  BOOST_CHECK_EQUAL(0, Data::move_ctor);
  BOOST_CHECK_EQUAL(3, Data::default_dtor);
}

BOOST_AUTO_TEST_CASE( zip_05 )
{
  const std::list<int> oo = {1, 2};
  std::vector<std::string> pp = {"e", "r", "u"};

  std::ostringstream stream;

  for(auto &i : toolsbox::make_zip(oo, pp))
  {
    const int &b = std::get<0>(i);
    std::string &h = std::get<1>(i);
    stream << b << " " << h << std::endl;
  }

  BOOST_CHECK_EQUAL(stream.str(), "1 e\n2 r\n");
}

BOOST_AUTO_TEST_CASE( zip_06 )
{
  std::list<int> oo = {1, 2};
  const std::vector<std::string> pp = {"e", "r", "u"};

  std::ostringstream stream;

  const auto zzz = toolsbox::make_zip(oo, pp);
  for(const auto &i : zzz)
  {
    const int &b = std::get<0>(i);
    const std::string &h = std::get<1>(i);
    stream << b << " " << h << std::endl;
  }

  BOOST_CHECK_EQUAL(stream.str(), "1 e\n2 r\n");
}
