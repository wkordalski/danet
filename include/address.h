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

#ifndef __DANET_ADDRESS_H
#define	__DANET_ADDRESS_H

namespace danet
{
  class address;
}

#include "netbase.h"
#include <memory>

namespace danet
{
  class acceptor;
  class connection;

  /**
   * The address base class.
   */
  class address
  {
    friend class netbase;
  public:
    /**
     * Distroys the address object.
     */
    virtual ~address() {};

    /**
     * Checks if the address is valid
     * @return True if address is valid, otherwise false.
     */
    virtual bool valid() = 0;

  protected:
    /**
     * Creates an acceptor based on address.
     * @return Pointer to acceptor.
     */
    virtual std::shared_ptr<danet::acceptor>     acceptor() = 0;

    /**
     * Creates a connection based on address.
     * @return Pointer to connection.
     */
    virtual std::shared_ptr<danet::connection>   connection() = 0;
  };
}

#endif	/* ADDRESS_H */

