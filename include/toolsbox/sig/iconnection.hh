#ifndef TOOLSBOX_SIG_ICONNECTION_HH
# define TOOLSBOX_SIG_ICONNECTION_HH

namespace toolsbox
{
  namespace sig
  {
    class iconnection
    {
      public:
        virtual ~iconnection(){}
        virtual void disconnect() = 0;
        virtual bool valid() const = 0;
        virtual void set_auto_disconnect(bool value) = 0;
    };
  }
}

#endif
