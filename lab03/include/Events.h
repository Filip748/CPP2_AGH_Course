#pragma once

#include <string>
#include <cstring>

enum class EventType {
    EnemyDamaged,
    EnemyDied,
    PlayerHealed,
    PlayerPickedItem
};

union EventData {
    int value;
    char item[32];
};

class Event {
public:
    Event(EventType t, int id) : m_type(t), m_id(id) {}

    static Event createEnemyDamaged(int id, int damage);
    static Event createEnemyDied(int id);
    static Event createPlayerHealed(int id, int amount);
    static Event createPlayerPickedItem(int id, const std::string& item);

    EventType getType() const { return m_type; }
    int getId() const { return m_id; }
    int getValue() const { return m_data.value; }
    const char* getItem() const { return m_data.item; }

private:
    EventType m_type;
    int m_id;
    EventData m_data;
};