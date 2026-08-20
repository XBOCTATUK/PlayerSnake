#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

class $modify(BMITextInput, CCTextInputNode) {
    void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo& info) override {
        log::info("keyboardWillShow");
    }
};