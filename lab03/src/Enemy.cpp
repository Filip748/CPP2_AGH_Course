#include "Enemy.h"
#include "Events.h"

Enemy::Enemy(int id, int hp, EventBus& bus) : m_id(id), m_hp(hp), bus(bus) {}

void Enemy::takeDamage(int amount) {
    m_hp -= amount;

    bus.emit(Event::createEnemyDamaged(getId(), amount));

    if(m_hp <= 0) {
        bus.emit(Event::createEnemyDied(getId()));
    }
}