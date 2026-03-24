#pragma once

#include "IEvent.h"
#include "Events.h"
#include <iostream>

#pragma once
#include "IEvent.h"
#include "Events.h"
#include <iostream>

class SoundSystem {
public:
    static void handleEvent(const IEvent* e) {
        switch (e->getType()) {
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