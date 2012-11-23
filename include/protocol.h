#ifndef __DANET_PROTOCOL_H
#define	__DANET_PROTOCOL_H

namespace danet
{
  class protocol;
}

#include "netbase.h"

namespace danet
{
  class connection;

  class protocol
  {
    friend class connection;
  public:
    virtual ~protocol() {};
  protected:
    virtual void data_received(packet &pkg) = 0;
  };
}

#endif

