#include "../headers/inventory.hpp"

#include <imgui_internal.h>
#include <iostream>
#include "imgui.h"
#include "../cmake-build-debug/_deps/sfml-src/src/SFML/Window/Win32/CursorImpl.hpp"

Inventory::Inventory(int rows, int columns)
    : mRows(rows), mColumns(columns), mSlots(rows, std::vector<std::optional<Item>>(columns)) {std::cout<<std::endl<<"asdasdas"<<std::endl;}

void Inventory::addItem(const Item& newItem) {
    // Jeśli przedmiot jest stackowalny, sprawdzamy, czy już znajduje się w ekwipunku
    if (newItem.isStackable()) {
        if(newItem.getCount() <= 0) return;
        for (auto& row : mSlots) {
            for (auto& slot : row) {
                if (slot.has_value() && slot->getName() == newItem.getName()) {
                    slot = newItem;
                    return;
                }
            }
        }
    }

    // Jeśli przedmiot nie jest stackowalny lub nie znajduje się jeszcze w ekwipunku
    // Szukamy pierwszego wolnego slotu
    for (auto& row : mSlots) {
        for (auto& slot : row) {
            if (!slot.has_value()) {
                slot = newItem; // Umieszczamy przedmiot w wolnym slocie
                return;
            }
        }
    }

    // Jeśli ekwipunek jest pełny
    std::cerr << "Inventory is full! Cannot add item: " << newItem.getName() << std::endl;
}

void Inventory::drawInventory() {
    struct DragPayload {
        int sourceRow;
        int sourceCol;
    };
    if (!mInventoryOpen) return;
    sf::Vector2u windowSize = window.getSize();
    unsigned int windowWidth = windowSize.x; // Szerokość okna
    unsigned int windowHeight = windowSize.y; // Wysokość okna
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    float inventoryWidth = 875.0f;
    float inventoryHeight = 430.0f;

    float inventoryX = (windowWidth - inventoryWidth) / 2.0f;
    float inventoryY = (windowHeight - inventoryHeight) / 2.0f;

    ImGui::SetNextWindowPos(ImVec2(inventoryX, inventoryY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(inventoryWidth, inventoryHeight));
    ImGui::Begin("Inventory", &mInventoryOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);


    const float spacing = 5.0f;
    float slotSize = 90.0f;

    // Przechodzimy przez wszystkie sloty ekwipunku
for (int row = 0; row < mRows; ++row) {
    for (int col = 0; col < mColumns; ++col) {
        ImGui::PushID(row * mColumns + col);
        auto& item = mSlots[row][col];
        ImVec2 slotPos = ImGui::GetCursorScreenPos();
        if(row == mRows - 1 && col == 0) {
            ImGui::SetCursorScreenPos(ImVec2(slotPos.x, slotPos.y + 35));
        }

        // Wyświetlanie przedmiotu lub pustego slotu
        if (item.has_value()) {
            auto texID = reinterpret_cast<ImTextureID>(item->getTexture().getNativeHandle());
            // Rysowanie obrazka przedmiotu w obrębie slotu
            ImGui::Image(texID, ImVec2(slotSize, slotSize));

            // Rozpoczęcie przeciągania (drag source)
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                DragPayload payload = { row, col };
                ImGui::SetDragDropPayload("ITEM_SLOT", &payload, sizeof(payload));

                // Wyświetlamy obrazek w mniejszym rozmiarze podczas przeciągania
                ImGui::Image(texID, ImVec2(slotSize * 0.8f, slotSize * 0.8f));

                // Jeśli przedmiot jest stackowalny, wyświetlamy ilość
                if (item->isStackable()) {
                    ImVec2 cursorPos = ImGui::GetCursorPos(); // Zapisz bieżącą pozycję kursora
                    // Przesunięcie tekstu z ilością przedmiotu na dole slotu
                    ImGui::SetCursorPos(ImVec2(cursorPos.x + 50, cursorPos.y - 15)); // Przesunięcie w dół
                    ImGui::Text("%d", item->getCount()); // Wyświetlanie ilości
                }
                ImGui::EndDragDropSource();
            }

            // Jeśli przedmiot jest stackowalny, wyświetlamy ilość w normalnym stanie
            if (item->isStackable()) {
                ImVec2 textPos;
                if(row == mRows - 1 && col == 0)  textPos = ImVec2(slotPos.x + slotSize - 15, slotPos.y + slotSize + 20);
                else textPos = ImVec2(slotPos.x + slotSize - 15, slotPos.y + slotSize - 15);
                std::string countText = std::to_string(item->getCount());
                ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), countText.c_str());
            }
        } else {
            // Jeśli slot jest pusty, wyświetlamy pusty przycisk
            ImGui::Button(" ", ImVec2(slotSize, slotSize));
        }

        // Obsługa celu upuszczania
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM_SLOT")) {
                const auto* draggedPayload = static_cast<const DragPayload*>(payload->Data);
                int sourceRow = draggedPayload->sourceRow;
                int sourceCol = draggedPayload->sourceCol;

                if (mSlots[row][col].has_value()) {
                    std::swap(mSlots[row][col], mSlots[sourceRow][sourceCol]);
                } else {
                    mSlots[row][col] = std::move(mSlots[sourceRow][sourceCol]);
                    mSlots[sourceRow][sourceCol] = std::nullopt;
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
        // Odstęp między slotami w tej samej linii
        if (col < mColumns - 1) {
            ImGui::SameLine(0.0f, spacing+1);
        }
    }
}
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);
}

