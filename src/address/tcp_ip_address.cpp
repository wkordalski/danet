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

#include "address_ip_tcp.h"
#include "../acceptors/tcp_ip_acceptor.h"
#include "../connections/tcp_ip_connection.h"

#include <memory>

using namespace std;

namespace bnet = boost::asio;
namespace bsys = boost::system;

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      address::address(std::string ip, int port)
      {
        this->ip = ip;
        this->port = port;
      }

      address::address()
      {
        this->ip = "";
        this->port = -1;
      }

      address::~address()
      {
      }

      shared_ptr<danet::acceptor>  address::acceptor()
      {
        return shared_ptr<danet::acceptor>(new danet::ip::tcp::acceptor(*this));
      }

      shared_ptr<danet::connection> address::connection()
      {
        return shared_ptr<danet::connection>(new danet::ip::tcp::connection(*this));
      }

      bool address::valid()
      {
        if(this->port == -1) return false;
        if(this->ip == "") return false;
        return true;
      }

//      address::phys_addr address::resolve(netbase *nb)
//      {
//        bnet::ip::tcp::resolver resolver(nb->get_service());
//        bnet::ip::tcp::resolver::query query(ip, to_string(port));
//        bsys::error_code ec;
//        bnet::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, ec);
//        if(ec)
//        {
//          // Nastąpił błąd - prawdopodobnie nie znaleziono host...
//          // TODO: Rzuć wyjątek, etc.
//        }
//        return *endpoint;
//      }
    }
  }
}
