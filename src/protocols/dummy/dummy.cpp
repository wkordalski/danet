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
    for(int k = 3; k >= 0; k--)
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
        netbase::user uid = h;
        add[uid] = h;
        {
          // Wyślij ID
          shared_ptr<packet> p(new packet());
          p->push_back(1);  // SET ID
          p->push_back(0);  //
          p->push_back(0);  // CHECKSUM
          p->push_back(0);  //
          insert_int(uid, *p);
          this->netbase_send_to_resource(p, h);
        }
        {
          // WYŚLIJ INFO INNYM
          shared_ptr<packet> p(new packet());
          p->push_back(2);  // ADD CLIENT
          p->push_back(0);  //
          p->push_back(0);  // CHECKSUM
          p->push_back(0);  //
          insert_int(uid, *p);
          for(pair<netbase::user, netbase::handle> P : add)
          {
            this->netbase_send_to_resource(p, P.second);
          }
        }
      }
      else
      {
        if(this->mid == 0)
        {
          this->srv = h;
        }
      }
    }

    void dummy::data_received(packet& pkg)
    {
      if(this->isserver)
      {
        // SERWER POŚREDNICZY TYLKO W WYMIANIE PAKIETÓW
        // Do kogo przesłać?
        if(pkg[0] == 0)
        {
          // Sprawdź checksum
          int chk = (get_int(0, pkg) >> 8);
          if(chk != 0)
          {
            // Checksum niepoprawny
          }
          vector<netbase::user> tos;
          int snd = get_int(4, pkg);
          int rcvc= get_int(8, pkg);    // Zawsze 1!
          tos.reserve(rcvc);
          for(int i = 0; i < rcvc; i++)
            tos.push_back(get_int(12+i*4, pkg));
          shared_ptr<packet> p(new packet());
          p->reserve(pkg.size()-4-rcvc*4);
          p->push_back(0); // USER PACKAGE
          p->push_back(0); //
          p->push_back(0); // CHECKSUM
          p->push_back(0); //
          insert_int(snd, *p);
          for(int i = (12+rcvc*4); i < pkg.size(); i++)
            p->push_back(pkg[i]);
          for(netbase::user U : tos)
          {
            auto it = add.find(U);
            if(it == add.end()) continue;
            this->netbase_send_to_resource(p, it->second);
          }
        }
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
          packet p;
          p.reserve(pkg.size()-8);
          for(int i = 8; i < pkg.size(); i++)
            p.push_back(pkg[i]);
          this->netbase_add_received_message(p, snd);
        }
        else if(pkg[0] == 1)
        {
          // DOSTALIŚMY OD SERWERA ID
          int chk = get_int(0, pkg);
          int nid = get_int(4, pkg);
          this->mid = nid;
        }
        else if(pkg[0] == 2)
        {
          // DOŁĄCZYŁ NOWY KLIENT
          int chk = get_int(0, pkg);
          int nid = get_int(4, pkg);
          this->usr.insert(nid);
        }
        else if(pkg[0] == 3)
        {
          // KLIENT NAS OPUŚCIŁ
          int chk = get_int(0, pkg);
          int nid = get_int(4, pkg);
          this->usr.erase(nid);
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
        netbase::user uid = 0;
        for(pair<netbase::user,netbase::handle> P : add)
        {
          if(P.second == h)
          {
            uid = P.first;
            break;
          }
        }
        if(uid == 0) return;
        add.erase(uid);
        // TODO: Wyślij info do reszty
        shared_ptr<packet> p(new packet());
        p->push_back(3);  // REMOVE CLIENT
        p->push_back(0);  //
        p->push_back(0);  // CHECKSUM
        p->push_back(0);  //
        insert_int(uid, *p);
        for(pair<netbase::user, netbase::handle> P : add)
        {
          this->netbase_send_to_resource(p, P.second);
        }
      }
      else
      {
        if(h == this->srv)
        {
          // Rozłączyliśmy się z serwerem!
          mid = 0;
          add.clear();
          this->srv = 0;
        }
      }
    }

    void dummy::send_data(packet p, const std::vector<netbase::user>& u)
    {
      if(this->isserver)
      {
        // SERWER NIE MOŻE WYSYŁAĆ SAM WIADOMOŚCI!!!
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

    set<netbase::user> dummy::get_users()
    {
      return this->usr;
    }
  }
}
