#include "acceptors/tcp_ip_acceptor.h"
#include "address/tcp_ip_address.h"

using namespace std;
namespace bnet = boost::asio;

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      acceptor::acceptor(danet::ip::tcp::address *adr)
      {
        this->adr = adr;
      }

      void acceptor::run(netbase* nb)
      {
        this->nb = nb;
        // Create acceptor
        acc = new bnet::ip::tcp::acceptor(danet::acceptor::get_ioservice(nb));

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
  }
}

