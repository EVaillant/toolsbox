#ifndef TOOLSBOX_POOL_THREAD_POOL_HPP
# define TOOLSBOX_POOL_THREAD_POOL_HPP

# include <toolsbox/apply.hpp>
# include <thread>
# include <vector>
# include <map>
# include <cassert>
# include <atomic>
# include <tuple>
# include <future>
# include <functional>
# include <algorithm>

namespace toolsbox
{
  namespace pool
  {
    template <class, template <class> class> class thread_pool;
    template <template <class> class Queue, class R, class ... Args> class thread_pool<R(Args...), Queue>
    {
      public:
        typedef thread_pool                                 self_type;
        typedef R                                           return_type;
        typedef std::function<R(Args...)>                   target_type;
        typedef std::function<return_type(target_type&)>    tuple_target_type;
        typedef std::map<std::size_t, target_type>          target_by_thread_type;
        typedef std::promise<return_type>                   promise_type;
        typedef std::future<return_type>                    future_type;
        typedef std::tuple<promise_type, tuple_target_type> queue_arg_type;
        typedef Queue<queue_arg_type>                       queue_type;

        thread_pool()
          : running_(false)
        {
        }

        ~thread_pool()
        {
          stop();
        }

        void bind_all(const target_type& target)
        {
          default_target_ = target;
        }

        void bind_by_thread(std::size_t id, const target_type& target)
        {
          target_by_thread_[id] = target;
        }

        void start(std::size_t nb)
        {
          bool status = running_.load();
          if(!status)
          {
            running_ = true;
            workers_.reserve(nb);
            for(std::size_t i = 0 ; i < nb ; ++i)
            {
              target_type& target = get_target_(i);
              assert(target);
              workers_.push_back(std::thread(std::bind(&self_type::worker_, this, std::ref(target))));
            }
          }
        }

        void stop()
        {
          bool status = running_.load();
          if(status)
          {
            running_ = false;
            std::for_each(workers_.begin(), workers_.end(), [](std::thread &t){ t.join(); });
          }
        }

        template <class ... LArgs> future_type push(LArgs && ... args)
        {
          auto          args_tuple = std::make_tuple(std::forward<LArgs>(args)...);
          tuple_target_type target = std::bind(&self_type::apply_<decltype(args_tuple)>, this, std::placeholders::_1, std::move(args_tuple));
          queue_arg_type       arg = std::make_tuple(promise_type(), std::move(target));
          auto&            promise = std::get<0>(arg);
          auto              future = promise.get_future();
          while(!queue_.push(std::move(arg)))
          {
            little_sleep_();
          }
          return future;
        }

      protected:
        target_type& get_target_(std::size_t id)
        {
          typename target_by_thread_type::iterator it = target_by_thread_.find(id);
          return (it != target_by_thread_.end() ? it->second : default_target_);
        }

        void worker_(target_type& target)
        {
          queue_arg_type data;
          while(running_ == true)
          {
            while(queue_.pop(data))
            {
              auto& promise  = std::get<0>(data);
              auto& function = std::get<1>(data);
              invoke_(promise, function, target);
            }
            little_sleep_();
          }
        }

        template <class Tuple> return_type apply_(target_type& target, Tuple& arg)
        {
          return apply(target, arg);
        }

        void invoke_(std::promise<void>& promise, tuple_target_type &function, target_type& target)
        {
          function(target);
          promise.set_value();
        }

        template <class T> void invoke_(std::promise<T>& promise, tuple_target_type &function, target_type& target)
        {
          return_type ret = function(target);
          promise.set_value(std::move(ret));
        }

        void little_sleep_()
        {
          std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }

      private:
        queue_type               queue_;
        target_type              default_target_;
        target_by_thread_type    target_by_thread_;
        std::vector<std::thread> workers_;
        std::atomic_bool         running_;
    };
  }
}

#endif

