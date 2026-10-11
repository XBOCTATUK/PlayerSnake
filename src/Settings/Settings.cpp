#include "Settings.hpp"

using namespace geode::prelude;

namespace Settings {
    static int g_playerCount = Mod::get()->getSettingValue<int>("player-count");
    static int g_playerDistance = Mod::get()->getSettingValue<int>("player-distance");

    int getPlayerCount() {
        return g_playerCount;
    }

    int getPlayerDistance() {
        return g_playerDistance;
    }
};

$on_mod(Loaded) {
    listenForSettingChanges<int>("player-count", [](int value) {
        Settings::g_playerCount = value;
    });

    listenForSettingChanges<int>("player-distance", [](int value) {
        Settings::g_playerDistance = value;
    });
}
