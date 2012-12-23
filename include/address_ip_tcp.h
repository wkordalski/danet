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

#ifndef __DANET_TCP_IP_ADDRESS_H
#define	__DANET_TCP_IP_ADDRESS_H

#include "address.h"

#include <string>

namespace danet
{
  namespace ip
  {
    namespace tcp
    {
      class acceptor;
      class connection;

      class address : public danet::address
      {
        friend class danet::ip::tcp::acceptor;
        friend class danet::ip::tcp::connection;

        std::string ip;
        int port;
      public:
        address(std::string ip, int port);
        ~address();
        bool valid();

      protected:
        address();

        std::shared_ptr<danet::acceptor>     acceptor();
        std::shared_ptr<danet::connection>   connection();
      };
    }
  }
}

#endif

