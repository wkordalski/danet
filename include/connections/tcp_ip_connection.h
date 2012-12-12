#ifndef __DANET_TCP_IP_CONNECTION_H
#define	__DANET_TCP_IP_CONNECTION_H

#include "connection.h"
#include "acceptors/tcp_ip_acceptor.h"

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class connection : public danet::connection
      {
        friend class danet::netbase;
        friend class danet::ip::tcp::acceptor;
        friend class danet::ip::tcp::address;
      public:
        virtual ~connection();

      protected:
        connection(const danet::ip::tcp::address &adr);
        connection(netbase *nb);
        virtual bool run(netbase *nb, netbase::handle id);
        virtual bool run(netbase::handle id);

        virtual void send_data(std::shared_ptr<packet> data);

        virtual std::shared_ptr<danet::address> get_address();

        void listen();

        void recv();
        void send();

        void on_connect(const boost::system::error_code &ec);
        void on_header(const boost::system::error_code &ec, const size_t &bt);
        void on_body(const boost::system::error_code &ec, const size_t &bt);
        void on_send(const boost::system::error_code &ec, const size_t &bt);
        void on_hsend(const boost::system::error_code &ec, const size_t &bt);

        // Netbase object pointer
        //netbase *nb;

        // Address to use
        danet::ip::tcp::address adr;

        // Socket used to communicate with other machine
        boost::asio::ip::tcp::socket *sck;

        // Buffer for header
        byte rcv_b[4];
        std::vector<byte> rcv_d;
        std::mutex rcv_m;

        // Kolejka danych wysyłanych i jej mutex
        std::queue<std::shared_ptr<packet>> snd_q;
        std::mutex snd_m;
        byte snd_b[4];
        std::vector<byte> snd_d;

        //unsigned char head[8];
        //std::vector<unsigned char> buff;
        //std::mutex rcvm;
        //int ct, cs, cr;     // Receiving data
      private:

      };

    }
  }
}

#endif

