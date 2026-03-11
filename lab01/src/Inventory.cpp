#include "Inventory.h"

Inventory::~Inventory() {
    for (Item* item : items) {
        delete item;
    }
}

Inventory::Inventory(const Inventory& other) {
    for (Item* item : other.items) {
        items.push_back(item->clone());
    }
}

Inventory& Inventory::operator=(const Inventory& other) {
    if(this == &other) return *this;

    for(Item* item : items) {
        delete item;
    }

    items.clear();

    for(Item* item : other.items) { 
        items.push_back(item->clone());
    }

    return *this;
}

void Inventory::add(Item* item) { 
    items.push_back(item);
}

std::ostream& operator<<(std::ostream& os, const Inventory& inv) {
    for (const Item* item : inv.items) {
        os << "- " << *item << "\n";
    }
    return os;
}
