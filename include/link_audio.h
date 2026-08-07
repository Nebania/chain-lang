#pragma once
#include <string>

namespace SysAudio {
    void init();
    void close();
    
    bool play(const std::string& path); // Load and play at the same time
    void pause();
    void resume();
    void stop();
    void update(); // Must be called inside the while loop
    float getEQBand(int bandIndex);
    
    // Add this for sound effects
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