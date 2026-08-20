#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

class $modify(BMITextInput, CCTextInputNode) {
	bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
		if (!CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont)) return false;
		
		bool onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *tField) {
			return false;
		}

		return true;
	}
};