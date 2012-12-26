/*
 * File:   pyprotocols.h
 * Author: wojtex
 *
 * Created on 26 grudzień 2012, 12:46
 */

#ifndef __DANET_PYTHON_PYPROTOCOLS_H
#define	__DANET_PYTHON_PYPROTOCOLS_H

#include <protocol.h>
#include <protocol_dummy.h>
#include <memory>

namespace danet
{
  class pyprotocols
  {
  public:
    std::shared_ptr<protocol> create_dummy(int server)
    {
      return danet::protocols::dummy::create(server);
    }
  };
}

#endif	/* PYPROTOCOLS_H */

