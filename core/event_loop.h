#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <queue>
#include <functional>
#include <cstdint>

class EventLoop {
public:
    using Callback = std::function<void()>;

    void schedule(uint64_t delay, Callback cb);
    void run();
    uint64_t now() const;

private:
    struct Event {
        uint64_t time;
        Callback cb;

        bool operator>(const Event& other) const {
            return time > other.time;
        }
    };

    uint64_t current_time = 0;

    std::priority_queue<
        Event,
        std::vector<Event>,
        std::greater<Event>
    > queue;
};

#endif
