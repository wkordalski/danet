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

#ifndef __DANET_ACCEPTOR_H
#define	__DANET_ACCEPTOR_H

namespace danet
{
  class acceptor;
}

#include "netbase.h"
#include "connection.h"

#include <boost/asio.hpp>

#include <memory>
#include <string>

namespace danet
{
  class acceptor
  {
    friend netbase;
  public:
    virtual ~acceptor() {};
  private:
  protected:
    virtual bool run(netbase *nb) = 0;
    virtual std::shared_ptr<danet::address> get_address() = 0;

    // Netbase object pointer
    netbase *nb;

    boost::asio::io_service & get_ioservice()
    {
      return nb->_service;
    }

    netbase::handle netbase_add_connection(std::shared_ptr<connection> con)
    {
      return this->nb->_connection_add(con);
    }

    void proto_add_connection(netbase::handle h)
    {
      return nb->_proto->connection_add(h);
    }


  };
}
#endif	/* ACCEPTOR_H */

