#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"

class Weapon : public Item {
private:
    int damage;

public:
    Weapon(const std::string& name, int damage);

    Item* clone() const override;

    void print(std::ostream& os) const override;
};

#endif