#pragma once

#include "IEvent.h"
#include "Events.h"
#include <iostream>

class ScoreSystem {
public:
    static void handleEvent(const IEvent* e) {
        if(e->getType() == EventType::EnemyDied) {
            const auto* event = dynamic_cast<const EnemyDiedEvent*>(e);
            if(event) {
                std::cout << "[ScoreSystem] Enemy " << event->getEnemyId() 
                          << " beated!\n";
            }
        } 
        else if (e->getType() == EventType::EnemyDamaged) {
            const auto* event = dynamic_cast<const EnemyDamagedEvent*>(e);
            if(event) {
                std::cout << "[ScoreSystem] Enemy " << event->getEnemyId() 
                          << " took " << event->getDamage() << "\n";
            }
        } 
    }
};