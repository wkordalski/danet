/*
 * FORMAT PAKIETU
 *  1) typ pakietu: system/user - 1b
 *  2) checksum                 - 3b
 *  3) sender                   - 4b
 *  4) receiver count           - 4b
 *  5) receivers                - 4b each
 *  6) data                     - the rest
 *
 */


#include "protocols/dummy/dummy.h"

#include <utility>
#include <vector>

using namespace std;

namespace
{
  void insert_int(int a, danet::packet &p)
  {
    p.push_back((a    )&0xFF);
    p.push_back((a>> 8)&0xFF);
    p.push_back((a>>16)&0xFF);
    p.push_back((a>>24)&0xFF);
  }

  int get_int(int i, danet::packet &p)
  {
    int a = 0;
    for(int k = 3; k >= 0; k++)
    {
      a <<= 8;
      a |= (p[k+i]);
    }
    return a;
  }
}

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
      if(this->isserver)
      {

      }
      else
      {
        if(pkg[0] == 0)
        {
          // Sprawdź checksum
          int chk = (get_int(0, pkg) >> 8);
          if(chk != 0)
          {
            // Checksum niepoprawny
          }
          int snd = get_int(4, pkg);
          int rcvc= get_int(8, pkg);    // Zawsze 1!
          int rcv = get_int(12,pkg);    // Zawsze twoje ID
          packet p;
          p.reserve(pkg.size()-16);
          for(int i = 16; i < pkg.size(); i++)
            p.push_back(pkg[i]);
          this->netbase_add_received_message(p, snd);
        }
        else
        {
          // TODO - inna wiadomość (systemowa)
        }
      }
    }

    void dummy::rem_connection(netbase::handle h)
    {
      // TODO
      if(this->isserver)
      {
        // Nie jesteśmy już połączeni z danym klientem
        for(pair<netbase::user,netbase::handle> p : add)
        {
          if(p.second == h)
          {
            add.erase(p.first);
            break;
          }
        }
        // TODO: Wyślij info do reszty
      }
      else
      {
        if(h == this->srv)
        {
          // Rozłączyliśmy się z serwerem!
          mid = 0;
          add.clear();
          sid = 0;
          this->srv = 0;
        }
      }
    }

    void dummy::send_data(packet p, const std::vector<netbase::user>& u)
    {
      if(this->isserver)
      {
        for(netbase::user U : u)
        {
          shared_ptr<packet> nts = shared_ptr<packet>(new packet());
          nts->reserve(p.size() + 16);
          nts->push_back(0);   // USER PACKET
          nts->push_back(0);   //--|
          nts->push_back(0);   //  |- CHECKSUM - empty now
          nts->push_back(0);   //--|
          insert_int(this->mid, *nts);
          insert_int(1, *nts);  // Rozmiar klient i tak pominie!
          insert_int(U, *nts);
          for(byte B : p) nts->push_back(B);
          this->netbase_send_to_resource(nts,this->add[U]);
        }
      }
      else
      {
        shared_ptr<packet> nts = shared_ptr<packet>(new packet());
        nts->reserve(p.size() + u.size() * 4 + 12);
        nts->push_back(0);   // USER PACKET
        nts->push_back(0);   //--|
        nts->push_back(0);   //  |- CHECKSUM - empty now
        nts->push_back(0);   //--|
        insert_int(this->mid, *nts);
        insert_int(u.size(), *nts);
        for(netbase::user U : u) insert_int(U, *nts);
        for(byte B : p) nts->push_back(B);
        this->netbase_send_to_resource(nts, this->srv);
      }
    }

    netbase::user dummy::get_id()
    {
      // TODO
      return 0;
    }
  }
}
