/*
 * File:   pynetwork.h
 * Author: wojtex
 *
 * Created on 25 grudzień 2012, 18:40
 */

#ifndef __DANET_PYTHON_PYNETWORK_H
#define	__DANET_PYTHON_PYNETWORK_H

#include <boost/python.hpp>

#include <netbase.h>
#include <string>
#include <vector>

namespace danet
{
  /**
   * Klasa menadżera sieci dla Pythona - zarządza połączeniami i dostarczaniem danych.
   */
  class pynetwork : protected netbase
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
    pynetwork(std::shared_ptr<protocol> pro) : netbase(pro)
    {

    }

    pynetwork(const pynetwork &) = delete;

    /**
     * Każe menadżerowi sieci nasłuchiwać na danym IP i porcie.
     *
     * @param adr Adres, na którym mamy słuchać
     * @return Zwraca uchwyt do akceptora.
     */
    handle listen(address *adr)
    {
      return this->_listen(adr);
    }

    /**
     * Każe menadżerowi sieci połączyć się z danym IP i portem.
     *
     * @param adr Adres, do którego mamy się podłączyć
     * @return Zwraca uchwyt do połączenia.
     */
    handle connect(address *adr)
    {
      return this->_connect(adr);
    }

    /**
     * Zamyka połączenie lub akceptor oznaczony przez uchwyt h.
     *
     * @param h Uchwyt do zamknięcia
     */
    void close(handle h)
    {
      this->_close(h);
    }

    // FIXME : Funkcja ma przyjmować boost::python::object
    /**
     * Wysyła wiadomość do użytkowników
     *
     * @param m Wiadomość do wysłania (jako string)
     * @param s ID osób, do których jest ta wiadomość
     */
    void send(const std::string &m, const std::vector<user> &s)
    {
      std::vector<byte> v;
      v.reserve(m.size());
      for(char c : m)
        v.push_back((byte)c);
      this->_send(v, s);
    }

    // FIXME : Funkcja ma modyfikować boost::python::object
    /**
     * Odbiera wiadomość z sieci
     *
     * @param m [out] Odebrana wiadomość.
     * @return ID użytkownika, od którego otrzymaliśmy wiadomość lub 0 jeśli nie ma żadnych wiadomości.
     */
    user receive(std::string &m)
    {
      user uid = 0;
      std::vector<byte> v;
      this->_recv(v, uid);
      if(uid > 0)
      {
        m.reserve(v.size());
        for(byte b : v)
          m.push_back((char)b);
      }
      return uid;
    }

    std::set<user> users()
    {
      return this->_get_users_list();
    }

    std::shared_ptr<danet::address> address(handle h)
    {
      return this->_get_address(h);
    }

    user id()
    {
      return this->_get_id();
    }
  };
}

#endif	/* PYNETWORK_H */

