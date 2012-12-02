#include "netbase.h"
#include "connections/tcp_ip_connection.h"

#include <iostream>
#include <string>

using namespace std;
namespace bsys = boost::system;
namespace bnet = boost::asio;

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      connection::connection(const danet::ip::tcp::address &adr) : adr(adr)
      {
      }

      connection::connection(netbase* nb) : adr()
      {
        this->nb = nb;
        sck = new boost::asio::ip::tcp::socket(this->get_ioservice());
        //strd_r = new boost::asio::strand(this->get_ioservice());
        //strd_w = new boost::asio::strand(this->get_ioservice());
      }

      connection::~connection()
      {
        delete sck;
        //delete strd_r;
        //delete strd_w;
      }

      bool connection::run(netbase *nb)
      {
        this->nb = nb;

        bsys::error_code ec;

        bnet::ip::tcp::resolver resolver(this->get_ioservice());
        bnet::ip::tcp::resolver::query query(adr.ip, to_string(adr.port));
        bnet::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, ec);
        if(ec) return false;

        sck = new boost::asio::ip::tcp::socket(this->get_ioservice());
        //strd_r = new boost::asio::strand(this->get_ioservice());
        //strd_w = new boost::asio::strand(this->get_ioservice());

        // TODO: I o wiele więcej...
        sck->async_connect(*endpoint, bind(&connection::on_connect, this, placeholders::_1));
        return true;
      }

      void connection::on_connect(const boost::system::error_code& ec)
      {
        if(ec)
        {
          // TODO: Jakiś błąd...
        }
        // TODO: rób co trzeba
        this->listen();
      }

      void connection::listen()
      {
        this->recv();
        this->send();
      }

      void connection::recv()
      {
        rcv_m.lock();
        bnet::async_read(*(this->sck),bnet::buffer(rcv_b, 4),bind(&connection::on_header, this, placeholders::_1, placeholders::_2));
        rcv_m.unlock();
      }

      void connection::send_data(shared_ptr<packet> data)
      {
        snd_m.lock();
        this->snd_q.push(data);
        snd_m.unlock();
      }

      void connection::send()
      {
        snd_m.lock();
        if(!snd_q.empty())
        {
          unsigned int msize = snd_q.front()->size();
          this->snd_b[3] = ((msize      ) & 0xFF);
          this->snd_b[2] = ((msize >>  8) & 0xFF);
          this->snd_b[1] = ((msize >> 16) & 0xFF);
          this->snd_b[0] = ((msize >> 24) & 0xFF);
          bnet::async_write(*(this->sck),bnet::buffer(snd_b, 4), bind(&connection::on_hsend, this, placeholders::_1, placeholders::_2));
        }
        snd_m.unlock();
      }

      void connection::on_hsend(const boost::system::error_code& ec, const size_t &bt)
      {
        snd_m.lock();
        if(ec)
        {
          // Błąd przy wysyłaniu nagłówka... (w miejscu bt)
        }
        bnet::async_write(*(this->sck),bnet::buffer(*(snd_q.front()), snd_q.front()->size()), /*strd_w->wrap(*/bind(&connection::on_send, this, placeholders::_1, placeholders::_2)/*)*/);
        snd_m.lock();
      }

      void connection::on_send(const boost::system::error_code& ec, const size_t &bt)
      {
        snd_m.lock();
        if(ec)
        {
          // Błąd przy wysyłaniu danych... (w miejscu bt)
        }
        snd_q.pop();
        if(!snd_q.empty())
        {
          unsigned int msize = snd_q.front()->size();
          this->snd_b[3] = ((msize      ) & 0xFF);
          this->snd_b[2] = ((msize >>  8) & 0xFF);
          this->snd_b[1] = ((msize >> 16) & 0xFF);
          this->snd_b[0] = ((msize >> 24) & 0xFF);
          bnet::async_write(*(this->sck),bnet::buffer(snd_b, 4), /*strd_w->wrap(*/bind(&connection::on_hsend, this, placeholders::_1, placeholders::_2)/*)*/);
        }
        snd_m.unlock();
      }

      void connection::on_header(const boost::system::error_code& ec, const size_t& bt)
      {
        rcv_m.lock();
        if(ec)
        {
          // Błąd przy odbieraniu nagłówka danych
        }
        unsigned int msgsiz = 0;
        for(byte b: rcv_b)
        {
          msgsiz <<= 8;
          msgsiz |= (unsigned int)b;
        }
        // <odbierz dane pakietu>
        //rcv_d.clear();
        if(msgsiz == 0)
        {
          this->recv();
        }
        else
        {
          rcv_d.resize(msgsiz);
          bnet::async_read(*(this->sck),bnet::buffer(rcv_d),/*strd_r->wrap(*/bind(&connection::on_body, this, placeholders::_1, placeholders::_2)/*)*/);
          rcv_m.unlock();
        }
      }

      void connection::on_body(const boost::system::error_code& ec, const size_t& bt)
      {
        rcv_m.lock();
        if(ec)
        {
          // Błąd przy odbieraniu danych
        }
        this->forward_protocol(rcv_d);
        rcv_m.unlock();
        this->recv();
      }

      void connection::password(const std::vector<byte>& passwd)
      {
        // TODO
      }
    }
  }
}