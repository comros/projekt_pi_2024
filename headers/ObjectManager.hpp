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
    int amountOfObjects = 2500;
    sf::Shader outlineShader;

    // Constructor accepting WorldGen reference
    ObjectManager(WorldGen& worldGen) : mWorldGen(worldGen)
    {
        if (!outlineShader.loadFromFile("../../assets/shaders/outline.frag", sf::Shader::Fragment)) {
            throw std::runtime_error("Failed to load outline shader");
        }

    }

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

    const std::unordered_map<Tile::TileType, std::vector<std::pair<std::string, int>>> spawnWeights = {
        {Tile::TileType::Sand, {{"Rock", 80}, {"Bush", 20}}}, // More rocks, fewer bushes
        {Tile::TileType::Grass, {{"Tree", 70}, {"Bush", 29}, {"Rock", 1}}}, // Mostly trees, some bushes, few rocks
        {Tile::TileType::Rock, {{"Rock", 100}}} // Almost exclusively rocks
    };

    // Distance requirements for each object type
    const std::unordered_map<std::string, float> minDistances = {
        {"Tree", 16.0f},
        {"Rock", 16.0f},
        {"Bush", 16.0f}
    };

    std::string getWeightedRandomObject(const std::vector<std::pair<std::string, int>>& weights, std::mt19937& gen) {
        int totalWeight = 0;
        for (const auto& pair : weights) {
            totalWeight += pair.second;
        }

        std::uniform_int_distribution<> dist(0, totalWeight - 1);
        int randomValue = dist(gen);

        for (const auto& pair : weights) {
            if (randomValue < pair.second) {
                return pair.first;
            }
            randomValue -= pair.second;
        }

        return ""; // Should never reach here
    }

    void spawnObjects(const sf::Vector2i& gridSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, gridSize.x - 1);
    std::uniform_int_distribution<> distY(0, gridSize.y - 1);

    // Distance requirements for each object type
    const std::unordered_map<std::string, float> minDistances = {
        {"Tree", 32.0f},
        {"Rock", 16.0f},
        {"Bush", 16.0f}
    };

    for (int i = 0; i < amountOfObjects; ++i) { // Example: Spawn 1000 random objects
        sf::Vector2f position(distX(gen) * 16, distY(gen) * 16); // 16x16 tiles
        Tile& tile = mWorldGen.getTileAt(position.x / 16, position.y / 16); // Get tile at position

        // Skip invalid tiles
        if (tile.getType() == Tile::TileType::DeepWater || tile.getType() == Tile::TileType::ShallowWater) {
            i--;
            continue;
        }

        // Determine object type based on weights
        auto it = spawnWeights.find(tile.getType());
        if (it == spawnWeights.end()) {
            continue; // Skip tiles with no defined weights
        }

        std::string objectType = getWeightedRandomObject(it->second, gen);

        // Check if the spawn position is too close to any existing objects
        bool tooClose = false;
        for (const auto& object : mObjects) {
            if (object) {
                float distance = std::sqrt(std::pow(position.x - object->getPosition().x, 2) +
                                           std::pow(position.y - object->getPosition().y, 2));
                float requiredDistance = minDistances.at(objectType);
                if (distance < requiredDistance) {
                    tooClose = true;
                    break;  // Exit the loop if an object is too close
                }
            }
        }

        // If the position is too close to any object, skip this spawn attempt
        if (tooClose) {
            i--;
            continue;
        }

        // Spawn the object
        if (objectType == "Rock") {
            spawnRock(position);
        } else if (objectType == "Bush") {
            spawnBush(position);
        } else if (objectType == "Tree") {
            spawnTree(position);
        }
    }
}

    void updateObjects() {
        for (const auto& object : mObjects) {
            // Logic for updating objects (e.g., animations, interactions)
        }
    }

    // Separate render functions for each object type
    // I render from the end of the Array to keep the proper layering
    // Also they are only rendered if in view bounds, but I do it 3 times so there might be a better way?
    void renderRocks(sf::RenderWindow& window) {
        // Get the view's visible bounds
        sf::View view = window.getView();
        sf::FloatRect viewBounds(
            view.getCenter() - view.getSize() / 2.0f,
            view.getSize()
        );

        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto it = mObjects.rbegin(); it != mObjects.rend(); ++it) {
            if (auto rock = std::dynamic_pointer_cast<Rock>(*it)) {
                sf::FloatRect rockBounds = rock->getSprite().getGlobalBounds();
                if (viewBounds.intersects(rockBounds)) {
                    sf::Sprite sprite = rock->getSprite();

                    if (rockBounds.contains(mouseWorldPos)) {
                        // Set shader uniforms
                        outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
                        outlineShader.setUniform("textureSize", sf::Glsl::Vec2(sprite.getTexture()->getSize()));
                        outlineShader.setUniform("cornerColor", sf::Glsl::Vec4(0.0f, 0.0f, 0.0f, .25f)); // White
                        outlineShader.setUniform("cornerSize", 2.0f); // Adjust for gap size
                        outlineShader.setUniform("frameThickness", 1.0f); // Adjust for frame thickness

                        // Render the rock with the shader
                        window.draw(sprite, &outlineShader);
                    } else {
                        // Render the rock normally
                        window.draw(sprite);
                    }
                }
            }
        }
    }

