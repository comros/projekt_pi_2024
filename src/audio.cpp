#include "../headers/audio.hpp"


void Audio::loadSound(const std::string& id, const std::string& filePath) {
    sf::SoundBuffer buffer;
    if (buffer.loadFromFile(filePath)) {
        mSoundBuffers[id] = buffer;   // Store Buffer
        mSounds[id].setBuffer(mSoundBuffers[id]); // Set buffer to sf::Sound
        mSounds[id].setVolume(mVolume);
        mSounds[id].setLoop(true);
    }
}

void Audio::playSound(const std::string& id) {
    if (mSounds.count(id) && mSounds[id].getStatus() != sf::Sound::Playing) {
        mSounds[id].play();
    }
}

void Audio::stopSound(const std::string& id) {
    if (mSounds.count(id) && mSounds[id].getStatus() == sf::Sound::Playing) {
        mSounds[id].stop();
    }
}

void Audio::setPitch(const std::string& id, float pitch) {
    if (mSounds.count(id)) {
        mSounds[id].setPitch(pitch);
    }
}

void Audio::setGlobalVolume(float volume) {
    mVolume = volume;
    for (auto& sound : mSounds) {
        sound.second.setVolume(volume);
    }
}

