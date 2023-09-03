#ifndef __WINDOW_INPUT_KEY_MAPPING_H__
#define __WINDOW_INPUT_KEY_MAPPING_H__

// Internal
#include <GLFW/glfw3.h>
#include <memory>
#include <svel/detail/keys.h>

namespace SVEL_NAMESPACE {

class KeyMapping::Impl : public std::enable_shared_from_this<KeyMapping::Impl> {
public:
  union Key {
    int Combined;
    short Id[2]; // [0] = Key, [1] = Modifier

    Key() = default;
    Key(int key, int mod) {
      Id[0] = (short)key;
      Id[1] = (short)mod;
    }
  };

private:
  std::unordered_map<int, Callback> _mapping;

  void mappingGlfwCallback(GLFWwindow *_window, int _key, int _scancode,
                           int _action, int _mods);

public:
  bool Exists(const Key &key);
  void Remove(const Key &key);
  void Add(const Key &key, const Callback &cb);
  void Apply(GLFWwindow *win);
};

} // namespace SVEL_NAMESPACE

#endif /* __WINDOW_INPUT_KEY_MAPPING_H__ */