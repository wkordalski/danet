#include "protocols/basic_00.h"

namespace danet
{
  namespace protocols
  {
    basic<0>::basic(int rts)
    {
      this->rts = rts;
      // TODO: Implementation
    }

    basic<0>::~basic()
    {
      // TODO: implement
    }

    void basic<0>::data_received(packet& pkg)
    {
      // TODO
    }
  }
}
