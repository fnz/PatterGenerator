#include "AppDelegate.h"

#include "ConfirmationWindow.h"
#include "UIConstants.h"
#include "SoundManager.h"
#include "SaveManager.h"
#include "Generator.h"
#include "WindowManager.h"
#include "MainWindow.h"
#include "TestWindow.h"
#include "ObserverManager.h"
#include "SystemProtocol.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages,
// don't modify or remove this function
static int register_all_packages() {
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
//		glview = GLViewImpl::createWithRect("Pattern Generator", Rect(0, 0, 1280, 800));
//		glview = GLViewImpl::createWithRect("Pattern Generator", Rect(0, 0, 960, 600));
//		glview = GLViewImpl::createWithRect("Pattern Generator", Rect(0, 0, 960, 540));
		glview = GLViewImpl::createWithRect("Pattern Generator", Rect(0, 0, 800, 480));
//		glview = GLViewImpl::createWithRect("Pattern Generator", Rect(0, 0, 1024, 768));
#else
        glview = GLViewImpl::create("Pattern Generator");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();
	
    float designHeight = BIG_MED_LOW(1536, 768, 768);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1, designHeight, ResolutionPolicy::FIXED_HEIGHT);
    FileUtils::getInstance()->addSearchPath("Resources/");

    UIConstants::init();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(std::string(BIG_MED_LOW("big", "med", "med")) + "/Interface.xml");
    SoundManager::getInstance()->preload();
    SoundManager::getInstance()->start();

    SaveManager::getInstance()->load();

    Generator::initWeights();

    // create a scene. it's an autorelease object
    auto scene = Scene::create();
    director->runWithScene(scene);

    WindowManager::getInstance()->setParentNode(scene, 100, 110);
    WindowManager::getInstance()->show<MainWindow>();

    int runs = UserDefault::getInstance()->getIntegerForKey("runs", 0);
    UserDefault::getInstance()->setIntegerForKey("runs", ++runs);

    if (!UserDefault::getInstance()->getBoolForKey("rate_shown") && runs > 2) {
        UserDefault::getInstance()->setBoolForKey("rate_shown", true);
        WindowManager::getInstance()->show<ConfirmationWindow>("If you use this app for practice and you like it, would you give it a review?", [](bool answer) {
            if (answer) {
                GameUtils::rate();
            }
        });
    }

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    ObserverManager::notify(&SystemProtocol::applicationDidEnterBackground);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    ObserverManager::notify(&SystemProtocol::applicationWillEnterForeground);
}
