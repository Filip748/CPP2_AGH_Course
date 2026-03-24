#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include "IEvent.h"

using EventCallback = std::function<void(const IEvent*)>;

class EventBus {
public:
    EventBus() = default;
    ~EventBus() = default;

    void subscribe(EventCallback callback);
    void emit(std::unique_ptr<IEvent> event);
    void processEvents();

private:

    std::vector<EventCallback> m_subscribers;
    std::queue<std::unique_ptr<IEvent>> m_eventsQueue;
};