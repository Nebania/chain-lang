#include "../include/link_audio.h"
#include <raylib.h>
#include <iostream>
#include <unordered_map>
#include <cmath> 

namespace SysAudio {
    
    static Music currentMusic;
    static bool isAudioInitialized = false;
    static bool isMusicLoaded = false;
    static std::unordered_map<std::string, Sound> soundRegistry;

    static float audio_buffer[256] = {0};
    static float smoothed_bands[5] = {0};

    void AudioProcessCallback(void *buffer, unsigned int frames) {
        float *samples = (float *)buffer;
        int channels = currentMusic.stream.channels;
        if (channels <= 0) channels = 2; // Fallback aman

        for (unsigned int i = 0; i < 256 && i < frames; i++) {
            audio_buffer[i] = samples[i * channels]; 
        }
    }

    void init() {
        if (!isAudioInitialized) {
            InitAudioDevice();
            isAudioInitialized = true;
        }
    }

    void close() {
        for (auto const& [name, sfx] : soundRegistry) UnloadSound(sfx);
        soundRegistry.clear();
        
        if (isMusicLoaded) {
            DetachAudioStreamProcessor(currentMusic.stream, AudioProcessCallback);
            UnloadMusicStream(currentMusic);
            isMusicLoaded = false;
        }
        if (isAudioInitialized) {
            CloseAudioDevice();
            isAudioInitialized = false;
        }
    }

    bool play(const std::string& path) {
        if (!isAudioInitialized) init();
        if (isMusicLoaded) {
            DetachAudioStreamProcessor(currentMusic.stream, AudioProcessCallback);
            UnloadMusicStream(currentMusic);
            isMusicLoaded = false;
        }

        currentMusic = LoadMusicStream(path.c_str());
        if (currentMusic.ctxData != nullptr) {
            PlayMusicStream(currentMusic);
            AttachAudioStreamProcessor(currentMusic.stream, AudioProcessCallback);
            isMusicLoaded = true;
            return true;
        }
        return false;
    }

    // ==========================================
    // UPDATE & KALKULASI FFT (Persis seperti kodemu)
    // ==========================================
    void update() { 
        if (!isMusicLoaded) return;
        UpdateMusicStream(currentMusic); 

        if (IsMusicStreamPlaying(currentMusic)) {
            float spectrum[128] = {0};
            float raw_bands[5] = {0};

            for (int k = 1; k < 128; k++) {
                float re = 0, im = 0;
                for (int n = 0; n < 256; n++) {
                    float angle = 2.0f * PI * k * n / 256.0f;
                    re += audio_buffer[n] * cosf(angle);
                    im -= audio_buffer[n] * sinf(angle);
                }
                spectrum[k] = sqrtf(re * re + im * im);
            }

            int bandLimits[6] = {1, 4, 10, 26, 61, 128}; 
            for (int b = 0; b < 5; b++) {
                float sum = 0;
                int count = bandLimits[b+1] - bandLimits[b];
                for (int k = bandLimits[b]; k < bandLimits[b+1]; k++) {
                    sum += spectrum[k];
                }
                raw_bands[b] = sum / count; 
                
                if (b == 2) raw_bands[b] *= 1.5f;
                if (b == 3) raw_bands[b] *= 2.0f;
                if (b == 4) raw_bands[b] *= 3.0f;
            }

            for (int i = 0; i < 5; i++) {
                smoothed_bands[i] += (raw_bands[i] - smoothed_bands[i]) * 0.2f;
            }
        }
    }

    float getSpectrum(int band) {
        if (band < 0 || band > 4) return 0.0f;
        return smoothed_bands[band];
    }

    void pause() { if (isMusicLoaded) PauseMusicStream(currentMusic); }
    void resume() { if (isMusicLoaded) ResumeMusicStream(currentMusic); }
    void stop() { if (isMusicLoaded) StopMusicStream(currentMusic); }
    void setVolume(float volume) { if (isMusicLoaded) SetMusicVolume(currentMusic, volume); }
    float getTimeLength() { return isMusicLoaded ? GetMusicTimeLength(currentMusic) : 0.0f; }
    float getTimePlayed() { return isMusicLoaded ? GetMusicTimePlayed(currentMusic) : 0.0f; }
    void seek(float time) { if (isMusicLoaded) SeekMusicStream(currentMusic, time); }
    bool isPlaying() { return isMusicLoaded ? IsMusicStreamPlaying(currentMusic) : false; }
    void loadSound(const std::string& name, const std::string& path) { if (!isAudioInitialized) init(); Sound sfx = LoadSound(path.c_str()); soundRegistry[name] = sfx; }
    void playSound(const std::string& name) { if (soundRegistry.find(name) != soundRegistry.end()) { PlaySound(soundRegistry[name]); } }
    void unloadSound(const std::string& name) { if (soundRegistry.find(name) != soundRegistry.end()) { UnloadSound(soundRegistry[name]); soundRegistry.erase(name); } }
}