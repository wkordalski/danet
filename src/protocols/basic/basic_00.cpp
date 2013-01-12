/*
 * Danet (Distributed Asynchronous Network)
 * This file is a part of danet library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For a list of authors see AUTHORS file.
 */

#include "protocol_basic_00.h"

#include <ctime>

using namespace std;

namespace danet
{
  namespace protocols
  {
    basic<0>::basic()
    {
      // Something better for seed.
      _rn.seed(time(0));
    }

    basic<0>::~basic()
    {
    }

    void basic<0>::on_receive(packet pkg, netbase::handle h)
    {
      // Przeczytaj wiadomość
      // Jeśli systemowa, obsłuż      // Jeśli zwykła, przekaż do odbiorców i ew. dodaj do kolejki odebranych.
      switch(pkg[0])
      {
        case 0:
          // Null package
          break;
        case 1:
          // Normal package
          break;
        case 2:
          // Broadcast package
          break;
        case 3:
          // New user
          break;
        case 4:
          // New connection
          break;
        case 5:
          // Somebody wants to join.
          std::uniform_int_distribution<netbase::user> gen(1, 0x7FFFFFFF);
          int nn;
          _dm.lock();
          do
          {
            nn = gen(_rn);
          } while(_rt.find(nn) != _rt.end());
          // nn is new id
          // send message(7) to h
          // and send message(3) to all
          // send message(4) to all
          _dm.unlock();
          break;
        case 6:
          // Somebody wants to add a connection.
          break;
        case 7:
          // Add user response
          // Read your id
          // Send request for network topology of this network.
          break;
        case 8:
          // Add connection response.
          break;
        case 9:
          // Network topology request.
          break;
        case 10:
          // Connection removed.
          break;
        default:
          // Invalid message id.
          break;
      }
    }

    void basic<0>::do_send(std::shared_ptr<packet> p, const std::vector<netbase::user>& u)
    {
      // TODO
      // Pogrupuj użytkowników po handle, do którego trzeba wysłać wiadomość
      // Utwórz wiadomości
      // Wyślij je.
    }

    void basic<0>::do_send_all(std::shared_ptr<packet> p)
    {
      // TODO
      // Simply send to handles from bcast_algo function.
    }

    void basic<0>::connection_add(netbase::handle h)
    {
      _dm.lock();
      if(_id == -1)
      {
        _id = 0;
        // Send add user request
        shared_ptr<packet> p(new packet());
        p->push_back(5);
        p->push_back(0);  //
        p->push_back(0);  // CHECKSUM
        p->push_back(0);  //
        this->netbase_do_send(p, h);
      }
      else
      {
        // INACZEJ OLEJ
        // ZAPISZ INFORMACJĘ, KTO JEST PO DRUGIEJ STRONIE.
      }
      _dm.unlock();
    }

    void basic<0>::connection_rem(netbase::handle h)
    {
      vector<netbase::user> du;
      for(auto P : _rt)
      {
        if(P.second == h)
          du.push_back(P.first);
      }

      // ROZESLIJ INFO O ODŁĄCZENIU OD SIECI
    }

    netbase::user basic<0>::get_id()
    {
      return (_id == -1)?0:_id;
    }

    set<netbase::user> basic<0>::get_users()
    {
      set<netbase::user> r;
      for(auto P : _rt)
        r.insert(P.first);
      return r;
    }
    
    set<netbase::handle> basic<0>::bcast_algo(netbase::user sender)
    {
      // TODO
    }
  }
}
