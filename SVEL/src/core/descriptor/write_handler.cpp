/**
 * @file write_handler.cpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Implementation of the WriteHandler
 * @date 2023-03-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "write_handler.h"

using namespace core::descriptor;

WriteHandler::WriteHandler(SharedSet set, uint32_t binding)
    : _set(set), _binding(binding) {
  // Update local buffer
  _buffer = _set->GetBuffer(_binding);
}

void WriteHandler::WriteData(void *_data) {
  // Write to the buffer
  unsigned int oldBuffer = _buffer->GetBufferIndex();
  _buffer->Write(_data);

  // On buffer change notify the set
  if (oldBuffer != _buffer->GetBufferIndex())
    _set->NotifyBufferChange(_binding);
  return;
}

void WriteHandler::Update(SharedSet set) {
  // Update internal values
  _set = set;
  _buffer = _set->GetBuffer(_binding);
}