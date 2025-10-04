#include "audioManager.hpp"
#include <iostream>
#include <cstdlib>

AudioManager::AudioManager() : audioEnabled(true) {}

AudioManager::~AudioManager() {
    stopAudio();
}

void AudioManager::loadAudio(const std::string& name, const std::string& filePath) {
    audioFiles[name] = filePath;
}

void AudioManager::playAudio(const std::string& audioName, bool async) {
    if (!audioEnabled || audioFiles.find(audioName) == audioFiles.end()) {
        return;
    }
    
    std::string filePath = audioFiles[audioName];
    std::string command;

    //command = "aplay -q \"" + filePath + "\" &";
    command = "paplay -q \"" + filePath + "\" &";

    if (async) {
        command += " &"; 
    }
    
    int result = system(command.c_str());
    if (result != 0) {
        std::cout << "Erro ao reproduzir audio: " << audioName << std::endl;
    }
}

void AudioManager::stopAudio() {
    system("pkill aplay 2>/dev/null");
}

void AudioManager::setAudioEnabled(bool enabled) {
    audioEnabled = enabled;
    if (!enabled) {
        stopAudio();
    }
}

bool AudioManager::isAudioEnabled() const {
    return audioEnabled;
}
void AudioManager::setSoundVolume(int volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    soundVolume = volume;
    std::string command = "pactl set-sink-volume @DEFAULT_SINK@ " + std::to_string(volume) + "%";
    system(command.c_str());
}
int AudioManager::getSoundVolume() const {
    return soundVolume;
}