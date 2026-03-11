#include "Weapon.h"

Weapon::Weapon(const std::string& name, int damage) : Item(name), damage(damage) {}

Item* Weapon::clone() const {
    return new Weapon(*this);
}

void Weapon::print(std::ostream& os) const {
    os << "Weapon: " << getName() << ", damage";
}