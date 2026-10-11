#pragma once

#include "PlayerState.hpp"

struct PlayerSnake {
    PlayerObject* source = nullptr;
    std::vector<PlayerObject*> players;
    std::vector<DisplayedState> displayedStates;
    std::vector<PlayerState> history;
};
