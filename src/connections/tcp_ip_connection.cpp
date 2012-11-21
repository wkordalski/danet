#include "netbase.h"
#include "connections/tcp_ip_connection.h"

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
        strd_r = new boost::asio::strand(this->get_ioservice());
      }

      connection::~connection()
      {
        delete sck;
        delete strd_r;
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
        strd_r = new boost::asio::strand(this->get_ioservice());

        // TODO: I o wiele więcej...
        sck->async_connect(*endpoint, bind(&connection::on_connect, this, placeholders::_1));
        return true;
      }
    }
  }
}