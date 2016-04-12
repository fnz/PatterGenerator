#include "MainWindow.h"

#include "UIConstants.h"
#include "WindowManager.h"
#include "MessageWindow.h"
#include "AboutWindow.h"
#include "ConfirmationWindow.h"
#include "SquareButton.h"

#include "Generator.h"
#include "SoundManager.h"
#include "SaveManager.h"
#include "ListWindow.h"
#include "ObserverManager.h"
#include "SaveManager.h"

#include <iostream>
#include <time.h>

#include "AndroidCpp.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace experimental;

#pragma mark - Init

bool MainWindow::initWindow() {
    FullscreenWindow::initWindow();
    srand(int(time(nullptr)));

    screenSize = Director::getInstance()->getWinSize();

    createUI();
	createStaff();

    changeBPM(100);
    auto pattern = Generator::generatePattern(8);
    auto orchestration = Generator::generateOrchestration(pattern);
    setPattern(pattern, orchestration, true);

    SoundManager::getInstance()->setHighlightCallback(std::bind(&MainWindow::highlight, this, std::placeholders::_1));

    ObserverManager::subscribe<SystemProtocol>(this);

    if (!UserDefault::getInstance()->getBoolForKey("tutorial_complete")) {
        setTutorial(true);
        showTutorial(0);
        setPattern("RKlRllRK", "TKsSssHK", true);
    }

    return true;
}

