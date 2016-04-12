#include "SaveManager.h"

#include "cocos2d.h"

#include "AndroidCpp.h"

using namespace cocos2d;

std::pair<std::string, std::string> SaveManager::getKeys(const int i) const {
    std::pair<std::string, std::string> ret;
    ret.first = "pattern_" + std::to_string(i);
    ret.second = "orchestration_" + std::to_string(i);
    return ret;
}

void SaveManager::save() const {
    UserDefault::getInstance()->setIntegerForKey("patterns_count", (int)patterns.size());
    for (int i = 0; i < patterns.size(); i++) {
        auto keys = getKeys(i);
        UserDefault::getInstance()->setStringForKey(keys.first.c_str(), patterns[i].first);
        UserDefault::getInstance()->setStringForKey(keys.second.c_str(), patterns[i].second);
    }
}

void SaveManager::load() {
    int size = UserDefault::getInstance()->getIntegerForKey("patterns_count");
    patterns.clear();
    for (int i = 0; i < size; i++) {
        auto keys = getKeys(i);
        auto pattern = UserDefault::getInstance()->getStringForKey(keys.first.c_str());
        auto orchestration = UserDefault::getInstance()->getStringForKey(keys.second.c_str());
        patterns.push_back(std::make_pair(pattern, orchestration));
    }
}

std::vector<std::pair<std::string, std::string>> SaveManager::get() const {
    return patterns;
}

int SaveManager::add(const std::string& pattern, const std::string& orchestration) {
    for (int i = 0; i < patterns.size(); i++) {
        if (patterns[i].first == pattern && patterns[i].second == orchestration) {
            return i + 1;
        }
    }
    patterns.push_back(std::make_pair(pattern, orchestration));
    return 0;
}

void SaveManager::remove(int pos) {
    if (pos < 0 || pos > patterns.size()) {
        return;
    }
    patterns.erase(patterns.begin() + pos);
    save();
}