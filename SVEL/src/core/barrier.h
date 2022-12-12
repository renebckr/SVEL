/**
 * @file barrier.h
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Declares Barrier
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 */

#ifndef __CORE_BARIER_H__
#define __CORE_BARIER_H__

#include "core/device.h"
#include "util/vulkan_object.hpp"

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace core {

/**
 * @brief Provides Safe resource allocation and deallocation for Fences.
 */
class Fence : public util::VulkanAdapter<vk::Fence> {
private:
  /**
   * @brief Device to use.
   */
  SharedDevice _device;

public:
  /**
   * @brief Construct an unsignaled Fence.
   *
   * @param device Device to use
   */
  Fence(SharedDevice device);

  /**
   * @brief Destroy the Fence
   */
  ~Fence();
};
SVEL_CLASS(Fence)

/**
 * @brief Barrier class that may be used in conjunction with a Fence to allow
 * for Data to stay alive until the activation of the Fence has been waited on.
 */
class Barrier {
public:
  /**
   * @brief Notifier that should be used to allow destruction of object on
   * completion of the barrier.
   */
  using Notifier = std::function<void()>;

private:
  /**
   * @brief Shared Pointer to the device that is in use.
   */
  SharedDevice _device;

  /**
   * @brief Fences to be waited on.
   */
  std::vector<vk::Fence> _fences;

  /**
   * @brief Map of Fences to Notifiers.
   */
  std::unordered_map<SharedFence, std::vector<Notifier>> _resources;

  /**
   * @brief Notifies every Notifier.
   */
  void _notifyAll();

public:
  /**
   * @brief Construct a Barrier object
   *
   * @param device
   */
  Barrier(SharedDevice device);

  /**
   * @brief Destroy the Barrier. Also waits until all fences are signaled.
   */
  ~Barrier();

  /**
   * @brief Add Ressources to the Barrier.
   *
   * @param fence Fence to wait on for these ressources.
   * @param resources Ressources that depend on the given Fence.
   */
  void AddResource(SharedFence fence, std::vector<Notifier> resources);

  /**
   * @brief Add a Ressource to the Barrier.
   *
   * @param fence Fence to wait on for the ressource.
   * @param resource Ressource that depends on the given Fence.
   */
  void AddResource(SharedFence fence, Notifier resource);

  /**
   * @brief Is this Barrier completed?
   *
   * @return true Barrier has completed.
   * @return false Barrier is not yet completed.
   */
  bool IsCompleted();

  /**
   * @brief Wait for the completion of this barrier.
   */
  void WaitCompletion();
};
SVEL_CLASS(Barrier)

} // namespace core

#endif /* __CORE_BARIER_H__ */