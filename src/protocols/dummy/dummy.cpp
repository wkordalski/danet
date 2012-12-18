#include "protocols/dummy/dummy.h"

namespace danet
{
  namespace protocols
  {
    dummy::dummy(bool server) : isserver(server)
    {
      // TODO
    }

    dummy::~dummy()
    {
      // TODO
    }

    void dummy::add_connection(netbase::handle h)
    {
      if(isserver)
      {
        // TODO
        // Wyślij ID
      }
    }

    void dummy::data_received(packet& pkg)
    {
      // TODO
    }

    void dummy::rem_connection(netbase::handle h)
    {
      // TODO
    }

    void dummy::send_data(packet p, const std::vector<netbase::user>& u)
    {
      // TODO
    }

    netbase::user dummy::get_id()
    {
      // TODO
      return 0;
    }
  }
}
