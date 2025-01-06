#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <optional>
#include "item.hpp"
#include "definitions.hpp"

class Inventory {
public:
    Inventory(int rows, int columns);

    void addItem(const Item& item); // Dodaj przedmiot do konkretnego slotu
    void drawInventory(sf::RenderWindow& window);        // Rysuj ekwipunek
    void toggleInventory() { mInventoryOpen = !mInventoryOpen; }
    bool getInventoryOpen() const { return mInventoryOpen; }
    void drawHotbarOnScreen(sf::RenderWindow& window);
    void updateHotbarSelection(int);
    std::optional<Item> getSelectedHotbarItem() const {
        if (mSelectedHotbarSlot >= 0 && mSelectedHotbarSlot < static_cast<int>(mSlots[0].size())) {
            return mSlots[3][mSelectedHotbarSlot];
        }
        return std::nullopt; // Jeśli slot jest poza zakresem, zwracamy brak przedmiotu
    }
private:
    int mRows, mColumns;
    bool mInventoryOpen = false;
    std::vector<std::vector<std::optional<Item>>> mSlots; // Sloty ekwipunku
    int mSelectedHotbarSlot = 0; // Indeks aktywnego slotu hotbara (domyślnie 0)
};

#endif // INVENTORY_HPP

