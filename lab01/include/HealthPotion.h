#ifndef HEALTHPOTION_H
#define HEALTHPOTION_H

#include "Item.h"

class HealthPotion : public Item {
private: 
    int healAmount;
public:
    HealthPotion(const std::string& name, int healAmount);

    Item* clone() const override;

    void print(std::ostream& os) const override;
};

#endif