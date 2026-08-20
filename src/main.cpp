#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

class $modify(BMITextInput, CCTextInputNode) {
	bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *tField) {
		return false;
	}
};