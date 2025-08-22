#pragma once

#include <Geode/Geode.hpp>
#include "TrailSettingsPopup.hpp"

using namespace geode::prelude;

class TrailCell : public CCLayer {
    TrailSettingsPopup* m_popup;
    Gamemode m_gamemode;
    TrailType m_trailType;
public:
    static TrailCell* create(TrailSettingsPopup* popup, Gamemode gamemode, TrailType trailType);
    bool init(TrailSettingsPopup* popup, Gamemode gamemode, TrailType trailType);

    void onToggle(CCObject* sender);
    void onSettings(CCObject* sender);
    void onReset(CCObject* sender);

    Gamemode getGamemode();
    TrailType getTrailType();
};