void MainWindow::createUI() {
    patternFontSize = MED(180);

	pad = 5 * screenSize.width / 320.0f;
	buttonWidth = (screenSize.width - 6 * pad) * 0.2;
	buttonHeight = (screenSize.height - 4 * pad) * 0.33333;
    Size buttonSize = Size(buttonWidth, buttonHeight);

    float buttonX = -2 * buttonWidth - 2 * pad;

    patternY = pad + buttonHeight;
    orchestrationY = 0.0;

	back = Sprite::createWithSpriteFrameName("box.png");
    back->setPosition(0.5 * screenSize);
    back->setScale(100.0f);
    back->setColor(UIConstants::primaryColor);
    addChild(back);

    // basic
    SquareButton* patternButton = SquareButton::create(buttonSize);
    patternButton->setName("pattern_button");
    patternButton->addCallback([this]() {
		patternStr = Generator::generatePattern(patternLength);
		orchestrationStr = Generator::generateOrchestration(patternStr);
		showPattern();
		showOrchestration();
		playPatternOnce();
    });
    patternButton->addLabel("CHANGE\nPATTERN");
    patternButton->setPosition(Vec2(buttonX, pad + buttonHeight));
    addChild(patternButton);

    SquareButton* orchestrationButton = SquareButton::create(buttonSize);
    orchestrationButton->setName("orchestration_button");
    orchestrationButton->addCallback([this]() {
		orchestrationStr = Generator::generateOrchestration(patternStr);
		showOrchestration();
		playPatternOnce();
    });
    orchestrationButton->addLabel("CHANGE\nORCHESTRATION");
    orchestrationButton->setPosition(Vec2(buttonX, 0));
    addChild(orchestrationButton);

    playButton = SquareButton::create(buttonSize);
    playButton->setName("play_button");
    playButton->addCallback([this]() {
		auto sm = SoundManager::getInstance();
		if (playing) {
			setPlaying(false);
			sm->stop();
		} else {
			setPlaying(true);
			sm->stop();
			sm->play(orchestrationStr, false);
		}
    });
    playButton->addIcon("play.png");
    playButton->setPosition(Vec2(buttonX, -pad - buttonHeight));
    addChild(playButton);

    // bpm
    bpmButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    bpmButton->setName("bpm_button");
    bpmButton->setInactiveColor(UIConstants::accentColor);
    bpmButton->setEnabled(false);
    bpmButton->addLabel("100 BPM");
    bpmButton->getLabel()->setColor(UIConstants::textColorLight);
    bpmButton->getLabel()->setOpacity(255);
    bpmButton->setAnchorPoint(Vec2(0.5, 1.0));
    bpmButton->setPosition(Vec2(-buttonWidth - pad, -pad - 0.5 * buttonHeight));
    addChild(bpmButton);

    SquareButton* bpmMinus = SquareButton::create(Size(0.5f * (buttonWidth - pad), 0.5f * (buttonHeight - pad)));
    bpmMinus->setName("bpm_minus");
    bpmMinus->addCallback([this]() {
		changeBPM(bpm - 5);
    });
    bpmMinus->setCallbackDelay(0.0);
    bpmMinus->addIcon("minus.png");
    bpmMinus->setAnchorPoint(Vec2(0.0, 0.0));
    bpmMinus->setPosition(Vec2(-1.5 * buttonWidth - pad, -pad - 1.5 * buttonHeight));
    addChild(bpmMinus);

    SquareButton* bpmPlus = SquareButton::create(Size(0.5f * (buttonWidth - pad), 0.5f * (buttonHeight - pad)));
    bpmPlus->setName("bpm_plus");
    bpmPlus->addCallback([this]() {
		changeBPM(bpm + 5);
    });
    bpmPlus->setCallbackDelay(0.0);
    bpmPlus->addIcon("plus.png");
    bpmPlus->setAnchorPoint(Vec2(1.0, 0.0));
    bpmPlus->setPosition(Vec2(-0.5 * buttonWidth - pad, -pad - 1.5 * buttonHeight));
    addChild(bpmPlus);

    // length
    lengthButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    lengthButton->setName("length_button");
    lengthButton->addLabel("8 NOTES");
    lengthButton->setInactiveColor(UIConstants::accentColor);
    lengthButton->setEnabled(false);
    lengthButton->getLabel()->setColor(UIConstants::textColorLight);
    lengthButton->getLabel()->setOpacity(255);
    lengthButton->setAnchorPoint(Vec2(0.5, 1.0));
    lengthButton->setPosition(Vec2(0.0, -pad - 0.5 * buttonHeight));
    addChild(lengthButton);

    SquareButton* lengthMinus = SquareButton::create(Size(0.5f * (buttonWidth - pad), 0.5f * (buttonHeight - pad)));
    lengthMinus->setName("length_minus");
    lengthMinus->addCallback([this]() {
		changePatternLength(patternLength - 1);
    });
    lengthMinus->setCallbackDelay(0.0);
    lengthMinus->addIcon("minus.png");
    lengthMinus->setAnchorPoint(Vec2(0.0, 0.0));
    lengthMinus->setPosition(Vec2(-0.5 * buttonWidth, -pad - 1.5 * buttonHeight));
    addChild(lengthMinus);

    SquareButton* lengthPlus = SquareButton::create(Size(0.5f * (buttonWidth - pad), 0.5f * (buttonHeight - pad)));
    lengthPlus->setName("length_plus");
    lengthPlus->addCallback([this]() {
		changePatternLength(patternLength + 1);
    });
    lengthPlus->setCallbackDelay(0.0);
    lengthPlus->addIcon("plus.png");
    lengthPlus->setAnchorPoint(Vec2(1.0, 0.0));
    lengthPlus->setPosition(Vec2(0.5 * buttonWidth, -pad - 1.5 * buttonHeight));
    addChild(lengthPlus);

    // save/load
    SquareButton* saveButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    saveButton->setName("save_button");
    saveButton->addCallback([this]() {
		int res = SaveManager::getInstance()->add(patternStr, orchestrationStr);
		if (res != 0) {
			auto message = "This pattern is already saved, you can find it int the list by number " + std::to_string(res) + ".";
			WindowManager::getInstance()->show<MessageWindow>(message);
		} else {
			WindowManager::getInstance()->show<MessageWindow>("Pattern was saved succesfully.");
			SaveManager::getInstance()->save();
		}
    });
    saveButton->addLabel("SAVE");
    saveButton->setAnchorPoint(Vec2(0.5, 1.0));
    saveButton->setPosition(Vec2(buttonWidth + pad, -pad - 0.5 * buttonHeight));
    addChild(saveButton);

    SquareButton* loadButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    loadButton->setName("load_button");
    loadButton->addCallback([this]() {
		auto patterns = SaveManager::getInstance()->get();
		if (patterns.empty()) {
			WindowManager::getInstance()->show<MessageWindow>("You have no saved patterns right now.");
		} else {
			setPlaying(false);
			auto callback = std::bind(&MainWindow::setPattern, this, std::placeholders::_1, std::placeholders::_2, false);
			WindowManager::getInstance()->show<ListWindow>(callback);
		}
    });
    loadButton->addLabel("LOAD");
    loadButton->setAnchorPoint(Vec2(0.5, 0.0));
    loadButton->setPosition(Vec2(buttonWidth + pad, -pad - 1.5 * buttonHeight));
    addChild(loadButton);

    //	about/help
    SquareButton* aboutButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    aboutButton->setName("share_button");
	aboutButton->addCallback(std::bind(&MainWindow::share, this));
    aboutButton->addLabel("SHARE");
    aboutButton->setAnchorPoint(Vec2(0.5, 1.0));
    aboutButton->setPosition(Vec2(2 * buttonWidth + 2 * pad, -pad - 0.5 * buttonHeight));
    addChild(aboutButton);

    SquareButton* helpButton = SquareButton::create(Size(buttonWidth, 0.5f * (buttonHeight - pad)));
    helpButton->setName("help_button");
    helpButton->addCallback([this]() {
		setPlaying(false);
		setTutorial(true);
		tutorialStep = 0;
		showTutorial(tutorialStep);
    });
    helpButton->addLabel("HELP");
    helpButton->setAnchorPoint(Vec2(0.5, 0.0));
    helpButton->setPosition(Vec2(2 * buttonWidth + 2 * pad, -pad - 1.5 * buttonHeight));
    addChild(helpButton);
}

