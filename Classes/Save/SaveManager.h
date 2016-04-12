#pragma once

#include "Singleton.h"

#include <vector>
#include <string>

class SaveManager : public Singleton<SaveManager> {
    friend class Singleton<SaveManager>;

public:
    void save() const;
    void load();
    std::vector<std::pair<std::string, std::string>> get() const;
    int add(const std::string& pattern, const std::string& orchestration);
    void remove(int pos);

protected:
    std::pair<std::string, std::string> getKeys(const int i) const;
    std::vector<std::pair<std::string, std::string>> patterns;
};