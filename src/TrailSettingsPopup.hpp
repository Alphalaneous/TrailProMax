#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum class Gamemode : int;
enum class TrailType : int;

class TrailSettingsPopup : public geode::Popup<> {
    geode::ScrollLayer* m_scollLayer;
    CCMenu* m_gamemodesMenu;
    Gamemode m_selectedGamemode;
    CCMenuItemToggler* m_trailToggler;
    CCMenuItemToggler* m_hideTrailToggler;
public:
    static TrailSettingsPopup* create();
    bool setup() override;
    void generateGamemodeButtons();
    void generateTrailCells();
    void onGamemodeButton(CCObject* object);
    void onToggleTrail(CCObject* object);
    void onToggleDisableTrail(CCObject* object);
    void updateTogglers();
    void toggleForceToggler();

    CCMenuItemSpriteExtra* createForGamemode(Gamemode gamemode);
};
