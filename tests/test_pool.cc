#define BOOST_TEST_DYN_LINK
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <toolsbox/pool/object_pool.hpp>
#include <vector>
#include <set>


struct Data
{
  Data()
   : value(-1)
  {
  }

  Data(int v)
   : value(v)
  {
  }

  int value;
};

typedef boost::mpl::list<toolsbox::pool::tree_object_pool<Data>, toolsbox::pool::linear_object_pool<Data>> object_all_pool_type;
typedef boost::mpl::list<toolsbox::pool::tree_object_pool<Data>, toolsbox::pool::linear_object_pool<Data>> object_segment_pool_type;

template <class Pool> struct Items
{
  typedef typename Pool::ptr_type ptr_type;

  std::vector<ptr_type> items;
  std::set<void*>       ptr_items;

  bool append(ptr_type &&item)
  {
    bool unique = ptr_items.insert(item.get()).second;
    items.push_back(std::move(item));
    return unique;
  }

  void* erase(std::size_t id)
  {
    ptr_type& item = items[id];
    void*      ret = item.get();
    ptr_items.erase(ret);
    item.reset();
    return ret;
  }
};

BOOST_AUTO_TEST_CASE_TEMPLATE(object_pool_init_01, Pool, object_all_pool_type)
{
  Pool pool;

  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(-1, item->value);
  }

  {
    auto item = pool.acquire(5);
    BOOST_CHECK_EQUAL(5,  item->value);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(object_segment_pool_01, Pool, object_segment_pool_type)
{
  Pool        pool;
  Items<Pool> items;
  
  const std::size_t cap = pool.capacity();

  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(cap, pool.size());
  BOOST_CHECK(!pool.full());
  for(std::size_t i = 0; i < cap ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(0,   pool.size());
  BOOST_CHECK(pool.full());

  void* data = items.erase(5);

  BOOST_CHECK(nullptr != data);
  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(1,   pool.size());

  auto item = pool.acquire();
  BOOST_CHECK(data == item.get());  
}

BOOST_AUTO_TEST_CASE_TEMPLATE(object_segment_pool_02, Pool, object_segment_pool_type)
{
  Pool        pool;
  Items<Pool> items;

  const std::size_t cap = pool.capacity();

  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(cap, pool.size());
  BOOST_CHECK(!pool.full());
  for(std::size_t i = 0; i < cap ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(0,   pool.size());
  BOOST_CHECK(pool.full());

  for(std::size_t i = 0; i < 3 ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(2*cap, pool.capacity());
  BOOST_CHECK_EQUAL(cap-3, pool.size());
  BOOST_CHECK(!pool.full());

  void* data = items.erase(cap + 2);
  BOOST_CHECK(nullptr != data);
  BOOST_CHECK_EQUAL(2*cap, pool.capacity());
  BOOST_CHECK_EQUAL(cap-2, pool.size());

  auto item = pool.acquire();
  BOOST_CHECK(data == item.get());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(object_segment_pool_03, Pool, object_segment_pool_type)
{
  Pool        pool;
  Items<Pool> items;

  const std::size_t cap = pool.capacity();

  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(cap, pool.size());
  BOOST_CHECK(!pool.full());

  for(std::size_t i = 0; i < cap ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(cap, pool.capacity());
  BOOST_CHECK_EQUAL(0,   pool.size());
  BOOST_CHECK(pool.full());

  for(std::size_t i = 0; i < cap ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(2*cap, pool.capacity());
  BOOST_CHECK_EQUAL(0,     pool.size());
  BOOST_CHECK(pool.full());

  for(std::size_t i = 0; i < cap ; ++i)
  {
    auto item = pool.acquire();
    BOOST_CHECK_EQUAL(cap-i-1, pool.size());
    BOOST_CHECK(items.append(std::move(item)));
  }
  BOOST_CHECK_EQUAL(3*cap, pool.capacity());
  BOOST_CHECK_EQUAL(0,     pool.size());

  for(std::size_t i = 0 ; i < items.items.size() ; ++i)
  {
    items.erase(i);
    BOOST_CHECK_EQUAL(i+1,   pool.size());
    BOOST_CHECK_EQUAL(3*cap, pool.capacity());
  }
}
