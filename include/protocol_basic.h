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

#ifndef __DANET_PROTOCOLS_BASIC_BASIC_H
#define	__DANET_PROTOCOLS_BASIC_BASIC_H

#include "protocol.h"

namespace danet
{
  namespace protocols
  {
    /**
     * Basic communication protocol family
     * @tparam version Protocol version
     */
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

