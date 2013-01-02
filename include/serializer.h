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

#ifndef __DANET_SERIALIZER_H
#define	__DANET_SERIALIZER_H

#include <memory>

namespace danet
{
  /**
   * Base class for serializers used to send data through network.
   * @tparam T The message type
   */
  template<class T>
  class serializer
  {
  public:
    /**
     * Distroys the serializer.
     */
    virtual ~serializer() {};

    /**
     * Loads a message from it's binary form.
     * @param b Binary form of data.
     * @return The message.
     */
    virtual T load(std::vector<danet::byte> b) = 0;

    /**
     * Saves message to it's binary form.
     * @param m The message.
     * @return Binary form of data.
     */
    virtual std::shared_ptr<std::vector<danet::byte>> save(const T & m) = 0;
  };
}

#endif	/* SERIALIZER_H */

