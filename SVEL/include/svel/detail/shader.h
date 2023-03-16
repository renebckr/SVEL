#ifndef __SVEL_DETAIL_SHADER_H__
#define __SVEL_DETAIL_SHADER_H__

#include <svel/config.h>

#include <memory>
#include <unordered_map>

namespace SVEL_NAMESPACE {

enum class BindingType {
  eUniformBuffer,
  eUniformBufferDynamic,
  eCombinedImageSampler
};

class SetLayout {
public:
  struct Binding {
    BindingType type;
    uint32_t dataElementSize;

    Binding(const BindingType &bindingType, uint32_t elementSize = 0)
        : type(bindingType), dataElementSize(elementSize) {}
  };

private:
  std::unordered_map<unsigned int, Binding> _bindings;

public:
  SetLayout &Add(unsigned int id, const Binding &binding);
  const std::unordered_map<unsigned int, Binding> &GetBindings() const;
};

class Shader {
public:
  enum class Type { eFragment, eVertex };

  virtual ~Shader(){};

  virtual Shader &AddSetLayout(unsigned int id, const SetLayout &setLayout) = 0;
};
SVEL_CLASS(Shader);

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_SHADER_H__ */