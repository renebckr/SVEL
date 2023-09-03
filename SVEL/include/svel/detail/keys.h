
#ifndef __SVEL_DETAIL_KEYS_H__
#define __SVEL_DETAIL_KEYS_H__

// SVEL
#include <svel/config.h>

// STL
#include <functional>
#include <memory>

namespace SVEL_NAMESPACE {

enum class Key {
  eNone = 0,
  eEscape = 0x100,
  eEnter,
  eTab,
  eBackspace,
  eInsert,
  eDelete,
  eRight,
  eLeft,
  eDown,
  eUp,
  ePageUp,
  ePageDown,
  eHome,
  eEnd,
  eCapsLock = 0x118,
  eScrollLock,
  eNumLock,
  ePrintScreen,
  ePause,
  eF1 = 0x122,
  eF2,
  eF3,
  eF4,
  eF5,
  eF6,
  eF7,
  eF8,
  eF9,
  eF10,
  eF11,
  eF12,
  eNum0 = 0x140,
  eNum1,
  eNum2,
  eNum3,
  eNum4,
  eNum5,
  eNum6,
  eNum7,
  eNum8,
  eNum9,
  eNumDecimal,
  eNumDivide,
  eNumMultiply,
  eNumSubtract,
  eNumAdd,
  eNumEnter,
  eNumEqual,
  eLeftShift = 0x154,
  eLeftControl,
  eLeftAlt,
  eLeftSuper,
  eRightShift,
  eRightControl,
  eRightAlt,
  eRightSuper,
  eMenu
};

enum class KeyModifier {
  eNone = 0x0,
  eShift = 0x1,
  eControl = 0x2,
  eAlt = 0x4,
  eSuper = 0x8,
  eCapsLock = 0x10,
  eNumLock = 0x20
};
SVEL_FLAGS(KeyModifier);

class KeyMapping {
  SVEL_PIMPL

public:
  using Callback = std::function<void(bool _pressed)>;

private:
  Key _toKey(char key);

public:
  KeyMapping();

  bool IsMapped(Key key, KeyModifier mod = KeyModifier::eNone);
  bool IsMapped(char key, KeyModifier mod = KeyModifier::eNone);
  void RemoveMapping(Key key, KeyModifier mod = KeyModifier::eNone);
  void RemoveMapping(char key, KeyModifier mod = KeyModifier::eNone);
  void SetMapping(Callback cb, Key key, KeyModifier mod = KeyModifier::eNone);
  void SetMapping(Callback cb, char key, KeyModifier mod = KeyModifier::eNone);
};

} // namespace SVEL_NAMESPACE

#endif /* __SVEL_DETAIL_KEYS_H__ */