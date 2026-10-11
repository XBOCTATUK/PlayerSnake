#pragma once

struct PlayerState {
    cocos2d::CCPoint position;
    float rotation = 0.0f;
    IconType gamemode = IconType::Cube;
    bool isMini = false;
    bool isUpsideDown = false;
    bool isVisible = true;
    bool isGrounded = false;
    bool isStationary = false;
    bool isFalling = false;
    float mainScaleX = 1.0f;
    float mainScaleY = 1.0f;
    float mainRotation = 0.0f;
};

struct DisplayedState {
    IconType gamemode = IconType::Cube;
    bool isMini = false;
    bool isUpsideDown = false;
    std::string animation;
};
