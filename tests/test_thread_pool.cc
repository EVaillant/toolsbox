#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>

#include <toolsbox/pool/thread_pool.hpp>
#include <toolsbox/lockfree/fix_size_queue.hpp>

#include <string>

namespace
{
  template <class T> using queue = toolsbox::lockfree::fix_size_queue<T, 64>;

  int worker_1()
  {
    return 5;
  }

  int worker_2(int v)
  {
    return v;
  }

  int worker_3(const std::string& arg)
  {
    return arg.size();
  }

  void worker_4(std::string& arg)
  {
    arg = "ttt";
  }

  int worker_5(const std::string& arg1, std::string& arg2, int v)
  {
    BOOST_CHECK_EQUAL(arg1, arg2);
    return v;
  }
}

BOOST_AUTO_TEST_CASE(thread_pool_01)
{
  toolsbox::pool::thread_pool<int (), queue> pool;
  pool.bind_all(worker_1);
  pool.start(5);

  std::future<int> r = pool.push();
  r.wait();
  BOOST_CHECK_EQUAL(r.get(), 5);
}

BOOST_AUTO_TEST_CASE(thread_pool_02)
{
  toolsbox::pool::thread_pool<int (int), queue> pool;
  pool.bind_all(worker_2);
  pool.start(5);

  std::future<int> r1 = pool.push(5);
  std::future<int> r2 = pool.push(50);

  r1.wait();
  r2.wait();

  BOOST_CHECK_EQUAL(r1.get(), 5);
  BOOST_CHECK_EQUAL(r2.get(), 50);
}

BOOST_AUTO_TEST_CASE(thread_pool_03)
{
  toolsbox::pool::thread_pool<int (const std::string&), queue> pool;
  pool.bind_all(worker_3);
  pool.start(5);

  std::future<int> r1 = pool.push("eee");
  std::future<int> r2 = pool.push("tttt");

  r1.wait();
  r2.wait();

  BOOST_CHECK_EQUAL(r1.get(), 3);
  BOOST_CHECK_EQUAL(r2.get(), 4);
}

BOOST_AUTO_TEST_CASE(thread_pool_04)
{
  toolsbox::pool::thread_pool<int (const std::string&), queue> pool;
  pool.bind_all(worker_3);
  pool.start(5);

  std::string arg1 = "eee";
  std::string arg2 = "tttt";

  std::future<int> r1 = pool.push(std::cref(arg1));
  std::future<int> r2 = pool.push(std::cref(arg2));

  r1.wait();
  r2.wait();

  BOOST_CHECK_EQUAL(r1.get(), 3);
  BOOST_CHECK_EQUAL(r2.get(), 4);
}

BOOST_AUTO_TEST_CASE(thread_pool_05)
{
  toolsbox::pool::thread_pool<void (std::string&), queue> pool;
  pool.bind_all(worker_4);
  pool.start(5);

  std::string arg1, arg2;

  std::future<void> r1 = pool.push(std::ref(arg1));
  std::future<void> r2 = pool.push(std::ref(arg2));

  r1.wait();
  r2.wait();

  BOOST_CHECK_EQUAL(arg1, "ttt");
  BOOST_CHECK_EQUAL(arg2, "ttt");
}

BOOST_AUTO_TEST_CASE(thread_pool_06)
{
  toolsbox::pool::thread_pool<int (const std::string&, std::string&, int), queue> pool;
  pool.bind_all(worker_5);
  pool.start(5);

  std::string arg1 = "eee";
  std::string arg2 = arg1;

  std::future<int> r = pool.push(std::cref(arg1), std::ref(arg2), 5);
  r.wait();

  BOOST_CHECK_EQUAL(r.get(), 5);
}

