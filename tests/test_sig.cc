#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/sig/signal.hh>

namespace
{
  struct Pouet1
  {
      Pouet1()
        : passA_(0)
        , passB_(0)
      {
      }

      void methodA()
      {
        ++passA_;
      }

      void methodB()
      {
        ++passB_;
      }

      int passA_, passB_;
  };
}

BOOST_AUTO_TEST_CASE(signal_01)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::connection c1 = signal.connect([&p]()
  {
    p.methodA();
  });
  toolsbox::sig::connection c2 = signal.connect([&p]()
  {
    p.methodB();
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  c1.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 2);

  c1.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 3);

  c2.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 3);
}

BOOST_AUTO_TEST_CASE(signal_group_01)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::group_connection gc = signal.connect([&p]()
  {
    p.methodA();
  });
  gc += signal.connect([&p]()
  {
    p.methodB();
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  gc.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_group_02)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::group_connection gc;
  gc += signal.connect([&p]()
  {
    p.methodA();
  });
  gc += signal.connect([&p]()
  {
    p.methodB();
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  gc.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_group_03)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  {
    toolsbox::sig::group_connection gc(true);
    gc += signal.connect([&p]()
    {
      p.methodA();
    });
    gc += signal.connect([&p]()
    {
      p.methodB();
    });

    signal();
    BOOST_CHECK_EQUAL(p.passA_, 1);
    BOOST_CHECK_EQUAL(p.passB_, 1);
  }

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_group_04)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::group_connection gc1;
  toolsbox::sig::group_connection gc2 = signal.connect([&p]()
  {
    p.methodA();
  });
  toolsbox::sig::group_connection gc3 = signal.connect([&p]()
  {
    p.methodB();
  });

  gc1 += gc2;
  gc1 += gc3;

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  gc1.disconnect();

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_reentrant_01)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::connection c1 = signal.connect([&p]()
  {
    p.methodA();
  });
  toolsbox::sig::connection c2 = signal.connect([&p, &c1]()
  {
    p.methodB();
    c1.disconnect();
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 2);
}

BOOST_AUTO_TEST_CASE(signal_reentrant_02)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::connection c1;
  toolsbox::sig::connection c2;
  c1 = signal.connect([&p, &c1]()
  {
    p.methodA();
    c1.disconnect();
  });
  c2 = signal.connect([&p, &c2]()
  {
    p.methodB();
    c2.disconnect();
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_reentrant_03)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::connection c1;
  toolsbox::sig::connection c2;
  c1 = signal.connect([&p, &c1, &c2, &signal]()
  {
    p.methodA();
    if(!c2.valid())
    {
      c2 = signal.connect([&p, &c2]()
      {
        p.methodB();
      });
    }
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 0);

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 2);
  BOOST_CHECK_EQUAL(p.passB_, 1);
}

BOOST_AUTO_TEST_CASE(signal_reentrant_04)
{
  toolsbox::sig::signal<void ()> signal;

  Pouet1 p;
  toolsbox::sig::connection c1;
  toolsbox::sig::connection c2;
  c1 = signal.connect([&p, &c1, &c2, &signal]()
  {
    p.methodA();
    if(!c2.valid())
    {
      c2 = signal.connect([&p, &c2]()
      {
        p.methodB();
      });
      c2.disconnect();
    }
  });

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 1);
  BOOST_CHECK_EQUAL(p.passB_, 0);

  signal();
  BOOST_CHECK_EQUAL(p.passA_, 2);
  BOOST_CHECK_EQUAL(p.passB_, 0);
}

