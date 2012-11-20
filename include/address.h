/*
 * File:   address.h
 * Author: wojtex
 *
 * Created on 20 listopad 2012, 15:36
 */

#ifndef __DANET_ADDRESS_H
#define	__DANTE_ADDRESS_H

#include "acceptor.h"
#include "connection.h"
#include "resolver.h"

namespace danet
{
  class address
  {
  public:
    virtual ~address() = 0;

    virtual danet::acceptor *    acceptor() = 0;
    virtual danet::connection *  connection() = 0;
  };
}

#endif	/* ADDRESS_H */