void MainWindow::createStaff() {
	lineY = 0.25*buttonHeight;
	float indent = MED(32);
	float lineScale = 0.0;
	Sprite* line;
	for (int i = 0; i < 5; i++) {
		line = Sprite::createWithSpriteFrameName("line.png");
		if (lineScale == 0.0) {
			lineScale = (4*buttonWidth + 1*pad)/line->getContentSize().width;
		}
		line->setPosition(Vec2(-1.5*buttonWidth, lineY - i*indent));
		line->setAnchorPoint(Vec2(0.0, 0.5));
		line->setScaleX(lineScale);
		addChild(line);
	}
	
	auto endLine1 = Sprite::createWithSpriteFrameName("line_thick.png");
	endLine1->setRotation(90.0);
	endLine1->setScaleX((4*indent + line->getBoundingBox().size.height)/endLine1->getContentSize().width);
	endLine1->setPosition(Vec2(2.5*buttonWidth + pad, lineY + 0.5*line->getBoundingBox().size.height));
	endLine1->setAnchorPoint(Vec2(0.0, 0.5));
	addChild(endLine1);
	
	auto endLine2 = Sprite::createWithSpriteFrameName("line.png");
	endLine2->setRotation(90.0);
	endLine2->setScaleX((4*indent + line->getBoundingBox().size.height)/endLine2->getContentSize().width);
	endLine2->setPosition(Vec2(2.5*buttonWidth + 0.25*pad, lineY + 0.5*line->getBoundingBox().size.height));
	endLine2->setAnchorPoint(Vec2(0.0, 0.5));
	addChild(endLine2);
	
	for (int i = 0; i < 2; i++) {
		auto dot = Sprite::createWithSpriteFrameName("dot.png");
		dot->setPosition(Vec2(2.5*buttonWidth - 0.5*pad, lineY - indent*(i + 1.5)));
		addChild(dot);
	}
	
	for (int i = 0; i < 2; i++) {
		auto clef = Sprite::createWithSpriteFrameName("line_thick.png");
		clef->setRotation(90.0);
		clef->setScaleX((2*indent + line->getBoundingBox().size.height)/clef->getContentSize().width);
		clef->setPosition(Vec2(-1.5*buttonWidth + pad*(i + 1), lineY - indent + 0.5*line->getBoundingBox().size.height));
		clef->setAnchorPoint(Vec2(0.0, 0.5));
		addChild(clef);
	}
	
	for (int i = 0; i < 2; i++) {
		auto topLine = Sprite::createWithSpriteFrameName("line_thick.png");
		topLine->setAnchorPoint(Vec2(0.0, 0.5));
		addChild(topLine);
		topLines.push_back(topLine);
		topLine->setName("line_" + std::to_string(i + 1));
	}
	
	notePositions["S"] = lineY - 1.5*indent;
	notePositions["s"] = lineY - 1.5*indent;
	notePositions["T"] = lineY - 2.5*indent;
	notePositions["K"] = lineY - 3.5*indent;
	notePositions["H"] = lineY + 0.5*indent;
	
	noteImages["S"] = "note.png";
	noteImages["s"] = "ghost.png";
	noteImages["T"] = "note.png";
	noteImages["K"] = "note.png";
	noteImages["H"] = "hat.png";
}

