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

