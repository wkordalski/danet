#include "address/tcp_ip_address.h"
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
