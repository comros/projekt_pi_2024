#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include  <SFML/Audio.hpp>
#include <functional>
#include <iostream>

class PauseMenu {
public:
    PauseMenu(Player& player, sf::Music& Music) : mIsVisible(false), mSettingsVisible(false), mPlayer(player), mMusic(Music) {}

    // Renderowanie menu pauzy
    void render(sf::RenderWindow& window) {
        if (mIsVisible) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f,1.0f));
            float windowWidth = 200.0f;
            float windowHeight = 200.0f;

            float menuX = (WINDOW_WIDTH - windowWidth) / 2.0f;
            float menuY = (WINDOW_HEIGHT - windowHeight) / 2.0f;

            float buttonWidth = 100.0f;
            float buttonHeight = 50.0f;
            float spacing = 10.0f; // Odstęp między przyciskami

            // Oblicz pozycję Y pierwszego przycisku, aby wszystkie były wyśrodkowane
            float totalButtonHeight = 3 * buttonHeight + 2 * spacing; // Wysokość 3 przycisków i odstępy
            float startY = (windowHeight - totalButtonHeight) / 2.0f;

            ImGui::SetNextWindowPos(ImVec2(menuX, menuY), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
            ImGui::Begin("Menu", &mIsVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

            // Pozycja X jest taka sama dla wszystkich przycisków (środek okna)
            float centerX = (windowWidth - buttonWidth) / 2.0f;

            // Ustaw pozycję pierwszego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY));
            if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) {
                if (mResumeCallback) mResumeCallback();
            }

            // Ustaw pozycję drugiego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY + buttonHeight + spacing));
            if (ImGui::Button("Settings", ImVec2(buttonWidth, buttonHeight))) {
                mSettingsVisible = true;
                mIsVisible = false;
            }

            // Ustaw pozycję trzeciego przycisku
            ImGui::SetCursorPos(ImVec2(centerX, startY + 2 * (buttonHeight + spacing)));
            if (ImGui::Button("Exit", ImVec2(buttonWidth, buttonHeight))) {
                if (mExitCallback) mExitCallback();
            }
            ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);
        }
        if (mSettingsVisible) {
            float windowWidth = 350.0f;
            float windowHeight = 200.0f;

            float menuX = (WINDOW_WIDTH - windowWidth) / 2.0f;
            float menuY = (WINDOW_HEIGHT - windowHeight) / 2.0f;
            ImGui::SetNextWindowPos(ImVec2(menuX, menuY), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
            ImGui::Begin("Settings", &mSettingsVisible,  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
            static float effectsVolume = 100.0f;
            float musicVolume = mMusic.getVolume() * 10;
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 5));
            if (ImGui::SliderFloat("Effects Volume", &effectsVolume, 0.0f, 200.0f)) {
                mPlayer.setEffectsVolume(effectsVolume); // Sets global volume in player
                }
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 5));
            if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 200.0f)) {
                mMusic.setVolume(musicVolume / 10); // Sets global volume in player
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 3, ImGui::GetCursorPosY() + 80));
            if (ImGui::Button("Back", ImVec2(100,50))) {
                mSettingsVisible = false;   // Zamknij okno ustawień
                mIsVisible = true;
            }
            ImGui::End();
        }

}

    void toggle() { mIsVisible = !mIsVisible; }
    bool isVisible() const { return mIsVisible; }
    bool SettingsVisible() const { return mSettingsVisible; }
    bool toggleSettings() { return mSettingsVisible = !mSettingsVisible; }
    // Callback setters
    void setResumeCallback(const std::function<void()>& callback) { mResumeCallback = callback; }
    void setExitCallback(const std::function<void()>& callback) { mExitCallback = callback; }

private:
    bool mIsVisible;
    bool mSettingsVisible;
    float effectsVolume;
    float musicVolume;

    std::function<void()> mResumeCallback;
    std::function<void()> mExitCallback;

    Player& mPlayer;
    sf::Music& mMusic;
};

#endif // PAUSEMENU_HPP
