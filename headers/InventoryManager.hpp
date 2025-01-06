#include "inventory.hpp"
#include "item.hpp"

#ifndef INVENTORYMANAGER_HPP
#define INVENTORYMANAGER_HPP

class InventoryManager {
public:
    InventoryManager(sf::RenderWindow& Window) : mInventory(4,9), iron_ore("iron_ore",IRONORE, 0), stone("stone",STONE, 0), stick("stick",STICK, 0), pickaxe("pickaxe",PICKAXE), sword("sword",SWORD),axe("axe",AXE), mWindow(Window) {
    }

    void drawInventory() {
        mInventory.drawInventory(mWindow);
    }
    void toggleInventory() {
        mInventory.toggleInventory();
    }
    void drawHotbarOnScreen() {
        mInventory.drawHotbarOnScreen(mWindow);
    }
    void updateHotbarSelection(int i) {
        mInventory.updateHotbarSelection(i);
    }
    void addItem(Item& item) {
        mInventory.addItem(item);
    }
    void incrementItemCount(Item& item,int i) {
        item.incrementCount(i);
        mInventory.addItem(item);
    }
    bool getInventoryOpen() {
        return mInventory.getInventoryOpen();
    }
    std::optional<Item> getSelectedHotbarItem() {
        return mInventory.getSelectedHotbarItem();
    }
    Item iron_ore;
    Item pickaxe;
    Item axe;
    Item sword;
    Item stone;
    Item stick;
private:
    Inventory mInventory;
    sf::RenderWindow& mWindow;
};



#endif //INVENTORYMANAGER_HPP

