/*
 * File:   acceptor.cpp
 * Author: wojtex
 *
 * Created on 11 listopad 2012, 16:48
 */

#include "acceptor.h"
#include "netbase.h"

#include <functional>
#include <string>

using namespace std;
namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  acceptor::acceptor(netbase *nb, string ip, int port)
  {
    this->nb = nb;
    // Create acceptor
    acc = new bnet::ip::tcp::acceptor(nb->service);

    // Get the selected endpoint
    netbase::address adr = nb->resolve(ip, port);

    // Wszystko ładnie działa
    // Inicjalizuj akceptor
    acc->open(adr.endpoint().protocol());
    acc->set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
    acc->bind(adr);
    acc->listen(bnet::socket_base::max_connections);

    // Akceptuj połączenie
    this->accept();
  }

  acceptor::~acceptor()
  {
    acc->close();
    delete acc;
  }

  void acceptor::accept()
  {
    // Stwórz nowe połączeie.
    shared_ptr<connection> nextcon(new connection(nb));

    // Akceptuj kolejne połączenie
    acc->async_accept(*(nextcon->sck), std::bind(&acceptor::on_accept, this, std::placeholders::_1, nextcon));
  }

  void acceptor::on_accept(const boost::system::error_code& ec, std::shared_ptr<connection> con)
  {
    if(ec)
    {
      // TODO: jakieś błąd akceptacji
    }
    // TODO: co robić, gdy zaakceptowano
  }
}
