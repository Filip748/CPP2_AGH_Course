#pragma once

#include <string>
#include "IEvent.h"


class EnemyDamagedEvent : public IEvent {
public:

    EnemyDamagedEvent(int id, int dmg) : m_enemyId(id), m_damage(dmg) {}
    EventType getType() const override { return EventType::EnemyDamaged; }
    int getEnemyId() const { return m_enemyId; }
    int getDamage() const { return m_damage; }

private:

    int m_enemyId;
    int m_damage;
};

// --- 

class EnemyDiedEvent : public IEvent {
public:

    explicit EnemyDiedEvent(int id) : m_enemyId(id) {}
    EventType getType() const override { return EventType::EnemyDied; }
    int getEnemyId() const { return m_enemyId; }

private:

    int m_enemyId;
};

// --- 

class PlayerHealedEvent : public IEvent {
public:

    PlayerHealedEvent(int id, int amount) : m_playerId(id), m_healAmount(amount) {}
    EventType getType() const override { return EventType::PlayerHealed; }
    int getPlayerId() const { return m_playerId; }
    int getHealAmount() const { return m_healAmount; }

private:

    int m_playerId;
    int m_healAmount;
};

// ---

class PlayerPickedItemEvent : public IEvent {
public:

    PlayerPickedItemEvent(int id, const std::string& item) 
        : m_playerId(id), m_itemName(item) {}
    EventType getType() const override { return EventType::PlayerPickedItem; }
    int getPlayerId() const { return m_playerId; }
    std::string getItemName()const { return m_itemName; }

private:

    int m_playerId;
    std::string m_itemName;
};