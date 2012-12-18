/*
 * File:   basic.h
 * Author: wojtex
 *
 * Created on 21 listopad 2012, 21:43
 */

#ifndef __DANET_PROTOCOLS_BASIC_BASIC_H
#define	__DANET_PROTOCOLS_BASIC_BASIC_H

#include "protocol.h"

namespace danet
{
  namespace protocols
  {
    template<unsigned int version>
    class basic : public danet::protocol
    {
      static_assert(version < 1, "Invalid version number");
    public:
      virtual ~basic() {}
    };
  }
}

#endif	/* BASIC_H */

