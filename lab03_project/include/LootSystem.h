#pragma once

#include "IEvent.h"
#include "Events.h"
#include <iostream>

class LootSystem {
public:
    static void handleEvent(const IEvent* e) {
        if (e->getType() == EventType::PlayerPickedItem) {
            const auto* event = dynamic_cast<const PlayerPickedItemEvent*>(e);
            if (event) {
                std::cout << "[LootSystem] Player " << event->getPlayerId() 
                          << " picked up: " << event->getItemName() << "\n";
            }
        }
    }
};