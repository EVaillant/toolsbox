#include <toolsbox/lockfree/fix_size_queue.hpp>
#include <toolsbox/lockfree/stack.hpp>

#include <boost/lockfree/queue.hpp>
#include <boost/lockfree/stack.hpp>

#include "ref_lockfree.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <cassert>

#include <boost/range/irange.hpp>

template <size_t S> struct capacity
{
  static const size_t size = S;
};

template<class T, class S> using fix_size_queue     = toolsbox::lockfree::fix_size_queue<T, S::size>;
template<class T, class S> using ref_fix_size_queue = ref::fix_size_queue<T, S::size>;
template<class T, class S> using boost_queue        = boost::lockfree::queue<T, boost::lockfree::capacity<S::size>>;
template<class T, class S> using boost_stack        = boost::lockfree::stack<T, boost::lockfree::capacity<S::size>>;

template <size_t S> struct Data
{
  Data() {}
  Data(uint64_t) {}
  std::array<char, S> data;
};

template<class Queue, class Data> void task_producer(Queue& queue, uint64_t nb)
{

  for(uint64_t i = 1 ; i <= nb ; ++i)
  {
    while(!queue.push(Data(i))) ;
  }
}

template<class Queue, class Data> void task_consumer(Queue& queue, std::atomic_bool &stop, uint64_t& nb)
{
  Data v;
  nb = 0;
  for(;;)
  {
    while(!queue.pop(v))
    {
      if(stop)
      {
        return;
      }
    }
    ++nb;
  }
}

template<class Queue, class Data> void run_test_case_01(const std::string& name, int nb_producer, int nb_consumer, uint64_t nb)
{
  std::stringstream stream;
  stream << name << ";" << nb_producer << ";" << nb_consumer << ";" << nb << ";" << sizeof(Data);

  Queue queue;
  std::atomic_bool  stop(false);
  std::vector<std::thread> producers; producers.reserve(nb_producer);
  std::vector<std::thread> consumers; consumers.reserve(nb_consumer);
  std::vector<uint64_t>    results;   results.reserve(nb_consumer);

  auto start = std::chrono::high_resolution_clock::now();
  for(int i = 0 ; i < nb_consumer ; ++i)
  {
    results.push_back(0);
    consumers.push_back(std::thread(task_consumer<Queue, Data>, std::ref(queue), std::ref(stop), std::ref(results[i])));
  }

  for(int i = 0 ; i < nb_producer ; ++i)
  {
    producers.push_back(std::thread(task_producer<Queue, Data>, std::ref(queue), nb));
  }

  std::for_each(producers.begin(), producers.end(), [](std::thread &t){ t.join(); });
  stop = true;
  std::for_each(consumers.begin(), consumers.end(), [](std::thread &t){ t.join(); });

  auto end = std::chrono::high_resolution_clock::now();

  if(! queue.empty())
  {
    stream << ";0;queue not empty";
  }
  else
  {
    uint64_t sum = std::accumulate(results.begin(), results.end(), 0);
    if(sum != nb*nb_producer)
    {
      stream << ";0; result is " << sum << " and expected value is " <<  nb*nb_producer;
    }
    else
    {
      stream << ";1;";
    }
  }
  stream << ";" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << stream.str() << std::endl;
}

template <template <class ...> class Queue, class Data, class ... Extras> void test_case_01_by_algo_and_data(const std::string &name)
{
  for(int i : boost::irange(1, 11))
  {
    for(int j : boost::irange(1, 11))
    {
      for(int k : boost::irange(0, 10))
      {
        (void)k;
        run_test_case_01<Queue<Data, Extras...>, Data>(name, i, j, 100000);
      }
    }
  }
}

template <template <class...> class Queue, class ... Extras> void test_case_01_by_algo(const std::string &name)
{
  test_case_01_by_algo_and_data<Queue, Data<8>,   Extras...>(name);
  test_case_01_by_algo_and_data<Queue, Data<64>,  Extras...>(name);
  test_case_01_by_algo_and_data<Queue, Data<128>, Extras...>(name);
  test_case_01_by_algo_and_data<Queue, Data<256>, Extras...>(name);
  test_case_01_by_algo_and_data<Queue, Data<512>, Extras...>(name);
}

template <template <class...> class Queue> void test_case_01_by_algo_fix_size(const std::string &name)
{
  test_case_01_by_algo<Queue, capacity<8>>  (name + "_008");
  test_case_01_by_algo<Queue, capacity<64>> (name + "_064");
  test_case_01_by_algo<Queue, capacity<128>>(name + "_128");
  test_case_01_by_algo<Queue, capacity<256>>(name + "_256");
}

void test_case_01_fix_size_queue()
{
  test_case_01_by_algo_fix_size<fix_size_queue>("test1;lockfree_fix_size_queue");
  test_case_01_by_algo_fix_size<ref_fix_size_queue>("test1;ref_fix_size_queue");
  test_case_01_by_algo_fix_size<boost_queue>("test1;boost_queue_lockfree");
}

void test_case_01_stack()
{
  test_case_01_by_algo<ref::stack>("test1;ref_stack");
  test_case_01_by_algo<toolsbox::lockfree::stack>("test1;lockfree_stack");
  test_case_01_by_algo_fix_size<boost_stack>("test1;boost_stack_lockfree");
}

void test_case_01_queue()
{
  test_case_01_by_algo<ref::queue>("test1;ref_queue");
}

void test_case_01()
{
  std::cout << "test_case;algo_name;nb_producer;nb_consumer;element_by_producer;sizeof(element);status;reason if failed;duration(ms)" << std::endl;
  test_case_01_fix_size_queue();
  test_case_01_stack();
  test_case_01_queue();
}

int main()
{
  test_case_01();
  return 0;
}
