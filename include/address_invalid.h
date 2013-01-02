#ifndef __DANET_INVALID_ADDRESS_H
#define	__DANET_INVALID_ADDRESS_H

#include "address.h"

#include <memory>

namespace danet
{
  /**
   * Represents an address that is allways invalid.
   */
  class invalid_address : public danet::address
  {
  public:
    /**
     * Creates such address.
     */
    invalid_address();

    /**
     * Distroys address.
     */
    ~invalid_address();

    /**
     * Checks if address is valid.
     * @return True if address is valid, otherwise false.
     */
    bool valid();

  protected:
    /**
     * Creates acceptor based on address.
     * @return Pointer to acceptor.
     */
    std::shared_ptr<danet::acceptor>     acceptor();

    /**
     * Creates connection based on connection.
     * @return Pointer to connection.
     */
    std::shared_ptr<danet::connection>   connection();
  };
}

#endif

