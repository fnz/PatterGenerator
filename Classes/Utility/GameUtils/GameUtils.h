#pragma once

#include <string>

namespace cocos2d {
class Node;
}

class GameUtils {
public:
    static void composeLetter(const std::string& to, const std::string& subj);
	static void share(const std::string& title, const std::string& text, const std::string& picture);
	static void rate();

    static void setCascade(cocos2d::Node* node, bool value);
    static std::string format(const std::string fmt_str, ...);
	
    static int getDeviceType();
	
	static std::string getExternalStoragePath();
};
