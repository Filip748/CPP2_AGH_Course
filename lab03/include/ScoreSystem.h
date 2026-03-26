#pragma once

#include "Events.h"
#include <iostream>

class ScoreSystem {
public:
    static void handleEvent(const Event& e) {
        if(e.getType() == EventType::EnemyDied) {
            std::cout << "[ScoreSystem] Enemy " << e.getId()
                        << " beated!\n";
        } 
        else if (e.getType() == EventType::EnemyDamaged) {
            std::cout << "[ScoreSystem] Enemy " << e.getId() 
                        << " took " << e.getValue() << "\n";
        } 
    }
};