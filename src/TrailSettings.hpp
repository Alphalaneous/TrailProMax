#pragma once

#include <Geode/Geode.hpp>
#include "TrailCell.hpp"

using namespace geode::prelude;

class TrailSettings : public geode::Popup<TrailCell*> {
    geode::ScrollLayer* m_scollLayer;
    CCNode* m_colorsContainer;
    TrailCell* m_cell;
    CCMenuItemToggler* m_colorToggler;
    CCMenuItemToggler* m_rainbowToggler;
public:
    static TrailSettings* create(TrailCell*);
    bool setup(TrailCell*) override;
    void generateColorsContainer();
    void onToggleRainbow(CCObject* object);
    void onToggleColors(CCObject* object);
    void onHueRateInfo(CCObject* object);

};