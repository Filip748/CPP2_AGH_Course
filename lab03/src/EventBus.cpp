#include "EventBus.h"

void EventBus::subscribe(EventCallback callback) {
    m_subscribers.push_back(callback);
}

void EventBus::emit(const Event& e) {
    m_eventsQueue.push(e);
}

void EventBus::processEvents() {
    while(!m_eventsQueue.empty()) {
        Event current = m_eventsQueue.front();
        m_eventsQueue.pop();

        for(const auto& callback : m_subscribers) {
            callback(current);
        }
    }
}