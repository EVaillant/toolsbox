#ifndef TOOLSBOX_SIG_CONNECTION_HH
# define TOOLSBOX_SIG_CONNECTION_HH

# include <memory>
# include <toolsbox/sig/iconnection.hh>

namespace toolsbox
{
  namespace sig
  {
    class connection : public iconnection
    {
      public:
        connection(bool auto_disconnect = false)
          : auto_disconnect_(auto_disconnect)
        {
        }

        connection(const std::shared_ptr<iconnection>& co, bool auto_disconnect = false)
          : connection_(co)
          , auto_disconnect_(auto_disconnect)
        {
        }

        connection(std::shared_ptr<iconnection>&& co, bool auto_disconnect = false)
          : connection_(std::move(co))
          , auto_disconnect_(auto_disconnect)
        {
        }

        ~connection()
        {
          if(auto_disconnect_)
          {
            disconnect();
          }
        }

        virtual void disconnect() override
        {
          if(connection_)
          {
            connection_->disconnect();
          }
        }

        virtual bool valid() const override
        {
          return (connection_ ? connection_->valid() : false);
        }

        virtual void set_auto_disconnect(bool value) override
        {
          auto_disconnect_ = value;
        }

      private:
        std::shared_ptr<iconnection> connection_;
        bool auto_disconnect_;
    };
  }
}

#endif
