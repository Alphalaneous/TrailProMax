#include <Geode/Geode.hpp>
#include "Config.hpp"
#include <Geode/modify/PauseLayer.hpp>
#include "TrailSettingsPopup.hpp"
#include "PlayerObject.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	Config::get().load();
}

$on_mod(DataSaved) {
	Config::get().save();
}

class $modify(MyPauseLayer, PauseLayer) {

    void customSetup() {
        PauseLayer::customSetup();
		auto iconSpr = CCSprite::create("button-icon.png"_spr);
        auto spr = CircleButtonSprite::create(iconSpr, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
        spr->setScale(0.6f);
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyPauseLayer::onTrailProMax));
        if (auto menu = getChildByID("right-button-menu")) {
            menu->addChild(btn);
			menu->updateLayout();
        }
    }

    void onTrailProMax(CCObject* sender) {
        TrailSettingsPopup::create()->show();
    }
};
