#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

class Item {
private:
    std::string name;
public:
    Item(const std::string& name) : name(name) {}

    virtual ~Item() = default;

    virtual Item* clone() const = 0;

    virtual void print(std::ostream& os) const = 0;

    std::string getName() const {
        return name;
    }

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        item.print(os);
        return os;
    }
};

#endif