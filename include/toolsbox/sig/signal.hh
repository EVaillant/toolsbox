#ifndef SIG_SIGNAL_HH
# define SIG_SIGNAL_HH

# include <memory>
# include <functional>
# include <list>
# include <algorithm>
# include <utility>

# include <toolsbox/sig/iconnection.hh>
# include <toolsbox/sig/connection.hh>
# include <toolsbox/sig/group_connection.hh>

namespace toolsbox
{
  namespace sig
  {
    template <class T> class signal
    {
      public:
        typedef std::function<T>                   slot_type;
        typedef std::list<slot_type>               array_slot_type;
        typedef typename array_slot_type::iterator slot_id;

        signal()
          : process_guard_(false)
        {
          valid_.reset(new bool(true));
        }

        ~signal()
        {
          *valid_ = false;
        }

        signal(signal const&) = delete;
        signal& operator=(signal const&) = delete;

        connection connect(const slot_type &slot)
        {
          std::shared_ptr<sig_connection> co(new sig_connection(valid_, this));
          if(process_guard_)
          {
            slot_tmp_id id = to_add_slots_.insert(to_add_slots_.end(), std::make_pair(co, slot));
            co->set_id(slots_.end(), id);
          }
          else
          {
            slot_id id = slots_.insert(slots_.end(), slot);
            co->set_id(id, to_add_slots_.end());
          }

          return connection(co);
        }

        bool empty() const
        {
          return slots_.empty();
        }

        template <class ... ARGS> void operator()(ARGS ... args)
        {
          slot_guard guard(this);

          for(slot_type &slot: slots_)
          {
            slot(std::forward<ARGS>(args)...);
          }
        }

        template <class ... ARGS> void operator()(ARGS ... args) const
        {
          slot_guard guard(this);

          for(const slot_type &slot: slots_)
          {
            slot(std::forward<ARGS>(args)...);
          }
        }

      protected:
        class sig_connection;
        typedef std::list<std::pair<std::shared_ptr<sig_connection>, slot_type> > to_add_slots;
        typedef typename to_add_slots::iterator slot_tmp_id;

        class sig_connection : public iconnection
        {
          public:
            sig_connection(const std::shared_ptr<bool> &valid, signal* sig)
              : signal_valid_(valid)
              , signal_(*sig)
              , already_disconnect_(false)
            {
            }

            virtual void disconnect() override
            {
              if(valid())
              {
                signal_.disconnect_(id_, tmp_id_);
                already_disconnect_ = true;
              }
            }

            virtual bool valid() const override
            {
              return *signal_valid_ && !already_disconnect_;
            }

            virtual void set_auto_disconnect(bool) override
            {
            }

            void set_id(slot_id id, slot_tmp_id tmp_id)
            {
              id_ = id;
              tmp_id_ = tmp_id;
            }

          private:
            std::shared_ptr<bool> signal_valid_;
            signal& signal_;
            slot_id id_;
            slot_tmp_id tmp_id_;
            bool already_disconnect_;
        };

        void disconnect_(slot_id id, slot_tmp_id tmp_id)
        {
          if(process_guard_)
          {
            if(id != slots_.end())
            {
              to_delete_slots_.push_back(id);
            }
            if(tmp_id != to_add_slots_.end())
            {
              to_add_slots_.erase(tmp_id);
            }
          }
          else
          {
            slots_.erase(id);
          }
        }

        class slot_guard
        {
          public:
            slot_guard(signal* owner)
              : owner_(*owner)
            {
              guard_ = !owner_.process_guard_;
              owner_.process_guard_ = true;
            }

            ~slot_guard()
            {
              if(guard_)
              {
                owner_.process_guard_ = false;

                //
                // delete
                if(!owner_.to_delete_slots_.empty())
                {
                  for(slot_id &id : owner_.to_delete_slots_)
                  {
                    owner_.slots_.erase(id);
                  }
                  owner_.to_delete_slots_.clear();
                }

                //
                // add
                if(!owner_.to_add_slots_.empty())
                {
                  for(std::pair<std::shared_ptr<sig_connection>, slot_type> &to_add : owner_.to_add_slots_)
                  {
                    slot_id id = owner_.slots_.insert(owner_.slots_.end(), to_add.second);
                    to_add.first->set_id(id, owner_.to_add_slots_.end());
                  }
                  owner_.to_add_slots_.clear();
                }
              }
            }

          private:
            bool    guard_;
            signal& owner_;
        };

      private:
        array_slot_type slots_;
        std::shared_ptr<bool> valid_;

        mutable to_add_slots        to_add_slots_;
        mutable std::list<slot_id>  to_delete_slots_;
        mutable bool                process_guard_;
    };
  }
}

#endif