#pragma mark - Core

void MainWindow::setPattern(const std::string& pattern, const std::string& orchestration, bool silent) {
    patternStr = pattern;
    orchestrationStr = orchestration;
    setPatternLength(pattern.length());
    if (!silent) {
        playPatternOnce();
    }
    showPattern();
    showOrchestration();
}

void MainWindow::showPattern() {
    for (Node* note : notes) {
        note->removeFromParentAndCleanup(true);
    }
    notes.clear();

    float halflen = (patternLength - 1) * 0.5;
    int fontSize = 2.0 * getPatternFont(patternLength);
	auto displayStr = Generator::convertToDisplay(patternStr);
    for (int i = 0; i < patternLength; i++) {
		std::string noteStr = "";
		noteStr.push_back(displayStr[i]);
		Label* note = Label::createWithTTF(noteStr.c_str(), UIConstants::patternFont, fontSize);
        note->setScale(0.5);
        note->setPosition(Vec2(0.105 * screenSize.width + (i - halflen) * 0.65 * screenSize.width / (1.0 * patternLength) + 0.5*pad, patternY));
        note->setTag(i);
        addChild(note, 5);
        notes.push_back(note);
    }
}

void MainWindow::showOrchestration() {
    for (Node* drum : drums) {
        drum->removeFromParentAndCleanup(true);
    }
    drums.clear();

    double halflen = (patternLength - 1) * 0.5;
	float topLineStartOffset = 0.0;
	float topLineEndOffset = 0.0;
    for (int i = 0; i < patternLength; i++) {
        std::string drumStr;
        drumStr.push_back(orchestrationStr[i]);

		auto drum = Sprite::createWithSpriteFrameName(noteImages[drumStr]);
        drum->setPosition(Vec2(0.105 * screenSize.width + (i - halflen) * 0.65 * screenSize.width / (1.0 * patternLength), notePositions[drumStr]));
        drum->setTag(i);
        addChild(drum, 5);
        drums.push_back(drum);
		
		auto line = Sprite::createWithSpriteFrameName("line.png");
		auto noteSize = drum->getContentSize();
		Vec2 offset;
		if (drumStr == "H") {
			line->setPosition(0.9*noteSize);
			offset = Vec2(0.4*noteSize.width, 0.3*noteSize.height);
		} else if (drumStr == "s") {
			line->setPosition(Vec2(0.71*noteSize.width, 0.6*noteSize.height));
			offset = Vec2(0.21*noteSize.width, 0.0*noteSize.height);
		} else {
			line->setPosition(Vec2(0.83*noteSize.width, 0.6*noteSize.height));
			offset = Vec2(0.33*noteSize.width, 0.0*noteSize.height);
		}
		
		line->setScaleX((lineY + 4*pad - drum->getPosition().y - offset.y)/line->getContentSize().width);
		line->setAnchorPoint(Vec2(0.0, 0.5));
		line->setRotation(-90.0);
		drum->addChild(line, -1);
		
		if (i == 0) {
			topLineStartOffset = offset.x;
		} else if (i == patternLength - 1) {
			topLineEndOffset = offset.x;
		}
    }
	
	float topLineStart = 0.105 * screenSize.width -halflen * 0.65 * screenSize.width / (1.0 * patternLength) + topLineStartOffset;
	float topLineEnd = 0.105 * screenSize.width + (patternLength - halflen - 1) * 0.65 * screenSize.width / (1.0 * patternLength) + topLineEndOffset;
	for (int i = 0; i < 2; i++) {
		topLines[i]->setScaleX((topLineEnd - topLineStart + MED(4))/topLines[i]->getContentSize().width);
		topLines[i]->setPosition(Vec2(topLineStart - MED(2), lineY + 4*pad - 0.75*pad*i));
	}
}

