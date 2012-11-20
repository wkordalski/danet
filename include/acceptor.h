/*
 * File:   acceptor.h
 * Author: wojtex
 *
 * Created on 11 listopad 2012, 16:48
 */

#ifndef __DANET_ACCEPTOR_H
#define	__DANET_ACCEPTOR_H

namespace danet
{
  class acceptor;
}

#include "connection.h"
#include "netbase.h"

#include <boost/asio.hpp>

#include <memory>
#include <string>

namespace danet
{
  class acceptor
  {
    friend netbase;
  public:
    virtual ~acceptor() = 0;
  private:
  protected:
    virtual void run(netbase *nb) = 0;

    // Musi być...
    netbase *nb;

    boost::asio::io_service & get_ioservice()
    {
      return nb->service;
    }


  };
}
#endif	/* ACCEPTOR_H */

