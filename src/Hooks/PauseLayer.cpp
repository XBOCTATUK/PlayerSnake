#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto rightMenu = getChildByID("right-button-menu");
        if (!rightMenu) return;

        auto spr = CCSprite::create("logo.png"_spr);
        spr->setScale(30.0f / spr->getContentWidth());

        auto btn = CCMenuItemExt::createSpriteExtra(
            spr, [](auto) {
                openSettingsPopup(Mod::get(), false);
            }
        );
        rightMenu->addChild(btn);

        rightMenu->updateLayout();
    }
};