#pragma mark - Touches

void MainWindow::processTouch(const cocos2d::Vec2& touchLocation) {
    auto note = getClosest(touchLocation, notes);
    if (note != nullptr) {
        auto tag = note->getTag();
        changePattern(note->getTag());
        highlight(tag);
        return;
    }

    auto drum = getClosest(touchLocation, drums);
    if (drum != nullptr) {
        auto tag = drum->getTag();
        changeOrchestration(tag);
        highlight(tag);
        return;
    }
}

Node* MainWindow::getClosest(const cocos2d::Vec2& point, const std::vector<cocos2d::Node*>& objs) {
    Node* ret = nullptr;
    float min = MED(50);
    float eps = MED(50);
    for (const auto obj : objs) {
        float d = obj->getPosition().distance(point);
        if (d < min && d < eps) {
            min = d;
            ret = obj;
        }
    }
    return ret;
}

void MainWindow::changePattern(const int pos) {
    setPlaying(false);
    patternStr[pos] = 'K';
    orchestrationStr[pos] = 'K';
    setPattern(patternStr, orchestrationStr, true);
}
void MainWindow::changeOrchestration(const int pos) {
    setPlaying(false);
    orchestrationStr[pos] = 'H';
    setPattern(patternStr, orchestrationStr, true);
}

#pragma mark - Length and BPM

bool MainWindow::setPatternLength(int newLength) {
    if (newLength > 16) {
        return false;
    }
    if (newLength < 4) {
        return false;
    }
    patternLength = std::min(16, std::max(4, newLength));
    lengthButton->getLabel()->setString(std::to_string(patternLength) + " NOTES");
    return true;
}

void MainWindow::changePatternLength(int newLength) {
    if (!setPatternLength(newLength)) {
        return;
    }
    auto pattern = Generator::generatePattern(patternLength);
    auto orchestration = Generator::generateOrchestration(pattern);
    setPattern(pattern, orchestration, false);
}

void MainWindow::changeBPM(int newBpm) {
    bpm = std::min(200, std::max(50, newBpm));
    bpmButton->getLabel()->setString(std::to_string(bpm) + " BPM");
    SoundManager::getInstance()->setDelay(15.0 / bpm);
}

#pragma mark - Playback

void MainWindow::setPlaying(bool value) {
    playing = value;
    playButton->getIcon()->setSpriteFrame(value ? "stop.png" : "play.png");
    auto sm = SoundManager::getInstance();
    if (!value && sm->isPlaying()) {
        sm->stop();
    }
}

void MainWindow::playPatternOnce() {
    setPlaying(false);
    SoundManager::getInstance()->play(orchestrationStr, true);
}

void MainWindow::highlight(int pos) {
    if (pos < 0 || pos >= notes.size()) {
        return;
    }

    float duration = 15.0 / bpm;

    auto action = Sequence::create(TintTo::create(0.0, UIConstants::accentColor),
                                   DelayTime::create(0.4 * duration),
                                   TintTo::create(0.6 * duration, UIConstants::textColorLight),
                                   nullptr);

    notes[pos]->stopAllActions();
    notes[pos]->runAction(action);
    drums[pos]->stopAllActions();
    drums[pos]->runAction(action->clone());
}

#pragma mark - Stuff

