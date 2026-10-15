#pragma once

#include "../Models/PlayerState.hpp"

inline const std::string FAKE_PLAYER_FLAG = "ps-fake-player";

namespace Utils {
    inline bool isFakePlayer(PlayerObject* player) {
        return player->getUserFlag(FAKE_PLAYER_FLAG);
    }

    inline IconType getPlayerMode(PlayerObject* player) {
        if (player->m_isShip) return player->m_isPlatformer ? IconType::Jetpack : IconType::Ship;
        if (player->m_isBird) return IconType::Ufo;
        if (player->m_isBall) return IconType::Ball;
        if (player->m_isDart) return IconType::Wave;
        if (player->m_isRobot) return IconType::Robot;
        if (player->m_isSpider) return IconType::Spider;
        if (player->m_isSwing) return IconType::Swing;

        return IconType::Cube;
    }

    inline PlayerState getPlayerState(PlayerObject* player) {
        return {
            .position = player->getPosition(),
            .rotation = player->getRotation(),
            .gamemode = getPlayerMode(player),
            .isMini = player->m_vehicleSize < 1.0f,
            .isUpsideDown = player->m_isUpsideDown,
            .isVisible = player->isVisible() && !player->m_isDead,
            .isGrounded = player->m_isOnGround,
            .isStationary = player->m_isPlatformer && std::abs(player->m_platformerXVelocity) < 0.1,
            .isFalling = player->m_yVelocity < 0.0,
            .mainScaleX = player->m_mainLayer->getScaleX(),
            .mainScaleY = player->m_mainLayer->getScaleY(),
            .mainRotation = player->m_mainLayer->getRotation()
        };
    }

    inline std::string getRobotAnimation(const PlayerState& state) {
        if (state.isGrounded) {
            return state.isStationary ? "idle01" : "run";
        }

        return state.isFalling ? "fall_loop" : "jump_loop";
    }

    inline void setPlayerMode(PlayerObject* player, IconType mode) {
        auto gm = GameManager::get();

        player->toggleFlyMode(false, true);
        player->toggleRollMode(false, true);
        player->toggleBirdMode(false, true);
        player->toggleDartMode(false, true);
        player->toggleRobotMode(false, true);
        player->toggleSpiderMode(false, true);
        player->toggleSwingMode(false, true);

        switch (mode) {
            case IconType::Ship:
                player->toggleFlyMode(true, true);
                player->updatePlayerShipFrame(gm->getPlayerShip());
                break;
            case IconType::Jetpack:
                player->toggleFlyMode(true, true);
                player->updatePlayerJetpackFrame(gm->getPlayerJetpack());
                break;
            case IconType::Ball:
                player->toggleRollMode(true, true);
                player->updatePlayerRollFrame(gm->getPlayerBall());
                break;
            case IconType::Ufo:
                player->toggleBirdMode(true, true);
                player->updatePlayerBirdFrame(gm->getPlayerBird());
                break;
            case IconType::Wave:
                player->toggleDartMode(true, true);
                player->updatePlayerDartFrame(gm->getPlayerDart());
                break;
            case IconType::Robot:
                player->toggleRobotMode(true, true);
                player->updatePlayerRobotFrame(gm->getPlayerRobot());
                break;
            case IconType::Spider:
                player->toggleSpiderMode(true, true);
                player->updatePlayerSpiderFrame(gm->getPlayerSpider());
                break;
            case IconType::Swing:
                player->toggleSwingMode(true, true);
                player->updatePlayerSwingFrame(gm->getPlayerSwing());
                break;
            default:
                break;
        }

        if (mode == IconType::Cube || mode == IconType::Ship || mode == IconType::Ufo || mode == IconType::Jetpack) {
            player->updatePlayerFrame(gm->getPlayerFrame());
        }

        player->updateGlowColor();
        player->updatePlayerGlow();
    }

    inline void setSwingFireVisible(PlayerObject* player, bool visible) {
        player->m_swingFireTop->setVisible(visible);
        player->m_swingFireMiddle->setVisible(visible);
        player->m_swingFireBottom->setVisible(visible);
    }

    inline void animateSwingFire(PlayerObject* player, bool goingDown) {
        if (goingDown) {
            player->m_swingFireTop->animateFireIn();
            player->m_swingFireBottom->animateFireOut();
        } else {
            player->m_swingFireTop->animateFireOut();
            player->m_swingFireBottom->animateFireIn();
        }
    }

    inline void setRobotFire(PlayerObject* player, bool enable) {
        if (!player->m_robotFire) return;

        player->m_robotFire->setVisible(enable);
        if (enable) player->m_robotFire->animateFireIn();
    }
}
