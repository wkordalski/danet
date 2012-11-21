#include "acceptors/tcp_ip_acceptor.h"
#include "connections/tcp_ip_connection.h"
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
        shared_ptr<danet::ip::tcp::connection> nextcon(new connection(nb));

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
  }
}

