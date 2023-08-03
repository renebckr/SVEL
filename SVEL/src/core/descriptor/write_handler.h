/**
 * @file write_handler.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declaration of the WriteHandler
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_DESCRIPTOR_WRITE_HANDLER_H__
#define __CORE_DESCRIPTOR_WRITE_HANDLER_H__

// Local
#include "set.h"

namespace core::descriptor {

/**
 * @brief Convenience Class that allows easy writes to descriptor set buffers.
 */
class WriteHandler {
private:
  /**
   * @brief Set that is currently being used.
   */
  SharedSet _set;

  /**
   * @brief Binding to use.
   */
  uint32_t _binding;

  /**
   * @brief Buffer that is currently being used.
   */
  SharedIBuffer _buffer;

public:
  /**
   * @brief Construct a Write Handler.
   *
   * @param set     Set that should be initially used.
   * @param binding Binding identifier that the handlers uses.
   */
  WriteHandler(SharedSet set, uint32_t binding);

  /**
   * @brief Writes data to the current set and the current buffer.
   *
   * @param _data Data to be written.
   */
  void WriteData(void *_data);

  /**
   * @brief Updates the write handler. The new set replaces the old set and the
   * buffer is fetched again from the new set.
   *
   * @param set The new set to use.
   */
  void Update(SharedSet set);
};

} // namespace core::descriptor

#endif /* __CORE_DESCRIPTOR_WRITE_HANDLER_H__ */