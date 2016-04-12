#include "SoundManager.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace cocos2d;
using namespace experimental;
using namespace CocosDenshion;
using namespace std::chrono;

#ifdef ANDROID
const std::string ext = "ogg";
#else
const std::string ext = "wav";
#endif

void SoundManager::preload() {
    ghostSound = "sounds/" + ext + "/ghost." + ext;
    snareSound = "sounds/" + ext + "/snare." + ext;
    kickSound = "sounds/" + ext + "/kick." + ext;
    tomSound = "sounds/" + ext + "/tom." + ext;
    hatSound = "sounds/" + ext + "/hat." + ext;

    std::vector<std::string> sounds = {ghostSound, snareSound, kickSound, tomSound, hatSound};
    for (const auto& s : sounds) {
        SimpleAudioEngine::getInstance()->preloadEffect(s.c_str());
    }
}

void SoundManager::start() {
    delay = 0.05;
    playing = false;

    sequenceMutex.lock();
    pos = 0;
    sequence = {hatSound};
    sequenceMutex.unlock();

    auto soundThread = std::thread([this]() {
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		static double ts1 = (tv.tv_sec) + (tv.tv_usec) * 0.000001;
		
		while(true) {
			gettimeofday(&tv, nullptr);
			double ts2 = (tv.tv_sec) + (tv.tv_usec) * 0.000001;
			double dt = ts2 - ts1;
			ts1 = ts2;
			
			timer += dt;
			if (timer >= delay) {
				timer -= delay;
				if (playing && !sequence.empty()) {
					if (highlight) {
						Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(highlight, pos%sequence.size()));
					}
					
					sequenceMutex.lock();
					std::string sound = sequence[pos%sequence.size()];
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([sound](){
						SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
					});
				
					pos++;
					if (pos >= sequence.size() && once == true) {
						playing = false;
						pos = 0;
					}
					sequenceMutex.unlock();
				}
			}
	
			std::chrono::nanoseconds pause(50);
			std::this_thread::sleep_for(pause);
		}
    });
    soundThread.detach();
}

std::vector<std::string> SoundManager::getSequence(const std::string& orchestration) {
    std::vector<std::string> sequence;
    for (int i = 0; i < orchestration.length(); i++) {
        std::string sound;
        switch (orchestration[i]) {
            case 's':
                sound = ghostSound;
                break;
            case 'T':
                sound = tomSound;
                break;
            case 'S':
                sound = snareSound;
                break;
            case 'K':
                sound = kickSound;
                break;
            case 'H':
                sound = hatSound;
                break;
            default:
                break;
        }
        sequence.push_back(sound);
    }

    return sequence;
}

void SoundManager::play(const std::string& orchestration, bool once) {
    if (playing) {
        CCLOGERROR("%s", "SoundManager needs to be stopped first");
        return;
    }
    sequenceMutex.lock();
    sequence = getSequence(orchestration);
    sequenceMutex.unlock();
    pos = 0;
    SoundManager::once = once;
    playing = true;
}

bool SoundManager::isPlaying() {
    return playing;
}

void SoundManager::stop() {
    playing = false;
}

void SoundManager::setDelay(float delay) {
    SoundManager::delay = delay;
}

void SoundManager::setHighlightCallback(const std::function<void(int)>& highlight) {
    SoundManager::highlight = highlight;
}