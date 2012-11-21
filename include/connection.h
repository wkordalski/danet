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
    virtual ~connection() = 0;

  protected:
    virtual bool run(netbase *nb) = 0;

    // Netbase object pointer
    netbase *nb;

    boost::asio::io_service & get_ioservice()
    {
      return nb->service;
    }

  private:

  };
}
#endif

