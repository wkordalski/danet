#ifndef __DANET_NETWORK_H
#define __DANET_NETWORK_H

#include "netbase.h"
#include "address.h"

#include <string>
#include <vector>


namespace
{
  std::vector<byte> default_password_serializer(const std::string &pwd)
  {
    return std::vector<byte>();
  }
}

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
  template<class T, class P>
  class network : protected netbase
  {
  public:
    /**
     * Represents the network manager handle type
     */
    //typedef netbase::handle handle;

    /**
     * Represents the network user id type
     */
    //typedef netbase::user user;

    /**
     * Tworzy obiekt menadżera sieci
     */
    network() : netbase()
    {

    }

    /**
     * Każe menadżerowi sieci nasłuchiwać na danym IP i porcie.
     *
     * @param adr Adres, na którym mamy słuchać
     * @return Zwraca uchwyt do akceptora.
     */
    handle listen(address *adr, const P& passwd)
    {
      return this->netbase::listen_at(adr, passwd.data());
    }

    /**
     * Każe menadżerowi sieci połączyć się z danym IP i portem.
     *
     * @param con Połączenie, które ma nawiązać.
     * @return Zwraca uchwyt do połączenia.
     */
    handle connect(address *adr, const P& passwd)
    {
      return this->netbase::connect_to(adr, passwd.data());
    }

    /**
     * Zamyka połączenie lub akceptor oznaczony przez uchwyt h.
     *
     * @param h Uchwyt do zamknięcia
     */
    void close(handle h)
    {
      this->netbase::close_resource(h);
    }

    /**
     * Wysyła wiadomość do użytkowników
     *
     * @param m Wiadomość do wysłania
     * @param s ID osób, do których jest ta wiadomość
     */
    void send(const T &m, const std::vector<user> &s)
    {
      this->netbase::send_message(T::get_data(m), s);
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
      this->netbase::recv_message(v, uid);
      if(uid > 0)
        m = T::set_data(v);
      return uid;
    }
  };
}

#endif