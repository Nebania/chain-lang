#pragma once
#include <string>

namespace SysAudio {
    void init();
    void close();
    
    bool play(const std::string& path); 
    void pause();
    void resume();
    void stop();
    void update(); 
    float getEQBand(int bandIndex);
  
    void loadSound(const std::string& name, const std::string& path);
    void playSound(const std::string& name);
    void unloadSound(const std::string& name);
    
    void setVolume(float volume);
    float getTimeLength();
    float getTimePlayed();
    void seek(float time);
    bool isPlaying();
    float getSpectrum(int band); 
}