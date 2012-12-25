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

    void basic<0>::on_receive(packet pkg)
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

    void basic<0>::do_send(packet p, const std::vector<netbase::user>& u)
    {
      // TODO
      // Stwórz pakiet do wysłania i wyślij do handle
      //this->add_message_sending(p, uchwyt)
      std::shared_ptr<packet> pp(new packet(move(p)));
      this->netbase_do_send(pp, 1);
    }

    void basic<0>::connection_add(netbase::handle h)
    {
      // TODO
    }

    void basic<0>::connection_rem(netbase::handle h)
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
