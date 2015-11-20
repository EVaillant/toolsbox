#ifndef TOOLSBOX_SIG_GROUP_CONNECTION_HH
# define TOOLSBOX_SIG_GROUP_CONNECTION_HH

# include <memory>
# include <list>

# include <toolsbox/sig/iconnection.hh>
# include <toolsbox/sig/connection.hh>

namespace toolsbox
{
  namespace sig
  {
    class group_connection : public iconnection
    {
      public:
        group_connection(bool auto_disconnect = false)
          : auto_disconnect_(auto_disconnect)
        {
        }

        group_connection(const connection& co, bool auto_disconnect = false)
          : auto_disconnect_(auto_disconnect)
        {
          connections_.push_back(co);
        }

        group_connection(connection && co, bool auto_disconnect = false)
          : auto_disconnect_(auto_disconnect)
        {
          connections_.push_back(std::move(co));
        }

        ~group_connection()
        {
          if(auto_disconnect_)
          {
            disconnect();
          }
        }

        virtual bool valid() const override
        {
          bool ret = false;

          for(const connection& co : connections_)
          {
            ret = ret || co.valid();
          }

          return ret;
        }

        virtual void disconnect() override
        {
          for(connection& co : connections_)
          {
            co.disconnect();
          }
        }

        virtual void set_auto_disconnect(bool value) override
        {
          auto_disconnect_ = value;
        }

        group_connection& operator+=(const connection & co)
        {
          connections_.push_back(co);
          return *this;
        }

        group_connection& operator+=(connection && co)
        {
          connections_.push_back(std::move(co));
          return *this;
        }

        group_connection& operator+=(const group_connection & co)
        {
          connections_.push_back(connection(std::shared_ptr<iconnection>(new group_connection(co))));
          return *this;
        }

      private:
        std::list<connection> connections_;
        bool auto_disconnect_;
    };
  }
}

#endif
