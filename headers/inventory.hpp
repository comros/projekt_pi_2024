#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <optional>
#include "item.hpp"
#include "definitions.hpp"

class Inventory {
public:
    Inventory(int rows, int columns);

    void addItem(const Item& item, int row, int column); // Dodaj przedmiot do konkretnego slotu
    void drawInventory(sf::RenderWindow& window);        // Rysuj ekwipunek
    void toggleInventory() { mInventoryOpen = !mInventoryOpen; }
    bool getInventoryOpen() const { return mInventoryOpen; }
    void drawHotbarOnScreen(sf::RenderWindow& window);
    void updateHotbarSelection(int);
private:
    int mRows, mColumns;
    bool mInventoryOpen = false;
    std::vector<std::vector<std::optional<Item>>> mSlots; // Sloty ekwipunku
    int mSelectedHotbarSlot = 0; // Indeks aktywnego slotu hotbara (domyślnie 0)
};

#endif // INVENTORY_HPP

