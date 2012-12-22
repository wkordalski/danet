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

#include "tcp_ip_acceptor.h"

#include "../connections/tcp_ip_connection.h"
#include "address/tcp_ip_address.h"

#include <string>

using namespace std;
namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      acceptor::acceptor(const danet::ip::tcp::address &adr) : adr(adr)
      {
      }

      bool acceptor::run(netbase* nb)
      {
        this->nb = nb;
        // Create acceptor
        acc = new bnet::ip::tcp::acceptor(this->get_ioservice());

        bsys::error_code ec;

        // Get the selected endpoint
        //netbase::address adr = nb->resolve(ip, port);
        bnet::ip::tcp::resolver resolver(this->get_ioservice());
        bnet::ip::tcp::resolver::query query(adr.ip, to_string(adr.port));
        bnet::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, ec);
        if(ec) return false;
        // Wszystko ładnie działa
        // Inicjalizuj akceptor
        acc->open(endpoint->endpoint().protocol(), ec);
        if(ec) return false;
        acc->set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
        acc->bind(*endpoint, ec);
        if(ec) return false;
        acc->listen(bnet::socket_base::max_connections, ec);
        if(ec) return false;

        // Akceptuj połączenie
        this->accept();

        return true;
      }

      acceptor::~acceptor()
      {
        acc->close();
        delete acc;
      }

      void acceptor::accept()
      {
        // Stwórz nowe połączeie.
        shared_ptr<danet::ip::tcp::connection> nextcon(new danet::ip::tcp::connection(nb));

        // Akceptuj kolejne połączenie
        acc->async_accept(*(nextcon->sck), std::bind(&acceptor::on_accept, this, std::placeholders::_1, nextcon));
      }

      void acceptor::on_accept(const boost::system::error_code& ec, std::shared_ptr<danet::ip::tcp::connection> con)
      {
        if(ec)
        {
          // TODO: jakieś błąd akceptacji
          return;
        }
        netbase::handle h = this->netbase_add_connection(con);
        con->run(h);
        con->listen();
        this->proto_add_connection(h);
        this->accept();
      }

      shared_ptr<danet::address> acceptor::get_address()
      {
        return shared_ptr<danet::address>(new danet::ip::tcp::address(this->adr));
      }
    }
  }
}

