#include "protocols/basic/basic_00.h"

using namespace std;

namespace danet
{
  namespace protocols
  {
    basic<0>::basic(int rts)
    {
      this->rts = rts;
      // TODO: Implementation
    }

    basic<0>::~basic()
    {
      // TODO: implement
    }

    void basic<0>::data_received(packet& pkg)
    {
      // TODO
      /*if(pkg.size() < 8)
      {
        // INVALID PACKAGE
      }
      if(pkg[0] == 0)
      {
        // Pakiet zwykły
        // Może być dalej jakiś checksum, etc. (3 bajty)
        // Drugie jest UID
        netbase::user u = 0;
        for(int i = 4; i < 8; i++)
        {
          u <<= 8;
          u |= pkg[i];
        }
        packet p;
        p.reserve(pkg.size() - 8);
        for(int i = 8; i < pkg.size(); i++)
        {
          p.push_back(pkg[i]);
        }
        this->add_received_message(p, u);
      }
      else
      {
        // Pakiet systemowy...
      }*/
      this->netbase_add_received_message(pkg, 1);
    }

    void basic<0>::send_data(packet p, const std::vector<netbase::user>& u)
    {
      // TODO
      // Stwórz pakiet do wysłania i wyślij do handle
      //this->add_message_sending(p, uchwyt)
      std::shared_ptr<packet> pp(new packet(move(p)));
      this->netbase_send_to_resource(pp, 1);
    }

    void basic<0>::add_connection(netbase::handle h)
    {
      // TODO
    }

    void basic<0>::rem_connection(netbase::handle h)
    {
      // TODO
    }

    netbase::user basic<0>::get_id()
    {
      // TODO
      return 0;
    }

    set<netbase::user> basic<0>::get_users()
    {
      return set<netbase::user>();
    }
  }
}
