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
    virtual ~acceptor() = 0;
  private:
  protected:
    virtual bool run(netbase *nb) = 0;

    // Netbase object pointer
    netbase *nb;

    boost::asio::io_service & get_ioservice()
    {
      return nb->service;
    }


  };
}
#endif	/* ACCEPTOR_H */

