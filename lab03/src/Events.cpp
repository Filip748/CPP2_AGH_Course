#include "Events.h"

Event Event::createEnemyDamaged(int id, int damage) {
    Event e(EventType::EnemyDamaged, id);
    e.m_data.value  = damage;
    return e;
}

Event Event::createEnemyDied(int id) {
    Event e(EventType::EnemyDied, id);
    return e;
}

Event Event::createPlayerHealed(int id, int amount) {
    Event e(EventType::PlayerHealed, id);
    e.m_data.value = amount;
    return e;
}

Event Event::createPlayerPickedItem(int id, const std::string& item) {
    Event e(EventType::PlayerPickedItem, id);
    std::strncpy(e.m_data.item, item.c_str(), 31);
    e.m_data.item[31] = '\0';
    return e;
}