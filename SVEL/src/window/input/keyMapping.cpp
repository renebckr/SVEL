#include "keyMapping.h"
#include "glfwCallbacks.h"
#include "svel/detail/keys.h"

#include <iostream>
#include <memory>
#include <stdexcept>

using namespace SVEL_NAMESPACE;

// --- Impl ---

void KeyMapping::Impl::mappingGlfwCallback(GLFWwindow * /* window */, int key,
                                           int /* scancode */, int action,
                                           int mods) {
  if (action == GLFW_REPEAT)
    return;
  bool pressed = action != GLFW_RELEASE;
  Key binding((short)key, (short)mods);

  if (const auto &it = _mapping.find(binding.Combined); it != _mapping.end())
    it->second(pressed);
}

bool KeyMapping::Impl::Exists(const Key &key) {
  return _mapping.find(key.Combined) != _mapping.end();
}

void KeyMapping::Impl::Remove(const Key &key) {
  if (const auto &it = _mapping.find(key.Combined); it != _mapping.end())
    _mapping.erase(it);
}

void KeyMapping::Impl::Add(const Key &key, const Callback &cb) {
  _mapping.insert_or_assign(key.Combined, cb);
}

void KeyMapping::Impl::Apply(GLFWwindow *win) {
  GlfwCallback::SetKeyFunc(
      win, std::bind(&KeyMapping::Impl::mappingGlfwCallback, shared_from_this(),
                     std::placeholders::_1, std::placeholders::_2,
                     std::placeholders::_3, std::placeholders::_4,
                     std::placeholders::_5));
}

// --- INTERFACE ---

Key KeyMapping::_toKey(char key) {
  // TODO: This could be more elaborate
  return static_cast<Key>(key);
}

KeyMapping::KeyMapping() { __pImpl = std::make_shared<Impl>(); }

bool KeyMapping::IsMapped(Key key, KeyModifier mod) {
  return __pImpl->Exists({static_cast<int>(key), static_cast<int>(mod)});
}

bool KeyMapping::IsMapped(char key, KeyModifier mod) {
  return IsMapped(_toKey(key), mod);
}

void KeyMapping::RemoveMapping(Key key, KeyModifier mod) {
  __pImpl->Remove({static_cast<int>(key), static_cast<int>(mod)});
}

void KeyMapping::RemoveMapping(char key, KeyModifier mod) {
  RemoveMapping(_toKey(key), mod);
}

void KeyMapping::SetMapping(Callback cb, Key key, KeyModifier mod) {
  __pImpl->Add({static_cast<int>(key), static_cast<int>(mod)}, cb);
}

void KeyMapping::SetMapping(Callback cb, char key, KeyModifier mod) {
  SetMapping(cb, _toKey(key), mod);
}
