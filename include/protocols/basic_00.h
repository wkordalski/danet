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

      /**
       * Creates new protocol instance
       * @param rts Routing Table maximum size (-1 is unlimited)
       * @return The shared pointer to protocol instance.
       */
      static std::shared_ptr<protocol> create(int rts)
      {
        return std::shared_ptr<protocol>(new basic<0>(rts));
      }

    protected:
      basic(int rts);

      virtual void data_received(packet &pkg);

      int rts;
    };
  }
}

#endif	/* BASIC_0_H */

