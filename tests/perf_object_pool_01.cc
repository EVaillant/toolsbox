# include <toolsbox/pool/object_pool.hpp>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <array>
#include <map>

#include "ref_lockfree.hpp"
#include "ref_object_pool.hpp"

class alloc_stat
{
  public:
    typedef std::chrono::high_resolution_clock::duration duration_type;

    alloc_stat()
      : min_(duration_type::max())
      , max_(duration_type::min())
      , accumulate_(duration_type::zero())
      , nb_(0)
    {
    }

    void append(const duration_type& d)
    {
      min_ = std::min(min_, d);
      max_ = std::max(max_, d);
      accumulate_ += d;
      ++nb_;

      auto it_value = values_.insert(std::make_pair(std::chrono::duration_cast<std::chrono::microseconds>(d).count(), 0)).first;
      ++ (it_value->second);
    }

    void merge(const alloc_stat &other)
    {
      min_ = std::min(min_, other.min_);
      max_ = std::max(max_, other.max_);
      accumulate_ += other.accumulate_;
      nb_ += other.nb_;
      for(const auto& value : other.values_)
      {
        auto it_value = values_.insert(std::make_pair(value.first, 0)).first;
        it_value->second += value.second;
      }
    }

    void dump(std::ostream& stream, bool value) const
    {
      stream << "min:" << std::chrono::duration_cast<std::chrono::microseconds>(min_).count();
      stream << " max:" << std::chrono::duration_cast<std::chrono::microseconds>(max_).count();
      if(nb_)
      {
        stream << " avg:" << std::chrono::duration_cast<std::chrono::microseconds>(accumulate_/nb_).count();
      }
      stream << " nb:" << nb_ << std::endl;
      if(value)
      {
        for(const auto& value : values_)
        {
          stream << " " << value.first << ";" << value.second << std::endl;
        }
      }
    }

  private:
    std::map<std::size_t, std::size_t> values_;
    duration_type min_;
    duration_type max_;
    duration_type accumulate_;
    std::size_t   nb_;
};

typedef std::array<char, 10*1024> data_type;

typedef ref::old_tree_object_pool<data_type>          old_tree_pool_type;
typedef toolsbox::pool::tree_object_pool<data_type>   tree_pool_type;
typedef toolsbox::pool::linear_object_pool<data_type> linear_pool_type;
typedef ref::mutex_object_pool<data_type>             mutex_pool_type;
typedef toolsbox::pool::none_object_pool<data_type>   ref_pool_type;

template <class Pool, class Queue> void task_producer(Pool& pool, Queue& queue, uint64_t nb, alloc_stat& stat)
{
  for(uint64_t i = 0; i < nb ; ++i)
  {
    auto start = std::chrono::high_resolution_clock::now();
    auto elt   = pool.acquire();
    auto end   = std::chrono::high_resolution_clock::now();
    stat.append(end-start);
    std::fill(elt->begin(), elt->end(), nb & 0xFF);
    queue.push(std::move(elt));
  }
}

template <class Queue> void task_consumer(Queue& queue, std::atomic_bool &stop, uint64_t& nb, alloc_stat& stat)
{
  nb = 0;
  for(;;)
  {
    typename Queue::value_type v;

    while(!queue.pop(v))
    {
      if(stop)
      {
        return;
      }
    }

    auto start = std::chrono::high_resolution_clock::now();
    v.reset();
    auto end   = std::chrono::high_resolution_clock::now();
    stat.append(end-start);

    ++nb;
  }
}

template <class Pool> void run_test_case(const std::string& name, int nb_producer, int nb_consumer, uint64_t nb)
{
  typedef typename Pool::ptr_type ptr_type;
  typedef ref::queue<ptr_type>    queue_type;

  Pool       pool;
  queue_type queue;

  std::string filename;
  {
    std::stringstream stream;
    stream << name << "_" << nb_producer << "_" << nb_consumer << "_" << nb << "_" << sizeof(typename queue_type::value_type::element_type) << ".out";
    filename = stream.str();
  }

  std::ofstream stream(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
  stream << name << ";" << nb_producer << ";" << nb_consumer << ";" << nb << ";" << sizeof(typename queue_type::value_type::element_type);

  std::atomic_bool  stop(false);
  std::vector<std::thread> producers;   producers.reserve(nb_producer);
  std::vector<alloc_stat>  alloc_stats; alloc_stats.resize(nb_producer);
  std::vector<alloc_stat>  free_stats;  free_stats.resize(nb_consumer);
  std::vector<std::thread> consumers;   consumers.reserve(nb_consumer);
  std::vector<uint64_t>    results;     results.reserve(nb_consumer);

  auto start = std::chrono::high_resolution_clock::now();
  for(int i = 0 ; i < nb_consumer ; ++i)
  {
    results.push_back(0);
    consumers.push_back(std::thread(task_consumer<queue_type>, std::ref(queue), std::ref(stop), std::ref(results[i]), std::ref(free_stats[i])));
  }

  for(int i = 0 ; i < nb_producer ; ++i)
  {
    producers.push_back(std::thread(task_producer<Pool, queue_type>, std::ref(pool), std::ref(queue), nb, std::ref(alloc_stats[i])));
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
  stream << ";" << pool.capacity() << std::endl;
  {
    stream << "alloc:" << std::endl;
    alloc_stat global_stat;
    for(alloc_stat& stat : alloc_stats)
    {
     global_stat.merge(stat);
    }
    global_stat.dump(stream, true);
    for(alloc_stat& stat : alloc_stats)
    {
      stream << " ";
      stat.dump(stream, false);
    }
  }
  {
    stream << "free:" << std::endl;
    alloc_stat global_stat;
    for(alloc_stat& stat : free_stats)
    {
     global_stat.merge(stat);
    }
    global_stat.dump(stream, true);
    for(alloc_stat& stat : free_stats)
    {
      stream << " ";
      stat.dump(stream, false);
    }
  }
}

template <class Pool> void run_test_case(const std::string& name)
{
  run_test_case<Pool> (name,  30,  1, 100000);
  run_test_case<Pool> (name,   1, 30, 100000);
  run_test_case<Pool> (name,  15, 15, 100000);
}

int main()
{
  run_test_case<old_tree_pool_type>("old_tree");
  run_test_case<tree_pool_type>    ("tree");
  run_test_case<linear_pool_type>  ("linear");
  run_test_case<mutex_pool_type>   ("mutex");
  run_test_case<ref_pool_type>     ("none");

  return 0;
}
