#include "HealthPotion.h"

HealthPotion::HealthPotion(const std::string& name, int healAmount)
    : Item(name), healAmount(healAmount) {}

Item* HealthPotion::clone() const {
    return new HealthPotion(*this);
}

void HealthPotion::print(std::ostream& os) const {
    os << "Health Potion: " << getName() << ", restores HP: " << healAmount;
}