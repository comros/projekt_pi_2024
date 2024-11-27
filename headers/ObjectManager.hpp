#ifndef OBJECT_MANAGER_HPP
#define OBJECT_MANAGER_HPP

#include "GameObject.hpp"
#include "WorldGen.hpp"
#include <vector>
#include <random>
#include <unordered_map>
#include <memory>

class ObjectManager {
public:
    // Constructor accepting WorldGen reference
    ObjectManager(WorldGen& worldGen) : mWorldGen(worldGen) {}

    void loadTextures() {
        // Load textures for different object types
        sf::Texture treeTexture, rockTexture, bushTexture;
        treeTexture.loadFromFile("../../assets/terrain/tree.png");
        rockTexture.loadFromFile("../../assets/terrain/rock.png");
        bushTexture.loadFromFile("../../assets/terrain/bush.png");

        mTextures["Tree"] = treeTexture;
        mTextures["Rock"] = rockTexture;
        mTextures["Bush"] = bushTexture;
    }

    void spawnObjects(const sf::Vector2i& gridSize) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, gridSize.x - 1);
        std::uniform_int_distribution<> distY(0, gridSize.y - 1);

        for (int i = 0; i < 1000; ++i) { // Example: Spawn 100 random objects
            sf::Vector2f position(distX(gen) * 16, distY(gen) * 16); // 16x16 tiles
            Tile& tile = mWorldGen.getTileAt(position.x / 16, position.y / 16); // Get tile at position

            // Now check the tile type to determine what to spawn
            if (tile.getType() == Tile::TileType::DeepWater || tile.getType() == Tile::TileType::ShallowWater) {
                // No objects are spawned on DeepWater or ShallowWater
                i--;
                continue;
            }
            else if (tile.getType() == Tile::TileType::Sand) {
                // Only spawn rocks on Sand
                spawnRock(position);
            }
            else if (tile.getType() == Tile::TileType::Grass) {
                // Spawn trees, bushes, or rocks on Grass
                int objectType = distX(gen) % 3; // Three object types (rock, bush, tree)
                if (objectType == 0) {
                    spawnRock(position);
                }
                else if (objectType == 1) {
                    spawnBush(position);
                }
                else {
                    spawnTree(position);
                }
            }
            else if (tile.getType() == Tile::TileType::Rock) {
                // Only spawn rocks on Rock tiles
                spawnRock(position);
            }
        }
    }

    void updateObjects() {
        for (const auto& object : mObjects) {
            // Logic for updating objects (e.g., animations, interactions)
        }
    }

    // Separate render functions for each object type
    void renderRocks(sf::RenderWindow& window) {
        for (const auto& object : mObjects) {
            if (auto rock = std::dynamic_pointer_cast<Rock>(object)) {
                window.draw(rock->getSprite()); // Render only rocks
            }
        }
    }

    void renderBushes(sf::RenderWindow& window) {
        for (const auto& object : mObjects) {
            if (auto bush = std::dynamic_pointer_cast<Bush>(object)) {
                window.draw(bush->getSprite()); // Render only bushes
            }
        }
    }

    void renderTrees(sf::RenderWindow& window) {
        for (const auto& object : mObjects) {
            if (auto tree = std::dynamic_pointer_cast<Tree>(object)) {
                window.draw(tree->getSprite()); // Render only trees
            }
        }
    }

    void handleObjectClick(const sf::Vector2f& worldPos, const sf::Vector2f& playerPos) {
        // Iterate through all objects in the container
        for (auto it = mObjects.begin(); it != mObjects.end(); ) {

            // Skip nullptr objects (objects that have been removed)
            if (it->get() == nullptr) {
                it = mObjects.erase(it);  // Remove invalid (nullptr) shared pointers
                continue;  // Skip to the next iteration
            }

            // Dereference the shared pointer to access the actual object
            GameObject& object = *it->get();

            // Check if the player is within the interaction range of the object
            if (object.getInteractionRange().contains(playerPos)) {
                // If player is in the interaction range, check if the click position is also inside the range
                if (object.getInteractionRange().contains(worldPos)) {
                    // Interact with the object (damage or other effects)
                    object.interact();

                    // If the object's health is 0 or less, remove it from the container
                    if (object.getHealth() <= 0) {
                        it = mObjects.erase(it);  // Erase the object from the container
                    } else {
                        ++it;  // Continue to the next object if it is still alive
                    }
                    return;  // Only handle one object per click
                }
            }

            // Move to the next object if the player isn't in interaction range
            ++it;
        }
    }

    const std::vector<std::shared_ptr<GameObject>>& getObjects() const {
        return mObjects;
    }

private:
    void spawnRock(const sf::Vector2f& position) {
        std::shared_ptr<GameObject> object = std::make_shared<Rock>(position, mTextures["Rock"]);
        mObjects.push_back(object);
    }

    void spawnBush(const sf::Vector2f& position) {
        std::shared_ptr<GameObject> object = std::make_shared<Bush>(position, mTextures["Bush"]);
        mObjects.push_back(object);
    }

    void spawnTree(const sf::Vector2f& position) {
        std::shared_ptr<GameObject> object = std::make_shared<Tree>(position, mTextures["Tree"]);
        mObjects.push_back(object);
    }

    std::unordered_map<std::string, sf::Texture> mTextures;
    std::vector<std::shared_ptr<GameObject>> mObjects;
    WorldGen& mWorldGen; // Reference to WorldGen for tile information
};

#endif // OBJECT_MANAGER_HPP
