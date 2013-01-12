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

#ifndef __DANET_TCP_IP_CONNECTION_H
#define	__DANET_TCP_IP_CONNECTION_H

#include "netbase.h"
#include "connection.h"
#include "../acceptors/tcp_ip_acceptor.h"

#include <memory>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      /**
       * Represents TCP/IP connection
       */
      class connection : public danet::connection
      {
        friend class danet::netbase;
        friend class danet::ip::tcp::acceptor;
        friend class danet::ip::tcp::address;
      public:
        virtual ~connection();

      protected:
        /**
         * Creates new connection based on specified address.
         * @param adr The address.
         */
        connection(const danet::ip::tcp::address &adr);

        /**
         * Creates connection instance with specified netbase.
         * @param nb Pointer to netbase.
         */
        connection(netbase *nb);
        virtual bool run(netbase *nb, netbase::handle id);
        virtual bool run(netbase::handle id);

        virtual void do_send(std::shared_ptr<packet> data);

        virtual std::shared_ptr<danet::address> get_address();

        /**
         * Makes the connection starts listening for data.
         */
        void listen();

        /**
         * Starts receiving data.
         */
        void recv();

        /**
         * Starts sending data.
         */
        void send();

        /**
         * Invoked when the connection starts beeing valid.
         * @param ec The errorcode.
         */
        void on_connect(const boost::system::error_code &ec);

        /**
         * Invoked when the connection received message header.
         * @param ec The error code
         * @param bt Number of bytes received.
         */
        void on_header(const boost::system::error_code &ec, const size_t &bt);

        /**
         * Invoked when the connection received message body.
         * @param ec The error code.
         * @param bt Number of bytes received.
         */
        void on_body(const boost::system::error_code &ec, const size_t &bt);

        /**
         * Invoked when a message has been sent.
         * @param ec The error code.
         * @param bt Number of bytes sent.
         */
        void on_send(const boost::system::error_code &ec, const size_t &bt);

        /**
         * Invoked when a message header has been sent.
         * @param ec The error code.
         * @param bt Number of bytes sent.
         */
        void on_hsend(const boost::system::error_code &ec, const size_t &bt);

        /**
         * The address associated to connection.
         */
        danet::ip::tcp::address adr;

        /**
         * Pointer to Boost Asio Socket object.
         */
        boost::asio::ip::tcp::socket *sck;

        // Buffer for header
        /**
         * Header receiving buffer.
         */
        byte rcv_b[4];

        /**
         * Data receiving buffer.
         */
        std::vector<byte> rcv_d;

        /**
         * Receiving mutex.
         */
        std::mutex rcv_m;

        /**
         * Message sending queue.
         */
        std::queue<std::shared_ptr<packet>> snd_q;

        /**
         * Sending mutex.
         */
        std::mutex snd_m;

        /**
         * Header sending buffer.
         */
        byte snd_b[4];

        /**
         * Data sending buffer.
         */
        std::vector<byte> snd_d;

      private:

      };

    }
  }
}

#endif

