#pragma once

#include "Events.h"
#include <iostream>

class SoundSystem {
public:
    static void handleEvent(const Event& e) {
        switch (e.getType()) {
            case EventType::EnemyDamaged:
                std::cout << "[SoundSystem] hit sound\n";
                break;
            case EventType::PlayerHealed:
                std::cout << "[SoundSystem] healing sound\n";
                break;
            case EventType::PlayerPickedItem:
                std::cout << "[SoundSystem] item pickup sound\n";
                break;
            default:
                break;
        }
    }
};