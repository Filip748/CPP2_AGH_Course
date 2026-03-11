#include <iostream>
#include "Weapon.h"
#include "HealthPotion.h"
#include "Inventory.h"

int main() {
    Inventory inventory;

    inventory.add(new Weapon("Sword", 25));
    inventory.add(new HealthPotion("Medium Health Potion", 50));

    std::cout << "=== POCZATKOWA ZAWARTOSC INVENTORY ===\n";
    std::cout << inventory << std::endl;

    std::cout << "=== DODANIE NOWEGO ITEMU ===\n";
    inventory.add(new Weapon("Axe", 40));

    std::cout << "=== AKTUALNA ZAWARTOSC ===\n";
    std::cout << inventory << std::endl;

    std::cout << "=== POJEDYNCZY ELEMENT ===\n";
    std::cout << inventory[0] << std::endl;

    const Inventory& cref = inventory;

    std::cout << "=== POJEDYNCZY ELEMENT (CONST) ===\n";
    std::cout << cref[1] << std::endl;

    Inventory copy = inventory;

    std::cout << "=== KOPIA INVENTORY ===\n";
    std::cout << copy << std::endl;

    std::cout << "=== MODYFIKACJA ORYGINALU (dodanie itemu) ===\n";
    inventory.add(new HealthPotion("Small Health Potion", 20));

    std::cout << "=== ORYGINAL PO MODYFIKACJI ===\n";
    std::cout << inventory << std::endl;

    std::cout << "=== KOPIA PO MODYFIKACJI ORYGINALU (nie powinna sie zmienic) ===\n";
    std::cout << copy << std::endl;

    return 0;
}