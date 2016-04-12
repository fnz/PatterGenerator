LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
			../../Classes/Tests/gtest/src/gtest-all.cc \
			../../Classes/AppDelegate.cpp \
			../../Classes/Generator/Generator.cpp \
			../../Classes/Generator/GeneratorTests.cpp \
			../../Classes/Main/AboutWindow.cpp \
			../../Classes/Main/ListWindow.cpp \
			../../Classes/Main/MainWindow.cpp \
			../../Classes/Save/SaveManager.cpp \
			../../Classes/Sound/SoundManager.cpp \
			../../Classes/Tests/TestWindow.cpp \
			../../Classes/UI/Buttons/ButtonBase.cpp \
			../../Classes/UI/Buttons/IconButton.cpp \
			../../Classes/UI/Buttons/SquareButton.cpp \
			../../Classes/UI/Buttons/TextButton.cpp \
			../../Classes/UI/Constants/UIConstants.cpp \
			../../Classes/UI/Scroll/Scroll.cpp \
			../../Classes/UI/Windows/ConfirmationWindow.cpp \
			../../Classes/UI/Windows/FullscreenWindow.cpp \
			../../Classes/UI/Windows/MessageWindow.cpp \
			../../Classes/UI/Windows/PopupWindow.cpp \
			../../Classes/UI/Windows/Window.cpp \
			../../Classes/UI/Windows/WindowManager.cpp \
			../../Classes/Utility/Android/AndroidCpp.cpp \
			../../Classes/Utility/EasyJNI/EasyJNI.cpp \
			../../Classes/Utility/GameUtils/GameUtils.cpp \
			../../Classes/Utility/Patterns/ObserverManager/BaseObserverProtocol.cpp \
			../../Classes/Utility/Patterns/ObserverManager/ObserverManager.cpp \
			../../Classes/Utility/GameUtils/GameUtils_android.cpp \

LOCAL_C_INCLUDES := \
			$(LOCAL_PATH)/../../Classes/Tests/gtest/ \
			$(LOCAL_PATH)/../../Classes/Tests/gtest/include \
			$(LOCAL_PATH)/../../Classes \
			$(LOCAL_PATH)/../../Classes/Generator \
			$(LOCAL_PATH)/../../Classes/Main \
			$(LOCAL_PATH)/../../Classes/Save \
			$(LOCAL_PATH)/../../Classes/Sound \
			$(LOCAL_PATH)/../../Classes/Tests \
			$(LOCAL_PATH)/../../Classes/UI/Buttons \
			$(LOCAL_PATH)/../../Classes/UI/Constants \
			$(LOCAL_PATH)/../../Classes/UI/Scroll \
			$(LOCAL_PATH)/../../Classes/UI/Windows \
			$(LOCAL_PATH)/../../Classes/Utility/Android \
			$(LOCAL_PATH)/../../Classes/Utility/EasyJNI \
			$(LOCAL_PATH)/../../Classes/Utility/GameUtils \
			$(LOCAL_PATH)/../../Classes/Utility/Patterns/ObserverManager \
			$(LOCAL_PATH)/../../Classes/Utility/Patterns/Singleton \
			$(LOCAL_PATH)/../../Classes/Utility/Synthesize \
			$(LOCAL_PATH)/../../Classes/Utility/System \


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
