#include <Geode/modify/PlayLayer.hpp>
#include "../Settings/Settings.hpp"
#include "../Utils/PlayerUtils.hpp"
#include "../Models/PlayerSnake.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    struct Fields {
        int m_ticks = 0;
        int m_playerDistance = 0;

        PlayerSnake m_player1Snake;
        PlayerSnake m_player2Snake;
    };

    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        int playerCount = Settings::getPlayerCount();
        int playerDistance = Settings::getPlayerDistance();
        if (playerCount <= 0 || playerDistance <= 0) return;

        m_fields->m_playerDistance = playerDistance;

        setupSnake(m_fields->m_player1Snake, m_player1, playerCount, false);
        setupSnake(m_fields->m_player2Snake, m_player2, playerCount, true);
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        m_fields->m_ticks = 0;

        for (auto player : m_fields->m_player1Snake.players) {
            player->setPosition({ 0.0f, 105.0f });
        }
        for (auto player : m_fields->m_player2Snake.players) {
            player->setPosition({ 0.0f, 105.0f });
        }
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (m_fields->m_player1Snake.players.empty() && m_fields->m_player2Snake.players.empty()) return;

        updateSnake(m_fields->m_player1Snake, true);
        updateSnake(m_fields->m_player2Snake, m_gameState.m_isDualMode);
        m_fields->m_ticks++;
    }

    void setupSnake(PlayerSnake& snake, PlayerObject* source, int playerCount, bool isSecondPlayer) {
        if (!source) return;

        snake.source = source;
        snake.history.resize(m_fields->m_playerDistance * playerCount + 1);

        for (int i = 0; i < playerCount; i++) {
            if (auto player = createFakePlayer(source, i, isSecondPlayer)) {
                snake.players.push_back(player);
            }
        }

        snake.displayedStates.resize(snake.players.size());
    }

    void updateSnake(PlayerSnake& snake, bool isActive) {
        if (snake.players.empty() || snake.history.empty()) return;

        int ticks = m_fields->m_ticks + 1;
        size_t capacity = snake.history.size();

        auto state = Utils::getPlayerState(snake.source);
        state.isVisible = state.isVisible && isActive;
        snake.history[m_fields->m_ticks % capacity] = state;

        for (size_t i = 0; i < snake.players.size(); i++) {
            int delay = m_fields->m_playerDistance * (i + 1);
            if (ticks <= delay) continue;

            size_t idx = (ticks - 1 - delay) % capacity;
            applyState(
                snake.players[i],
                snake.displayedStates[i],
                snake.history[idx]
            );
        }
    }

    PlayerObject* createFakePlayer(PlayerObject* source, int index, bool isSecondPlayer) {
        auto gm = GameManager::get();

        auto player = PlayerObject::create(
            gm->getPlayerFrame(), gm->getPlayerShip(), this, m_objectLayer, true
        );
        if (!player) return nullptr;

        player->m_isPlatformer = source->m_isPlatformer;
        player->m_shipStreakType = ShipStreak::None;
        if (player->m_shipStreak) {
            player->m_shipStreak->removeFromParent();
            player->m_shipStreak = nullptr;
        }
        player->setUserFlag(FAKE_PLAYER_FLAG);

        player->setColor(source->m_iconSprite->getColor());
        player->setSecondColor(source->getSecondColor());

        player->m_hasGlow = source->m_hasGlow;
        if (source->m_hasCustomGlowColor) {
            player->enableCustomGlowColor(source->m_glowColor);
        }
        else {
            player->disableCustomGlowColor();
        }

        Utils::setPlayerMode(player, IconType::Cube);

        player->setPosition(source->getPosition());
        player->setVisible(false);
        player->setID(fmt::format("fake-player-{}-{}"_spr, isSecondPlayer ? 2 : 1, index + 1));

        auto parent =
            source->getParent()
            ? source->getParent()
            : static_cast<CCNode*>(m_objectLayer);

        parent->addChild(player, source->getZOrder() - 1);

        return player;
    }

    void applyState(PlayerObject* player, DisplayedState& displayed, const PlayerState& state) {
        bool switchedMode = displayed.gamemode != state.gamemode;
        bool changedGravity = displayed.isUpsideDown != state.isUpsideDown;

        player->m_isUpsideDown = state.isUpsideDown;
        player->m_isGoingLeft = state.mainScaleX < 0.0f;

        if (displayed.isMini != state.isMini) {
            player->togglePlayerScale(state.isMini, true);
            player->updatePlayerScale();
            displayed.isMini = state.isMini;
        }

        if (switchedMode || (changedGravity && state.gamemode == IconType::Ship)) {
            if (displayed.gamemode == IconType::Swing) {
                Utils::setSwingFireVisible(player, false);
            }

            if (displayed.gamemode == IconType::Robot) {
                Utils::setRobotFire(player, false);
            }

            Utils::setPlayerMode(player, state.gamemode);

            displayed.gamemode = state.gamemode;
            displayed.animation.clear();
        }

        if (state.gamemode == IconType::Swing && (switchedMode || changedGravity)) {
            if (switchedMode) {
                Utils::setSwingFireVisible(player, true);
                player->m_swingFireMiddle->animateFireIn();
            }

            Utils::animateSwingFire(player, !state.isUpsideDown);
        }

        displayed.isUpsideDown = state.isUpsideDown;

        if (state.gamemode == IconType::Robot || state.gamemode == IconType::Spider) {
            auto animation = Utils::getRobotAnimation(state);

            if (displayed.animation != animation) {
                bool isRobot = state.gamemode == IconType::Robot;
                auto sprite =
                    isRobot
                    ? player->m_robotSprite
                    : player->m_spiderSprite;

                sprite->tweenToAnimation(animation, animation == "idle01" ? 0.3f : 0.1f);
                if (isRobot) {
                    Utils::setRobotFire(player, animation == "jump_loop");
                }

                displayed.animation = animation;
            }
        }

        player->setPosition(state.position);
        player->setRotation(state.rotation);

        player->m_mainLayer->setScaleX(state.mainScaleX);
        player->m_mainLayer->setScaleY(state.mainScaleY);
        player->m_mainLayer->setRotation(state.mainRotation);

        if (player->isVisible() != state.isVisible) {
            player->setVisible(state.isVisible);
        }
    }
};