int MainWindow::getPatternFont(int patternLength) {
    int ret = UIConstants::patternFontSize;
    ret -= std::max(0, patternLength - 12) * MED(3);
    return ret;
}

void MainWindow::applicationDidEnterBackground() {
    setPlaying(false);
}

void MainWindow::onBack() {
    if (cover != nullptr) {
        return;
    }
    WindowManager::getInstance()->show<ConfirmationWindow>("Are you sure you want to quit?", [](bool answer) {
		if (answer) {
			Director::getInstance()->end();
		}
    });
}

#pragma mark - Tutorial

void MainWindow::setTutorial(bool value) {
    if (value) {
        shadow = Sprite::createWithSpriteFrameName("box.png");
        shadow->setPosition(0.5 * screenSize);
        shadow->setScale(100.0f);
        shadow->setOpacity(0);
        shadow->setColor(Color3B::BLACK);
        addChild(shadow, 10);

        cover = Sprite::createWithSpriteFrameName("box.png");
        cover->setPosition(0.5 * screenSize);
        cover->setScale(100.0f);
        cover->setOpacity(0);
        addChild(cover, 1000);

        hint = Label::createWithTTF("", UIConstants::buttonFont, 4.0 * UIConstants::buttonFontSize);
        hint->setScale(0.5);
        hint->setColor(UIConstants::primaryColorWhite);
        hint->setPosition(Vec2(0.11 * windowSize.width, 0.1666 * windowSize.height));
        hint->setOpacity(0);
        addChild(hint, 20);

        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
			return true;
        };
        touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
			event->stopPropagation();
			if (shadow->getNumberOfRunningActions() != 0) {
				return;
			}
			hideTutorial(tutorialStep);
        };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, cover);
    } else {
        if (shadow != nullptr) {
            shadow->removeFromParent();
            shadow = nullptr;
        }
        if (cover != nullptr) {
            cover->removeFromParent();
            cover = nullptr;
        }
    }
}

std::vector<Node*> MainWindow::getTutorialNodes(int step) {
    std::vector<Node*> ret;

    if (step == 0) {
    }

    if (step == 1) {
        for (auto note : notes) {
            ret.push_back(note);
        }
    }

    if (step == 2) {
        for (auto drum : drums) {
            ret.push_back(drum);
        }
		ret.push_back(getChildByName("line_1"));
		ret.push_back(getChildByName("line_2"));
    }

    if (step == 3) {
        ret.push_back(getChildByName("pattern_button"));
        ret.push_back(getChildByName("orchestration_button"));
    }

    if (step == 4) {
        ret.push_back(getChildByName("play_button"));
    }

    if (step == 5) {
        ret.push_back(getChildByName("bpm_button"));
        ret.push_back(getChildByName("bpm_plus"));
        ret.push_back(getChildByName("bpm_minus"));
    }

    if (step == 6) {
        ret.push_back(getChildByName("length_button"));
        ret.push_back(getChildByName("length_plus"));
        ret.push_back(getChildByName("length_minus"));
    }

    if (step == 7) {
        ret.push_back(getChildByName("save_button"));
        ret.push_back(getChildByName("load_button"));
    }

    if (step == 8) {
        ret.push_back(getChildByName("share_button"));
    }

    if (step == 9) {
        ret.push_back(getChildByName("help_button"));
    }

    return ret;
}

std::string MainWindow::getTutorialString(int step) {
    switch (step) {
        case 0:
            return "Hello, fellow drummer!\nThis tutorial will guide you\nthrough the basics of the app.";
        case 1:
            return "This line of letters represents the pattern.\nR and L are for right and left hands\nand K is for kick.";
        case 2:
            return "And this is the orchestration.\nStandard drum notes are used.";
        case 3:
            return "Using these two buttons you can change\n the pattern and orchestration.";
        case 4:
            return "This button controls the playback.";
        case 5:
            return "Using these controls you\n can adjust the tempo.";
        case 6:
            return "If you want to change the length of\n your pattern - use these buttons.";
        case 7:
            return "These two buttons allow you to save\na pattern and load it the next time.";
        case 8:
            return "Share cool patterns with the world!";
        case 9:
            return "Help button repeats this tutorial.\nHave fun!";
    }

    return "";
}

