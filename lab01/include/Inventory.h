#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include <vector>
#include <iostream>

class Inventory {
private:
    std::vector<Item*> items;

public:
    Inventory() = default;

    ~Inventory();

    Inventory(const Inventory& other);

    Inventory& operator=(const Inventory& other);

    void add(Item* item);

    Item& operator[](size_t index) { return *items[index]; }

    const Item& operator[](size_t index) const { return *items[index]; }

    friend std::ostream& operator<<(std::ostream& os, const Inventory& inv);
};

#endif