#ifndef __DANET_ADDRESS_H
#define	__DANET_ADDRESS_H

namespace danet
{
  class address;
}

#include "netbase.h"
#include <memory>

namespace danet
{
  class acceptor;
  class connection;

  class address
  {
    friend class netbase;
  public:
    virtual ~address() {};
    virtual bool valid() = 0;

  protected:
    virtual std::shared_ptr<danet::acceptor>     acceptor() = 0;
    virtual std::shared_ptr<danet::connection>   connection() = 0;
  };
}

#endif	/* ADDRESS_H */