void MainWindow::showTutorial(int step) {
    if (tutorialStep > 9) {
        setTutorial(false);
        UserDefault::getInstance()->setBoolForKey("tutorial_complete", true);
        return;
    }
    for (auto node : getTutorialNodes(step)) {
        reorderChild(node, 20);
    }
    shadow->runAction(FadeTo::create(tutorialFadeDuration, 220));
    hint->runAction(Sequence::create(DelayTime::create(0.8 * tutorialFadeDuration),
                                     FadeTo::create(0.2 * tutorialFadeDuration, 255),
                                     nullptr));
    hint->setString(getTutorialString(step));
    if (step == 1) {
        hint->setPosition(Vec2(0.11 * windowSize.width, 0.03 * windowSize.height));
    } else if (step == 2) {
        hint->setPosition(Vec2(0.11 * windowSize.width, -0.27 * windowSize.height));
    } else {
        hint->setPosition(Vec2(0.11 * windowSize.width, 0.1666 * windowSize.height));
    }
}

void MainWindow::hideTutorial(int step) {
    shadow->runAction(FadeTo::create(tutorialFadeDuration, 0));
    hint->runAction(FadeTo::create(0.2 * tutorialFadeDuration, 0));
    runAction(Sequence::create(DelayTime::create(tutorialFadeDuration),
                               CallFunc::create([this, step]() {
								for (auto node : getTutorialNodes(step)) {
									reorderChild(node, 5);
								}
								showTutorial(++tutorialStep);
                               }),
                               nullptr));
}

#pragma mark - Share

void MainWindow::share() {
	setPlaying(false);
	
	back->setVisible(false);
	utils::captureScreen([this](bool, const std::string& screenFile){
		auto pattern = Sprite::create(screenFile);
		auto rect = pattern->getTextureRect();
		float scale = screenSize.width/pattern->getContentSize().width;
		rect.origin.x += (buttonWidth + 2*pad)/scale;
		rect.size.width -= (buttonWidth + 2*pad)/scale;
		rect.size.height -= (buttonHeight + 2*pad)/scale;
		pattern->setTextureRect(rect);
		pattern->setScale(1024.0/pattern->getContentSize().width);
		pattern->setPosition(0.5*pattern->getBoundingBox().size);
		
		pattern->retain();
		
		auto inverseShaderPath = FileUtils::getInstance()->fullPathForFilename("shaders/inverse.fsh");
		auto inverse_frag_str = FileUtils::getInstance()->getStringFromFile(inverseShaderPath);
		const GLchar* inverse_frag = inverse_frag_str.c_str();
		
		GLProgram* inverse = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, inverse_frag);
		inverse->link();
		inverse->updateUniforms();
		
		auto glState = GLProgramState::getOrCreateWithGLProgram(inverse);
		pattern->setGLProgramState(glState);
		
		RenderTexture* renderTexture = RenderTexture::create(pattern->getBoundingBox().size.width,
															 pattern->getBoundingBox().size.height,
															 Texture2D::PixelFormat::RGBA8888);
		renderTexture->begin();
		pattern->visit();
		renderTexture->end();
		
		auto patternDir = GameUtils::getExternalStoragePath() + "PatternGenerator/";
		FileUtils::getInstance()->createDirectory(patternDir);
		auto patternFile = patternDir + "pattern_" + Generator::convertToDisplay(patternStr)  + ".png";
		
		renderTexture->saveToFile(patternFile, Image::Format::PNG, true, [this, pattern, screenFile](RenderTexture*, const std::string& patternFile){
			back->setVisible(true);
			pattern->release();
			Director::getInstance()->getTextureCache()->removeTextureForKey(screenFile);
			FileUtils::getInstance()->removeFile(screenFile);
			GameUtils::share("Share with", "Go grab Pattern Generator on Google Play: ", patternFile);
			std::cout << patternFile;
		});
	}, "screen.png");
}
