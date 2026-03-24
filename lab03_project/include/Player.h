#pragma once

#include "EventBus.h"
#include <string>

class Player {
public:
    Player(int id, int hp, EventBus& bus);

    void heal(int amount);
    void pickItem(const std::string& itemName);

    int getId() const { return m_id; }
    int getHp() const { return m_hp; }


private:
    int m_id;
    int m_hp;
    EventBus& bus;
};