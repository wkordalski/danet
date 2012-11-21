/*
 * File:   basic_0.h
 * Author: wojtex
 *
 * Created on 21 listopad 2012, 21:49
 */

#ifndef __DANET_PROTOCOLS_BASIC_0_H
#define __DANET_PROTOCOLS_BASIC_0_H

#include "basic.h"

#include <memory>

namespace danet
{
  namespace protocols
  {
    template<>
    class basic<0> : public danet::protocol
    {
    public:
      virtual ~basic();

      static std::shared_ptr<protocol> create()
      {
        return std::shared_ptr<protocol>(new basic<0>());
      }

    protected:
      basic();
    };
  }
}

#endif	/* BASIC_0_H */

