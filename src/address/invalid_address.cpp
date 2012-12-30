#include "address_invalid.h"

using namespace std;

namespace danet
{
  invalid_address::invalid_address()
  {
  }

  invalid_address::~invalid_address()
  {
  }

  shared_ptr<danet::acceptor> invalid_address::acceptor()
  {
    return shared_ptr<danet::acceptor>(nullptr);
  }

  shared_ptr<danet::connection> invalid_address::connection()
  {
    return shared_ptr<danet::connection>(nullptr);
  }

  bool invalid_address::valid()
  {
    return false;
  }
}
