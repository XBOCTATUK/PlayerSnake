#include <Geode/modify/PlayerObject.hpp>
#include "../Utils/PlayerUtils.hpp"

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
    void activateStreak() {
        if (Utils::isFakePlayer(this)) return;

        PlayerObject::activateStreak();
    }
};
