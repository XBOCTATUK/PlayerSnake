#include <Geode/modify/CCIMEDelegate.hpp>

using namespace geode::prelude;

class $modify(MyIMEDelegate, CCIMEDelegate) {
    bool attachWithIME() {
        log::info("attachWithIME");
        return CCIMEDelegate::attachWithIME();
    }
};