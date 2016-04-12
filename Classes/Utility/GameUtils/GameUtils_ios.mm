#include "GameUtils.h"

#include "cocos2d.h"
#include "AppController.h"

using namespace cocos2d;

void GameUtils::composeLetter(const std::string& to, const std::string& subj) {
    std::string escapedSubj = [[[NSString stringWithUTF8String:subj.c_str()] stringByReplacingOccurrencesOfString:@" " withString:@"%20"] UTF8String];
    std::string url = "mailto:" + to + "?subject=" + escapedSubj;
    Application::getInstance()->openURL(url);
}

void GameUtils::share(const std::string& title, const std::string& text, const std::string& picture) {
	
}

std::string GameUtils::getExternalStoragePath() {
	return cocos2d::FileUtils::getInstance()->getWritablePath();
}

void GameUtils::rate() {

}