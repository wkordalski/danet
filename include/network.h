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

#ifndef __DANET_NETWORK_H
#define __DANET_NETWORK_H

#include "netbase.h"
#include "address.h"

#include <set>
#include <string>
#include <vector>


/**
 * Przestrzeń nazw zawierająca obiekty do zarządzania siecią.
 */
namespace danet
{
  /**
   * Klasa menadżera sieci - zarządza połączeniami i dostarczaniem danych.
   *
   * @param T Typ reprezentujący wiadomości wysyłane przez menadżer sieci.
   * Musi zawierać statyczne funkcje get_data i set_data.
   *
   * @param P Typ reprezentujący hasło przy łączeniu z siecią.
   * Musi zawierać funkcję data.
   */
  template<class T>
  class network : protected netbase
  {
  public:
    /**
     * Represents the network manager handle type
     */
    typedef netbase::handle handle;

    /**
     * Represents the network user id type
     */
    typedef netbase::user user;

    /**
     * Tworzy obiekt menadżera sieci
     */
    network(std::shared_ptr<protocol> pro) : netbase(pro)
    {

    }

    network(const network<T> &) = delete;

    /**
     * Każe menadżerowi sieci nasłuchiwać na danym IP i porcie.
     *
     * @param adr Adres, na którym mamy słuchać
     * @return Zwraca uchwyt do akceptora.
     */
    handle listen(address *adr)
    {
      return this->listen_at(adr);
    }

    /**
     * Każe menadżerowi sieci połączyć się z danym IP i portem.
     *
     * @param adr Adres, do którego mamy się podłączyć
     * @return Zwraca uchwyt do połączenia.
     */
    handle connect(address *adr)
    {
      return this->connect_to(adr);
    }

    /**
     * Zamyka połączenie lub akceptor oznaczony przez uchwyt h.
     *
     * @param h Uchwyt do zamknięcia
     */
    void close(handle h)
    {
      this->close_resource(h);
    }

    /**
     * Wysyła wiadomość do użytkowników
     *
     * @param m Wiadomość do wysłania
     * @param s ID osób, do których jest ta wiadomość
     */
    void send(const T &m, const std::vector<user> &s)
    {
      this->send_message(T::get_data(m), s);
    }

    /**
     * Odbiera wiadomość z sieci
     *
     * @param m [out] Odebrana wiadomość.
     * @return ID użytkownika, od którego otrzymaliśmy wiadomość lub 0 jeśli nie ma żadnych wiadomości.
     */
    user receive(T &m)
    {
      user uid = 0;
      std::vector<byte> v;
      this->recv_message(v, uid);
      if(uid > 0)
        m = T::set_data(v);
      return uid;
    }

    std::set<user> users()
    {
      return this->get_users_list();
    }

    std::shared_ptr<danet::address> address(handle h)
    {
      return this->get_address(h);
    }

    user id()
    {
      return this->get_id();
    }
  };
}

#endif