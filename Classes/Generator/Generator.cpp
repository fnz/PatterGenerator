#include "Generator.h"

#include <algorithm>

#pragma mark - Weights

typedef std::map<std::string, float> Weights;
Weights sw;
Weights Rw;
Weights rw;
Weights rrw;
Weights Lw;
Weights lw;
Weights llw;
Weights Kw;
Weights KKw;

Weights orchestrationWeights;

void Generator::initWeights() {
    sw["R"] = 10;
    sw["K"] = 10;

    Kw["r"] = 7;
    Kw["R"] = 10;
    Kw["l"] = 7;
    Kw["L"] = 10;
    Kw["K"] = 10;

    KKw["r"] = 5;
    KKw["R"] = 10;
    KKw["l"] = 5;
    KKw["L"] = 10;

    Rw["l"] = 5;
    Rw["L"] = 10;
    Rw["K"] = 10;

    rw["r"] = 7;
    rw["l"] = 5;
    rw["L"] = 10;
    rw["K"] = 10;

    rrw["l"] = 3;
    rrw["L"] = 10;
    rrw["K"] = 10;

    Lw["r"] = 5;
    Lw["R"] = 10;
    Lw["K"] = 10;

    lw["r"] = 5;
    lw["R"] = 10;
    lw["l"] = 7;
    lw["K"] = 10;

    llw["r"] = 3;
    llw["R"] = 10;
    llw["K"] = 10;

    orchestrationWeights["H"] = 7;
    orchestrationWeights["S"] = 10;
    orchestrationWeights["T"] = 10;
}

#pragma mark - Aux

int Generator::getBackCount(const std::string& pattern, const std::string& note) {
    if (note.length() != 1) {
        return 0;
    }
    int ret = 0;
    for (long i = pattern.size() - 1; i >= 0; i--) {
        if (pattern[i] == note[0]) {
            ret++;
        } else {
            return ret;
        }
    }
    return ret;
}

#pragma mark - Pattern

std::string Generator::generatePattern(const int length) {
    if (length == 0) {
        return "";
    }

    std::string ret = pick(sw);
    while (true) {
        if (getBackCount(ret, "K") == 1) {
            ret += pick(Kw);
        } else if (getBackCount(ret, "K") == 2) {
            ret += pick(KKw);
        } else if (getBackCount(ret, "R") == 1) {
            ret += pick(Rw);
        } else if (getBackCount(ret, "r") == 1) {
            ret += pick(rw);
        } else if (getBackCount(ret, "r") == 2) {
            ret += pick(rrw);
        } else if (getBackCount(ret, "L") == 1) {
            ret += pick(Lw);
        } else if (getBackCount(ret, "l") == 1) {
            ret += pick(lw);
        } else if (getBackCount(ret, "l") == 2) {
            ret += pick(llw);
        } else {
            break;
        }

        if (ret[0] == 'K' && !checkTriple(ret)) {
            ret = pick(sw);
            continue;
        }

        if (!checkDoubleAccent(ret)) {
            ret = pick(sw);
            continue;
        }

        if (ret.length() == length) {
            break;
        }
    }

    return ret;
}

std::string Generator::convertToDisplay(const std::string& pattern) {
	std::string ret;
	for (int i = 0; i < pattern.length(); i++) {
		char note = pattern[i];
		if (note == 'l') {
			note = 'L';
		}
		if (note == 'r') {
			note = 'R';
		}
		ret.push_back(note);
	}
	return ret;
}

bool Generator::checkTriple(const std::string& pattern) {
    if (pattern.length() < 3) {
        return true;
    }
    std::string str = pattern;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    str = str + str;
    for (int i = 0; i < pattern.length(); i++) {
        if (str[i] == str[i + 1] && str[i + 1] == str[i + 2]) {
            return false;
        }
    }

    return true;
}

bool Generator::checkDoubleAccent(const std::string& pattern) {
    if (pattern.length() < 2) {
        return true;
    }
    std::string str1, str2;
    str1 = str2 = pattern;
    std::transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
    for (int i = 0; i < pattern.length() - 1; i++) {
        if (str1[i] != str1[i + 1] || str1[i] == 'K') {
            continue;
        }
        if (str2[i] == str2[i + 1] && toupper(str2[i]) != str2[i]) {
            continue;
        }
        return false;
    }
    return true;
}

bool Generator::checkEnds(const std::string& pattern) {
    if (pattern.length() < 3) {
        return true;
    }
    std::string str = pattern;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    if (str[0] == 'K') {
        return true;
    }
    if (str[0] == str[str.length() - 1]) {
        return false;
    }
    return true;
}

#pragma mark - Orchestration

std::string Generator::generateOrchestration(const std::string& pattern) {
    std::string orchestration = "";
    for (int i = 0; i < pattern.length(); i++) {
        if (pattern[i] == 'R' || pattern[i] == 'L') {
            orchestration += pick(orchestrationWeights);
        } else if (pattern[i] == 'r' || pattern[i] == 'l') {
            orchestration += "s";
        } else {
            orchestration += "K";
        }
    }

    return orchestration;
}