void Inventory::drawHotbarOnScreen() {
    if (mInventoryOpen) return;
    const float slotSize = 90.0f;
    const float spacing = 5.0f;
    float hotbarWidth = 870.0f; // Szerokość hotbara (dopasowana do ekwipunku)
    float hotbarHeight = 105.0f; // Wysokość hotbara (przykładowa)
    sf::Vector2u windowSize = window.getSize();
    unsigned int windowWidth = windowSize.x; // Szerokość okna
    unsigned int windowHeight = windowSize.y; // Wysokość okna
    float hotbarX = (windowWidth - hotbarWidth) / 2.0f;
    float hotbarY = windowHeight - hotbarHeight - 20.0f;
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::SetNextWindowPos(ImVec2(hotbarX, hotbarY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(hotbarWidth, hotbarHeight));



    ImGui::Begin("Hotbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    for (int col = 0; col < mColumns; ++col) {
        ImGui::PushID(col);

        auto& item = mSlots[mRows - 1][col];
        ImVec2 slotPos = ImGui::GetCursorScreenPos();
        if (item.has_value()) {
            auto texID = reinterpret_cast<ImTextureID>(item->getTexture().getNativeHandle());

            // Podświetlenie aktywnego slotu
            if (col == mSelectedHotbarSlot) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
                ImGui::ImageButton(texID, ImVec2(slotSize, slotSize));
                ImGui::PopStyleColor();
            } else {
                ImGui::ImageButton(texID, ImVec2(slotSize, slotSize));
            }
            if (item->isStackable()) {
                ImVec2 textPos = ImVec2(slotPos.x + slotSize - 15, slotPos.y + slotSize - 15);
                std::string countText = std::to_string(item->getCount());
                ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), countText.c_str());
            }
        } else {
            if (col == mSelectedHotbarSlot) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
                ImGui::Button("", ImVec2(slotSize, slotSize));
                ImGui::PopStyleColor();
            } else {
                ImGui::Button("", ImVec2(slotSize, slotSize));
            }
        }

        ImGui::PopID();

        if (col < mColumns - 1) {
            ImGui::SameLine(0.0f, spacing);
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void Inventory::updateHotbarSelection(int i) {
            mSelectedHotbarSlot = i; // Zmień aktywny slot
}


