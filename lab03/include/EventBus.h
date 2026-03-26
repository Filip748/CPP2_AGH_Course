#pragma once

#include <vector>
#include <queue>
#include "Events.h"

using EventCallback = void (*)(const Event&);

class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    void subscribe(EventCallback callback);
    void emit(const Event& e);
    void processEvents();

private:

    std::vector<EventCallback> m_subscribers;
    std::queue<Event> m_eventsQueue;
};