/*
 * File:   connection.h
 * Author: wojtex
 *
 * Created on 11 listopad 2012, 16:46
 */

#ifndef __DANET_CONNECTION_H
#define	__DANET_CONNECTION_H

namespace danet
{
  class connection;
}

#include "netbase.h"
#include "protocol.h"
#include "acceptor.h"

#include <boost/asio.hpp>

#include <mutex>
#include <string>
#include <vector>

namespace danet
{
  class connection
  {
    friend class netbase;
    friend class acceptor;
  public:
    virtual ~connection() {};

  protected:
    virtual bool run(netbase *nb, netbase::handle id) = 0;
    virtual bool run(netbase::handle id) = 0;

    virtual void send_data(std::shared_ptr<packet> data) = 0;

    virtual std::shared_ptr<danet::address> get_address() = 0;

    // Netbase object pointer
    netbase *nb;

    // Connection id
    netbase::handle id;

    boost::asio::io_service & get_ioservice()
    {
      return nb->service;
    }

    void forward_protocol(packet &pkg)
    {
      return nb->proto->data_received(pkg);
    }

    void connection_add()
    {
      return nb->proto->add_connection(id);
    }

    void connection_rem()
    {
      return nb->proto->rem_connection(id);
    }

    void cancel_connection()
    {
      nb->rem_connection(id);
    }

  private:

  };
}
#endif

