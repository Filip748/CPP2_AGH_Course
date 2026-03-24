#include "Player.h"
#include "Events.h"

Player::Player(int id, int hp, EventBus& bus) : m_id(id), m_hp(hp), bus(bus) {}

void Player::heal(int amount) {
    m_hp += amount;
    bus.emit(std::make_unique<PlayerHealedEvent>(getId(), amount));
}

void Player::pickItem(const std::string& itemName) {
    bus.emit(std::make_unique<PlayerPickedItemEvent>(getId(), itemName));
}