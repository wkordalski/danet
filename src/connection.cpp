/*
 * File:   connection.cpp
 * Author: wojtex
 *
 * Created on 11 listopad 2012, 16:46
 */

/*
 * Wiadomość:
 * 1 - type
 * 3 - clients count
 * 4 - message size
 *  <clients: 4 byte each
 *  <message: specified size>
 */

#include "connection.h"
#include "netbase.h"

#include <functional>

using namespace std;
namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  connection::connection(netbase *nb, string ip, int port)
  {
    this->nb = nb;
    netbase::address adr = nb->resolve(ip, port);
    this->strd_r = new bnet::strand(nb->service);
    sck = new bnet::ip::tcp::socket(nb->service);
    sck->async_connect(adr, bind(&connection::on_connect, this, placeholders::_1));
  }

  connection::connection(netbase *nb)
  {
    this->nb = nb;
    sck = new bnet::ip::tcp::socket(nb->service);
  }

  connection::~connection()
  {
    delete sck;
  }

  void connection::listen()
  {
    sck->async_read_some(bnet::buffer(header_buff, 8), strd_r->wrap(bind(&connection::on_header, this, placeholders::_1, placeholders::_2)));
  }

  void connection::on_connect(const boost::system::error_code& ec)
  {
    // TODO: what to do if we are connected...
  }

  void connection::on_header(const boost::system::error_code& ec, const size_t& bt)
  {
    // TODO: co gdy odbierzemy dane - traktuj jako nagłówek wiadomości
  }

  void connection::on_body(const boost::system::error_code& ec, const size_t& bt)
  {
    // TODO: co gdy odbierzemy dane - jako wiadomość
  }

  void connection::on_send(const boost::system::error_code& ec)
  {
    // TODO: co gdy wyślemy dane...
  }
}
