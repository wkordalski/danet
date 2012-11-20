#include "address/tcp_ip_address.h"
#include "acceptors/tcp_ip_acceptor.h"
#include "connections/tcp_ip_connection.h"

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

      address::~address()
      {
      }

      danet::acceptor * address::acceptor()
      {
        return new danet::ip::tcp::acceptor(this);
      }

      danet::connection * address::connection()
      {
        return new danet::ip::tcp::connection(this);
      }

      address::phys_addr address::resolve(netbase *nb)
      {
        bnet::ip::tcp::resolver resolver(nb->get_service());
        bnet::ip::tcp::resolver::query query(ip, to_string(port));
        bsys::error_code ec;
        bnet::ip::tcp::resolver::iterator endpoint = resolver.resolve(query, ec);
        if(ec)
        {
          // Nastąpił błąd - prawdopodobnie nie znaleziono host...
          // TODO: Rzuć wyjątek, etc.
        }
        return *endpoint;
      }
    }
  }
}
