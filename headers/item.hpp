#ifndef ITEM_HPP
#define ITEM_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <optional> // Dla dynamicznej obsługi ilości



class Item {
public:
    // Konstruktor dla przedmiotu z liczbą sztuk (stackable)
    Item(const std::string& name, const std::string& texturePath, int count)
        : mName(name), mStackable(true), mCount(count)
    {
        loadTexture(texturePath);
    }

    // Konstruktor dla przedmiotu niestackowalnego (np. narzędzia)
    Item(const std::string& name, const std::string& texturePath)
        : mName(name), mStackable(false), mCount(std::nullopt)
    {
        loadTexture(texturePath);
    }

    // Funkcja zwracająca nazwę przedmiotu
    std::string getName() const { return mName; }

    // Funkcja zwracająca sprite przedmiotu (można rysować)
    sf::Sprite& getSprite() { return mSprite; }

    // Funkcja zwracająca teksturę przedmiotu
    const sf::Texture& getTexture() const {
        return mTexture;
    }

    // Sprawdza, czy przedmiot może być stackowany
    bool isStackable() const { return mStackable; }

    // Zwraca liczbę sztuk (jeśli dotyczy)
    int getCount() const {
        return mStackable ? mCount.value_or(1) : 1;
    }

    // Ustawia liczbę sztuk (tylko dla stackowalnych przedmiotów)
    void setCount(int count) {
        if (mStackable) {
            mCount = count;
        }
    }

    // Zwiększa liczbę sztuk
    void incrementCount(int amount = 1) {
        if (mStackable) {
            //std::cout<<"Current count: "<<mCount.value()<<", Adding: "<<amount<<std::endl;
            mCount = mCount.value_or(0) + amount;
        }
    }

    // Zmniejsza liczbę sztuk
    void decrementCount(int amount = 1) {
        if (mStackable) {
            mCount = std::max(0, mCount.value_or(0) - amount);
        }
    }

private:
    std::string mName;                     // Nazwa przedmiotu
    sf::Texture mTexture;                  // Tekstura przedmiotu
    sf::Sprite mSprite;                    // Sprite do rysowania
    bool mStackable;                       // Czy przedmiot może być stosowany w stosy
    std::optional<int> mCount;             // Liczba sztuk (dla stackowalnych przedmiotów)

    // Funkcja pomocnicza do ładowania tekstury
    void loadTexture(const std::string& texturePath) {
        if (!mTexture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture for " << mName << std::endl;
        }
        mSprite.setTexture(mTexture);
    }
};

#endif // ITEM_HPP