#include "EventBus.h"

void EventBus::subscribe(EventCallback callback) {
    m_subscribers.push_back(std::move(callback));
}

void EventBus::emit(std::unique_ptr<IEvent> event) {
    m_eventsQueue.push(std::move(event));
}

void EventBus::processEvents() {
    while(!m_eventsQueue.empty()) {
        std::unique_ptr<IEvent> currentEvent = std::move(m_eventsQueue.front());
        m_eventsQueue.pop();

        for(const auto& callback : m_subscribers) {
            callback(currentEvent.get());
        }
    }
}