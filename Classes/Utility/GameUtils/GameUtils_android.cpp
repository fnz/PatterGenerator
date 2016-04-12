#include "GameUtils.h"

#include "EasyJNI.h"

void GameUtils::composeLetter(const std::string& to, const std::string& theme) {
    easyjni::callVoidMethod("com/vk/utils/Utils", "composeLetter", to, theme);
}

void GameUtils::share(const std::string& title, const std::string& text, const std::string& picture) {
	easyjni::callVoidMethod("com/vk/utils/Utils", "share", title, text, picture);
}

std::string GameUtils::getExternalStoragePath() {
	return easyjni::callStringMethod("com/vk/utils/Utils", "getExternalStoragePath");
}

void GameUtils::rate() {
	easyjni::callVoidMethod("com/vk/utils/Utils", "rate");
}