#ifndef __SVEL_CONFIG_H__
#define __SVEL_CONFIG_H__

#ifndef SVEL_NAMESPACE
#define SVEL_NAMESPACE sv
#endif

#define SVEL_CLASS(T)                                                          \
  using Unique##T = std::unique_ptr<T>;                                        \
  using Shared##T = std::shared_ptr<T>;

#define SVEL_PIMPL                                                             \
private:                                                                       \
  class Impl;                                                                  \
  SVEL_CLASS(Impl)                                                             \
  SharedImpl __pImpl;                                                          \
                                                                               \
public:                                                                        \
  SharedImpl __getImpl() { return __pImpl; }

#endif /* __SVEL_CONFIG_H__ */