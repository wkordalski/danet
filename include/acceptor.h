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
    virtual ~acceptor();
  private:
  protected:
    acceptor(netbase *nb, std::string ip, int port);
    void accept();
    void on_accept(const boost::system::error_code & ec, std::shared_ptr<connection> con);

    // Netbase object pointer
    netbase *nb;

    // Acceptor object pointer
    boost::asio::ip::tcp::acceptor *acc;
  };
}
#endif	/* ACCEPTOR_H */

