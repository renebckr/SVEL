/**
 * @file dispatcher.hpp
 * @author René Pascal Becker (rene.becker2@gmx.de)
 * @brief Describes interface for event dispatching.
 * @date 2023-08-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __CORE_EVENT_NOTIFIER_HPP__
#define __CORE_EVENT_NOTIFIER_HPP__

// STL
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace core::event {

/**
 * @brief Handle to a subscription that will unsubscribe when it goes out of
 * scope.
 */
class SubscriptionHandle {
public:
  /**
   * @brief Type of the unsubscribe callback.
   */
  using UnsubscribeCallback = std::function<void()>;

private:
  /**
   * @brief The unsubscribe method that must be called when the subscription
   * ends.
   */
  UnsubscribeCallback _unsubscribeMethod;

public:
  /**
   * @brief Construct a Subscription Handle. Will call unsubscribe after going
   * out of scope.
   *
   * @param unsubscribeMethod Method to call. Should be a method bound with
   * shared_from_this.
   */
  SubscriptionHandle(UnsubscribeCallback &&unsubscribeMethod)
      : _unsubscribeMethod(unsubscribeMethod) {}

  /**
   * @brief Destroy the Subscription handle. Terminates the subscription.
   */
  ~SubscriptionHandle() { _unsubscribeMethod(); }
};

/**
 * @brief Class that allows the communication of events to subscribers.
 *
 * @tparam Event Enum type that specifies event types.
 * @tparam Userdata Custom data passed to subscribers.
 */
template <typename Event, typename Userdata,
          std::enable_if_t<std::is_enum<Event>::value, bool> = true>
class Notifier
    : public std::enable_shared_from_this<Notifier<Event, Userdata>> {
public:
  /**
   * @brief Callback that is used for subscriptions.
   */
  using Callback = std::function<void(Event, const Userdata &)>;

private:
  /**
   * @brief Struct that holds information on a subscription.
   */
  struct SubscriberInfo {
    /**
     * @brief Id of the subscription.
     */
    size_t id;

    /**
     * @brief Callback for this subscription.
     */
    Callback callback;

    /**
     * @brief Construct a Subscriber Info.
     *
     * @param identifier         Unique Id of this subscription.
     * @param subscriberCallback Callback of this subscription.
     */
    SubscriberInfo(size_t identifier, Callback subscriberCallback = Callback())
        : id(identifier), callback(subscriberCallback) {}

    /**
     * @brief Comparison method of the Subscriber Info.
     *
     * @param otherSubscriber Subscriber Info to compare against.
     * @return true   The ids of both infos are the same.
     * @return false  The ids of both infos are different.
     */
    bool operator==(const SubscriberInfo &otherSubscriber) const {
      return this->id == otherSubscriber.id;
    }
  };

  /**
   * @brief Custom hasher for the Subscriber Info. (Not really a hash method
   * because we assume ids to be unique)
   */
  struct SubscriberHash {
    /**
     * @brief Hash method for the Subscriber Info.
     *
     * @param info Subscriber info to hash.
     * @return size_t Hash of the subscriber info.
     */
    size_t operator()(const SubscriberInfo &info) const { return info.id; }
  };

  /**
   * @brief Counter for ids to make sure that they are unique.
   */
  size_t _idCounter = 0;

  /**
   * @brief Map that contains all subscribers for each event type.
   */
  std::unordered_map<Event, std::unordered_set<SubscriberInfo, SubscriberHash>>
      _subscribers;

  /**
   * @brief Unsubscribe method that will be called by the subscription handle.
   *
   * @param eventType Type of the event.
   * @param id        Id of the subscription.
   */
  void _unsubscribe(Event eventType, size_t id) {
    auto &subscriberSet = _subscribers[eventType];
    if (const auto &it = subscriberSet.find(SubscriberInfo(id));
        it != subscriberSet.end())
      subscriberSet.erase(it);
  }

public:
  /**
   * @brief Subscribe to this notifier for the given event type. The caller must
   * keep the Subscription handle until no notification should be sent anymore.
   *
   * @param eventType Event type to subscribe to.
   * @param c         Callback to call when a notification occurs.
   * @return std::unique_ptr<SubscriptionHandle>  Handle to the subscription.
   */
  std::unique_ptr<SubscriptionHandle> Subscribe(Event eventType, Callback &&c) {
    _subscribers[eventType].emplace(SubscriberInfo(++_idCounter, c));
    return std::make_unique<SubscriptionHandle>(
        std::bind(&Notifier<Event, Userdata>::_unsubscribe,
                  this->shared_from_this(), eventType, _idCounter));
  }

  /**
   * @brief Notify every subscriber for the given type and userdata.
   *
   * @param eventType Event type for which to notify subscribers.
   * @param userdata  Userdata passed to the subscribers.
   */
  void Notify(Event eventType, const Userdata &userdata) {
    const auto &subscriberSet = _subscribers[eventType];
    for (auto &subscriber : subscriberSet)
      subscriber.callback(eventType, userdata);
  }
};

} // namespace core::event

#endif /* __CORE_EVENT_NOTIFIER_HPP__ */