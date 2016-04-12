#pragma once

#include "cocos2d.h"
#include "FullscreenWindow.h"
#include "SystemProtocol.h"

#include <vector>
#include <unordered_map>

class SquareButton;

class MainWindow : public FullscreenWindow, public SystemProtocol {
public:
    virtual bool initWindow() override;
    virtual void onBack() override;

protected:
    virtual void applicationDidEnterBackground() override;

private:
	cocos2d::Sprite* back;
    cocos2d::Size screenSize;
    void createUI();
	void createStaff();

    void setPattern(const std::string& pattern, const std::string& orchestration, bool silent = false);
    void showPattern();
    void showOrchestration();
	
	float pad;
	float buttonWidth;
	float buttonHeight;
	float lineY;
	
	std::vector<Node*> topLines;

    std::string patternStr;
    std::string orchestrationStr;
    int patternFontSize;
    float patternY;
    float orchestrationY;
    std::vector<float> positions;
    std::vector<cocos2d::Node*> notes;
    std::vector<cocos2d::Node*> drums;
	std::unordered_map<std::string, float> notePositions;
	std::unordered_map<std::string, std::string> noteImages;

    void processTouch(const cocos2d::Vec2& touchLocation);
    cocos2d::Node* getClosest(const cocos2d::Vec2& point, const std::vector<cocos2d::Node*>& objs);
    void changePattern(const int pos);
    void changeOrchestration(const int pos);

    int patternLength = 8;
    bool setPatternLength(int newLength);
    void changePatternLength(int newLength);
    SquareButton* lengthButton;

    int bpm = 100;
    void changeBPM(int newBPM);
    SquareButton* bpmButton;

    SquareButton* playButton;
    void setPlaying(bool value);
    bool playing = false;
    void playPatternOnce();
    void highlight(int pos);
    int getPatternFont(int patternLength);

    void setTutorial(bool value);
    std::vector<cocos2d::Node*> getTutorialNodes(int step);
    std::string getTutorialString(int step);
    void showTutorial(int step);
    void hideTutorial(int step);
    cocos2d::Sprite* shadow = nullptr;
    cocos2d::Sprite* cover = nullptr;
    cocos2d::Label* hint = nullptr;
    float tutorialFadeDuration = 0.5;
    int tutorialStep = 0;
	
	void share();
};
