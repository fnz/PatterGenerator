#pragma once

#include <vector>
#include <string>
#include <map>
#include <cstdlib>

class Generator {
public:
    static void initWeights();
    static std::string generatePattern(const int length);
    static std::string generateOrchestration(const std::string& pattern);
	static std::string convertToDisplay(const std::string& pattern);

    static int getBackCount(const std::string& pattern, const std::string& note);
    static bool checkTriple(const std::string& pattern);
    static bool checkDoubleAccent(const std::string& pattern);
    static bool checkEnds(const std::string& pattern);
};

template <typename T>
T pick(const std::map<T, float>& weights) {
    float sum = 0.0;
    std::vector<float> steps;
    std::vector<T> ret;
    for (const auto& w : weights) {
        sum += w.second;
        ret.push_back(w.first);
        steps.push_back(sum);
    }
    static int max = 10000000;
    float p = sum * float(rand() % max) / float(max);
    for (int i = 0; i < steps.size(); i++) {
        if (p < steps[i]) {
            return ret[i];
        }
    }
    return T();
}