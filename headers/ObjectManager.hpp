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

    void renderObjects(sf::RenderWindow& window) {
        for (const auto& object : mObjects) {
            window.draw(object->getSprite());
        }
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
