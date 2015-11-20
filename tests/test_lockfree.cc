#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <toolsbox/lockfree/fix_size_queue.hpp>
#include <toolsbox/lockfree/stack.hpp>

#include "ref_lockfree.hpp"

typedef boost::mpl::list<ref::stack<int>, toolsbox::lockfree::stack<int>> stack_type;
typedef boost::mpl::list<ref::fix_size_queue<int, 8>, toolsbox::lockfree::fix_size_queue<int, 8>> fix_size_queue_type;

BOOST_AUTO_TEST_CASE_TEMPLATE( lockfree_stack_01, Stack, stack_type)
{
  Stack stack;

  BOOST_CHECK( stack.empty() );
  BOOST_CHECK( stack.push(1) );
  BOOST_CHECK(!stack.empty() );
  BOOST_CHECK( stack.push(2) );

  int val;
  BOOST_CHECK( stack.pop(val) );
  BOOST_CHECK_EQUAL(val, 2);
  BOOST_CHECK( stack.pop(val) );
  BOOST_CHECK_EQUAL(val, 1);
  BOOST_CHECK(!stack.pop(val) );
  BOOST_CHECK( stack.empty() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( lockfree_fix_size_queue_01, Queue, fix_size_queue_type)
{
  Queue queue;

  BOOST_CHECK( queue.empty() );
  for(int i = 1 ; i <= 8 ; ++i)
  {
    BOOST_CHECK( queue.push(i) );
    BOOST_CHECK(!queue.empty() );
  }

  BOOST_CHECK(!queue.push(15) );
  BOOST_CHECK(!queue.empty() );

  int val;
  for(int i = 1 ; i <= 8 ; ++i)
  {
    BOOST_CHECK( queue.pop(val) );
    BOOST_CHECK_EQUAL(val, i );
  }
}
