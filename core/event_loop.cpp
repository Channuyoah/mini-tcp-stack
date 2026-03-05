#include "event_loop.h"

void EventLoop::schedule(uint64_t delay, Callback cb) {
    queue.push(Event{current_time + delay, cb});
}

void EventLoop::run() {
    while (!queue.empty()) {
        Event e = queue.top();
        queue.pop();

        current_time = e.time;
        e.cb();
    }
}

uint64_t EventLoop::now() const {
    return current_time;
}
