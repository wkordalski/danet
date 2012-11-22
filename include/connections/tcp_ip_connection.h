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
        virtual bool run(netbase *nb);
        virtual void password(const std::vector<byte>& passwd);

        virtual void send_data(const std::vector<byte>& data);

        void listen();

        void on_connect(const boost::system::error_code &ec);
        void on_header(const boost::system::error_code &ec, const size_t &bt);
        void on_body(const boost::system::error_code &ec, const size_t &bt);
        void on_send(const boost::system::error_code &ec);

        // Netbase object pointer
        netbase *nb;

        // Address to use
        danet::ip::tcp::address adr;

        // Socket used to communicate with other machine
        boost::asio::ip::tcp::socket *sck;

        // Buffer for header
        byte header_buff[8];

        // Strands
        boost::asio::strand *strd_r;     // Reading strand
        //boost::asio::strand strda;    // Send
        //boost::asio::strand strdb;    // Recv
        //std::queue<std::shared_ptr<std::vector<unsigned char>>> sndq;
        //std::mutex sndm;

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

