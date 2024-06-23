#ifndef RFID_HUB_TIMEOUTS_H
#define RFID_HUB_TIMEOUTS_H

#include <algorithm>
#include <functional>
#include <vector>

using TimeoutHandler = std::function<void()>;

struct Timer
{
  unsigned long  id;
  long           time;
  TimeoutHandler handler;
};

class Timeouts
{
private:
  unsigned long _lastMillis = 0;
  unsigned long _nextId = 1;
  std::vector<Timer> _timers;

public:
  void clearTimeout(unsigned long id) {
    auto it = std::remove_if(
      _timers.begin(),
      _timers.end(),
      [&id](const Timer& timer) {
        return timer.id == id;
      }
    );

    _timers.erase(it, _timers.end());
  }

  unsigned long setTimeout(TimeoutHandler&& handler, unsigned long delay) {
    unsigned long id = _nextId++;
    long time = (signed)delay;
    Timer timer = {
      id = id,
      time = time,
      handler = handler
    };

    _timers.push_back(timer);
    return id;
  }

protected:
  void processTimeouts() {
    unsigned long now = millis();
    long step = (signed)(now - _lastMillis);

    _lastMillis = now;
    if (step == now) {
      return;
    }

    for (auto &timer : _timers) {
      timer.time -= step;
      if (timer.time <= 0) {
        timer.handler();
      }
    }

    auto it = std::remove_if(
      _timers.begin(),
      _timers.end(),
      [](const Timer& timer) { return timer.time <= 0; }
    );

    _timers.erase(it, _timers.end());
  }
};

#endif //RFID_HUB_TIMEOUTS_H
