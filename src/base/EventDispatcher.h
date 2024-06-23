#ifndef RFID_HUB_DISPATCHER_H
#define RFID_HUB_DISPATCHER_H

#include <functional>
#include <map>
#include "Event.h"

using EventHandler = std::function<void(const Event&)>;

struct EventListener
{
  EventHandler  handler;
  unsigned long id;
};

class EventDispatcher
{
private:
  std::map<EventType, std::vector<EventListener>> _observers;
  long _nextId = 1;

public:
  unsigned long subscribe(const EventType& descriptor, EventHandler&& handler) {
    unsigned long id = _nextId++;
    _observers[descriptor].push_back(EventListener{handler, id});
    return id;
  }

  void trigger(const Event& event) {
    EventType type = event.type();
    if (_observers.find(type) == _observers.end()) {
      return;
    }

    auto& listeners = _observers.at(type);
    for (auto& listener : listeners) {
      listener.handler(event);
    }
  }

  void unsubscribe(unsigned long id) {
    for (auto& observer : _observers) {
      auto it = std::remove_if(
        observer.second.begin(),
        observer.second.end(),
        [&id](const EventListener& listener) {
          return listener.id == id;
        }
      );

      observer.second.erase(it, observer.second.end());
    }
  }
};

#endif //RFID_HUB_DISPATCHER_H
