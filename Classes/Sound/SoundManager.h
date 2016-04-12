#pragma once

#include "cocos2d.h"
#include "Singleton.h"
#include <functional>
#include <atomic>

class SoundManager : public Singleton<SoundManager> {
    friend class Singleton<SoundManager>;

public:
    void preload();
    void start();

    void play(const std::string& orchestration, bool once = false);
    bool isPlaying();
    void stop();

    void setDelay(float delay);
    void setHighlightCallback(const std::function<void(int)>& highlight);

private:
    std::vector<std::string> getSequence(const std::string& orchestration);

    float timer = 0.0;

    std::string ghostSound;
    std::string snareSound;
    std::string kickSound;
    std::string tomSound;
    std::string hatSound;

    std::atomic<float> delay;
    std::atomic<int> pos;
    std::atomic<bool> playing;
    std::atomic<bool> once;
    std::vector<std::string> sequence;
    std::mutex sequenceMutex;

    std::function<void(int)> highlight;
};