void renderBushes(sf::RenderWindow& window) {
    // Get the view's visible bounds
    sf::View view = window.getView();
    sf::FloatRect viewBounds(
        view.getCenter() - view.getSize() / 2.0f,
        view.getSize()
    );

    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto it = mObjects.rbegin(); it != mObjects.rend(); ++it) {
        if (auto bush = std::dynamic_pointer_cast<Bush>(*it)) {
            sf::FloatRect bushBounds = bush->getSprite().getGlobalBounds();
            if (viewBounds.intersects(bushBounds)) {
                sf::Sprite sprite = bush->getSprite();

                if (bushBounds.contains(mouseWorldPos)) {
                    // Set shader uniforms
                    outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
                    outlineShader.setUniform("textureSize", sf::Glsl::Vec2(sprite.getTexture()->getSize()));
                    outlineShader.setUniform("cornerColor", sf::Glsl::Vec4(0.0f, 0.0f, 0.0f, .25f));
                    outlineShader.setUniform("cornerSize", 2.0f); // Adjust for gap size
                    outlineShader.setUniform("frameThickness", 1.0f); // Adjust for frame thickness

                    // Render the bush with the shader
                    window.draw(sprite, &outlineShader);
                } else {
                    // Render the bush normally
                    window.draw(sprite);
                }
            }
        }
    }
}

    void renderTreesBehindThePlayer(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Get the view's visible bounds
        sf::View view = window.getView();
        sf::FloatRect viewBounds(
            view.getCenter() - view.getSize() / 2.0f,
            view.getSize()
        );

        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto it = mObjects.rbegin(); it != mObjects.rend(); ++it) {
            if (auto tree = std::dynamic_pointer_cast<Tree>(*it)) {
                sf::FloatRect treeBounds = tree->getSprite().getGlobalBounds();
                if (viewBounds.intersects(treeBounds)) {
                    sf::Sprite sprite = tree->getSprite();

                    // Check if the mouse is over the tree for highlighting
                    if (treeBounds.contains(mouseWorldPos)) {
                        // Set shader uniforms
                        outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
                        outlineShader.setUniform("textureSize", sf::Glsl::Vec2(sprite.getTexture()->getSize()));
                        outlineShader.setUniform("cornerColor", sf::Glsl::Vec4(0.0f, 0.0f, 0.0f, 0.25f));
                        outlineShader.setUniform("cornerSize", 2.0f); // Adjust for gap size
                        outlineShader.setUniform("frameThickness", 1.0f); // Adjust for frame thickness

                        // Render the tree with the shader
                        window.draw(sprite, &outlineShader);
                    } else {
                        // Check if the tree is behind the player
                        if (!tree->isInUpperHalfOfInteractionRange(playerPos)) {
                            tree->adjustAlpha(1.0f); // Full opacity for trees behind the player

                            // Render the tree normally
                            window.draw(sprite);
                        }
                        else tree->adjustAlpha(0.5f);
                    }
                }
            }
        }
    }

        void renderTreesOnTopOfPlayer(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Get the view's visible bounds
        sf::View view = window.getView();
        sf::FloatRect viewBounds(
            view.getCenter() - view.getSize() / 2.0f,
            view.getSize()
        );

        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (auto it = mObjects.rbegin(); it != mObjects.rend(); ++it) {
            if (auto tree = std::dynamic_pointer_cast<Tree>(*it)) {
                sf::FloatRect treeBounds = tree->getSprite().getGlobalBounds();
                if (viewBounds.intersects(treeBounds)) {
                    sf::Sprite sprite = tree->getSprite();

                    // Check if the mouse is over the tree for highlighting
                    if (treeBounds.contains(mouseWorldPos) && tree->isInUpperHalfOfInteractionRange(playerPos)) {
                        // Set shader uniforms
                        outlineShader.setUniform("texture", sf::Shader::CurrentTexture);
                        outlineShader.setUniform("textureSize", sf::Glsl::Vec2(sprite.getTexture()->getSize()));
                        outlineShader.setUniform("cornerColor", sf::Glsl::Vec4(0.0f, 0.0f, 0.0f, 0.25f));
                        outlineShader.setUniform("cornerSize", 2.0f); // Adjust for gap size
                        outlineShader.setUniform("frameThickness", 1.0f); // Adjust for frame thickness

                        // Render the tree with the shader
                        window.draw(sprite, &outlineShader);
                    } else {
                        // Check if the tree is behind the player
                        if (tree->isInUpperHalfOfInteractionRange(playerPos)) {
                            tree->adjustAlpha(.5f); // Half opacity for trees behind the player

                            // Render the tree normally
                            window.draw(sprite);
                        }
                        else tree->adjustAlpha(1.0f);
                    }
                }
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
                if (object.getCollisionBox().contains(worldPos)) {
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

    bool isPositionOccupied(const sf::Vector2f& position, float radius) const {
        for (const auto& object : mObjects) {
            if (object) {
                float distance = std::sqrt(std::pow(position.x - object->getPosition().x, 2) +
                                           std::pow(position.y - object->getPosition().y, 2));
                if (distance < radius) {
                    return true; // Position is too close to an existing object
                }
            }
        }
        return false; // Position is clear
    }

    sf::Vector2f findValidPlayerSpawn(const sf::Vector2i& gridSize, float objectClearanceRadius) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distX(0, gridSize.x - 1);
        std::uniform_int_distribution<> distY(0, gridSize.y - 1);

        while (true) {
            sf::Vector2f position(distX(gen) * 16, distY(gen) * 16); // 16x16 tiles
            Tile& tile = mWorldGen.getTileAt(position.x / 16, position.y / 16);

            // Skip invalid tiles
            if (tile.getType() != Tile::TileType::Sand) {
                continue;
            }

            // Check if the position is clear of objects
            if (!isPositionOccupied(position, objectClearanceRadius)) {
                return position; // Valid spawn position found
            }
        }
    }

    void clearObjects() {
        mObjects.clear(); // Clear the vector of objects
    }

    void adjustObjectBrightness(float brightness) {
        for (auto& object : mObjects) {
            if (object) {
                // Assuming all objects have a method to set their sprite color
                object->setBrightness(sf::Color(255, 255, 255), brightness); // Adjust brightness by changing RGB values
            }
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
