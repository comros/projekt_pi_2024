#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class Audio {
public:

    void loadSound(const std::string& id, const std::string& filePath);
    void playSound(const std::string& id);
    void stopSound(const std::string& id);
    void setPitch(const std::string& id, float pitch);
    void setGlobalVolume(float volume);


private:
    std::unordered_map<std::string, sf::SoundBuffer> mSoundBuffers;
    std::unordered_map<std::string, sf::Sound> mSounds;
    float mVolume = 50.0f;  // default volume
};


#endif //AUDIO_HPP
