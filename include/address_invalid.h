/*
 * Danet (Distributed Asynchronous Network)
 * This file is a part of danet library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For a list of authors see AUTHORS file.
 */

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

