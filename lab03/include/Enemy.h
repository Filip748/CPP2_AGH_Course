#pragma once

#include "EventBus.h"

class Enemy {
public:

    Enemy(int id, int hp, EventBus& bus);
    
    void takeDamage(int amount);

    int getId() const { return m_id; }
    int getHp() const { return m_hp; }

private:

    int m_id;
    int m_hp;
    EventBus& bus;
};