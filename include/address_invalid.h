/*
 * File:   address_invalid.h
 * Author: wojtex
 *
 * Created on 30 grudzień 2012, 19:55
 */

#ifndef __DANET_INVALID_ADDRESS_H
#define	__DANET_INVALID_ADDRESS_H

#include "address.h"

#include <memory>

namespace danet
{
  class invalid_address : public danet::address
  {
  public:
    invalid_address();
    ~invalid_address();
    bool valid();

  protected:
    std::shared_ptr<danet::acceptor>     acceptor();
    std::shared_ptr<danet::connection>   connection();
  };
}

#endif	/* ADDRESS_INVALID_H */

