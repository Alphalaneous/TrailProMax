#pragma once

#include <Geode/Geode.hpp>
#include "SimpleColorPicker.hpp"

using namespace geode::prelude;

class RGBPopup : public geode::Popup<SimpleColorPicker*> {
    SimpleColorPicker* m_colorPicker;
    ccColor4B m_color;
public:
    static RGBPopup* create(SimpleColorPicker*);
    bool setup(SimpleColorPicker*) override;
    CCNode* createInput(GLubyte value, const std::string& labelStr, std::function<void(GLubyte byte)>);
};