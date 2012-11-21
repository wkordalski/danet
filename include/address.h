/*
 * File:   address.h
 * Author: wojtex
 *
 * Created on 20 listopad 2012, 15:36
 */

#ifndef __DANET_ADDRESS_H
#define	__DANTE_ADDRESS_H

#include "netbase.h"
#include "acceptor.h"
#include "connection.h"

namespace danet
{
  class address
  {
    friend class netbase;
  public:
    virtual ~address() = 0;

  protected:
    virtual danet::acceptor *    acceptor() = 0;
    virtual danet::connection *  connection() = 0;
  };
}

#endif	/* ADDRESS_H */

