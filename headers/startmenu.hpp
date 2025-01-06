#ifndef STARTMENU_HPP
#define STARTMENU_HPP

#include <imgui.h>


class StartMenu {
    public:

    StartMenu(Player& player, sf::Music& Music, WorldGen& WorldGen, ObjectManager& ObjectManager ) : mStarted(false), mSettings(false), mGenerate(false), mPlayer(player), mMusic(Music), mWorldGen(WorldGen), objectManager(ObjectManager) {}

    bool isStarted() const {return mStarted;}
    bool Settings() const {return mSettings;}
    void toggle() {mStarted = !mStarted;}

    void render(sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        float windowWidth = windowSize.x; // Szerokość okna
        float windowHeight = windowSize.y; // Wysokość okna
        if(!mGenerate) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));

            float buttonWidth = 100.0f;
            float buttonHeight = 50.0f;
            float spacing = 10.0f; // Odstęp między przyciskami

            // Oblicz pozycję Y pierwszego przycisku, aby wszystkie były wyśrodkowane
            float totalButtonHeight = 3 * buttonHeight + 2 * spacing; // Wysokość 3 przycisków i odstępy
            float startY = (windowHeight - totalButtonHeight) / 2.0f;
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
            ImGui::Begin("MenuStart",0 , ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
            float centerX = (windowWidth - buttonWidth) / 2.0f;

            // Ustaw pozycję pierwszego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY));
            if (ImGui::Button("Start Game", ImVec2(buttonWidth, buttonHeight))) {
                mGenerate = true;
            }

            // Ustaw pozycję drugiego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY + buttonHeight + spacing));
            if (ImGui::Button("Settings", ImVec2(buttonWidth, buttonHeight))) {
                mSettings = true;
            }

            // Ustaw pozycję trzeciego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY + 2 * (buttonHeight + spacing)));
            if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
                window.close();
            }

            ImGui::End();
            ImGui::PopStyleVar();
        }
        if (mSettings) {

            ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
            ImGui::Begin("Settings", &mSettings,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
            static float effectsVolume = 100.0f;
            float musicVolume = mMusic.getVolume() * 10;
            ImGui::SetCursorPos(ImVec2(0.1f*windowWidth,0.3f*windowHeight));
            ImGui::BeginGroup();
            if (ImGui::SliderFloat("Effects Volume", &effectsVolume, 0.0f, 200.0f)) {
                mPlayer.setEffectsVolume(effectsVolume); // Sets global volume in player
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 5));
            if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 200.0f)) {
                mMusic.setVolume(musicVolume / 10); // Sets global volume in player
            }
            ImGui::EndGroup();
            float buttonWidth = 120.0f;
            float buttonHeight = 30.0f;
            ImGui::SetCursorPosX(10);
            ImGui::SetCursorPosY(windowHeight - buttonHeight - 10);
            if (ImGui::Button("Back", ImVec2(buttonWidth,buttonHeight))) {
                mSettings = false;   // Zamknij okno ustawień
            }
            ImGui::End();
        }
        if (mGenerate) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::Begin("World Generation1", &mGenerate,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPos(ImVec2(0.1f*windowWidth,0.3f*windowHeight));
    ImGui::BeginGroup(); // Rozpocznij grupę
    ImGui::Text("Seeds:");
    ImGui::InputInt("Terrain Seed", &mWorldGen.mTerrainSeed);

    ImGui::Text("Objects:");
    ImGui::InputInt("Amount of objects", &objectManager.amountOfObjects);

    ImGui::Text("Noise Parameters:");
    ImGui::SliderFloat("Falloff Strength", &mWorldGen.falloffStrength, 1.0f, 10.0f);
    ImGui::SliderInt("Octaves", &mWorldGen.octaves, 1, 20);
    ImGui::SliderFloat("Frequency", &mWorldGen.frequency, 0.001f, 0.05f);

    ImGui::Text("Thresholds:");
    ImGui::SliderFloat("Deep Water Threshold", &mWorldGen.thresholdDeepWater, 0.0f, 1.0f);
    ImGui::SliderFloat("Shallow Water Threshold", &mWorldGen.thresholdShallowWater, 0.0f, 1.0f);
    ImGui::SliderFloat("Sand Threshold", &mWorldGen.thresholdSand, 0.0f, 1.0f);
    ImGui::SliderFloat("Grass Threshold", &mWorldGen.thresholdGrass, 0.0f, 1.0f);
    ImGui::EndGroup(); // Zakończ grupę

    // Przyciski na dole
    float buttonWidth = 120.0f;
    float buttonHeight = 30.0f;
    ImGui::SetCursorPosX(10);
    ImGui::SetCursorPosY(windowHeight - buttonHeight - 10);
    // Przyciski "Back" i "Generate Map"
    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        mGenerate = false; // Zamknij okno ustawień
    }

    ImGui::SetCursorPosX(windowWidth - 10 - buttonWidth); // Prawy dolny róg
    ImGui::SetCursorPosY(windowHeight - buttonHeight - 10);
    if (ImGui::Button("Generate Map", ImVec2(buttonWidth, buttonHeight))) {
        mWorldGen.generateMap(); // Trigger map regeneration with new parameters
        objectManager.clearObjects(); // Clear existing objects
        objectManager.spawnObjects({512, 512}); // Spawn new objects
        float playerClearanceRadius = 32.0f; // Minimum distance from objects
        sf::Vector2f playerSpawn = objectManager.findValidPlayerSpawn({512, 512}, playerClearanceRadius);
        mPlayer.setPosition(playerSpawn);
        mStarted = true;
    }

    ImGui::End();
}

}

    private:
    bool mStarted;
    bool mGenerate;
    bool mSettings;
    Player& mPlayer;
    sf::Music& mMusic;
    WorldGen& mWorldGen;
    ObjectManager& objectManager;
};

#endif //STARTMENU_HPP
