#pragma once

#include "Events.h"
#include <iostream>

class LootSystem {
public:
    static void handleEvent(const Event& e) {
        if (e.getType() == EventType::PlayerPickedItem) {
            std::cout << "[LootSystem] Player " << e.getId()
                        << " picked up: " << e.getItem() << "\n";
        }
    }
};