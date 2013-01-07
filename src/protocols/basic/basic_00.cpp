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
    basic<0>::basic()
    {
    }

    basic<0>::~basic()
    {
    }

    void basic<0>::on_receive(packet pkg)
    {
      // Przeczytaj wiadomość
      // Jeśli systemowa, obsłuż
      // Jeśli zwykła, przekaż do odbiorców i ew. dodaj do kolejki odebranych.
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
    }

    void basic<0>::connection_add(netbase::handle h)
    {
      if(_id == 0)
      {
        // TODO => pobierz ID
      }
      // INACZEJ OLEJ
      // ZAPISZ INFORMACJĘ, KTO JEST PO DRUGIEJ STRONIE.
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
      return _id;
    }

    set<netbase::user> basic<0>::get_users()
    {
      set<netbase::user> r;
      for(auto P : _rt)
        r.insert(P.first);
      return r;
    }
  }
}
