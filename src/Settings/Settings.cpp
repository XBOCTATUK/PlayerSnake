#include "Settings.hpp"

using namespace geode::prelude;

namespace Settings {
    static bool g_enabled = Mod::get()->getSettingValue<bool>("enabled");
    static int g_playerCount = Mod::get()->getSettingValue<int>("player-count");
    static int g_playerDistance = Mod::get()->getSettingValue<int>("player-distance");
    static bool g_stopSnakeOnDeath = Mod::get()->getSettingValue<bool>("stop-snake-on-death");

    bool getEnabled() {
        return g_enabled;
    }

    int getPlayerCount() {
        return g_playerCount;
    }

    int getPlayerDistance() {
        return g_playerDistance;
    }

    bool getStopSnakeOnDeath() {
        return g_stopSnakeOnDeath;
    }
};

$on_mod(Loaded) {
    listenForSettingChanges<bool>("enabled", [](bool value) {
        Settings::g_enabled = value;
    });

    listenForSettingChanges<int>("player-count", [](int value) {
        Settings::g_playerCount = value;
    });

    listenForSettingChanges<int>("player-distance", [](int value) {
        Settings::g_playerDistance = value;
    });

    listenForSettingChanges<bool>("stop-snake-on-death", [](bool value) {
        Settings::g_stopSnakeOnDeath = value;
    });
}
