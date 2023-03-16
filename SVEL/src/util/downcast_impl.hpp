#ifndef __UTIL_DOWNCAST_IMPL_HPP__
#define __UTIL_DOWNCAST_IMPL_HPP__

#define SVEL_DOWNCAST_IMPL(DERIVED, BASE)                                      \
  inline std::shared_ptr<DERIVED> GetImpl(std::shared_ptr<BASE> ptr) {         \
    return std::static_pointer_cast<DERIVED>(ptr);                             \
  }

#endif /* __UTIL_DOWNCAST_IMPL_HPP__ */