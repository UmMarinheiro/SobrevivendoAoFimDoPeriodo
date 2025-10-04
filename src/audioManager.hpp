#pragma once
#include <string>
#include <map>
#include <unistd.h>

class AudioManager {
private:
    std::map<std::string, std::string> audioFiles;
    bool audioEnabled;
    int soundVolume = 50; 

public:
    AudioManager();
    ~AudioManager();
    
    void loadAudio(const std::string& name, const std::string& filePath);
    void playAudio(const std::string& audioName, bool async = true);
    void stopAudio();
    void setAudioEnabled(bool enabled);
    bool isAudioEnabled() const;
    void setSoundVolume(int volume);
    int getSoundVolume() const;
};