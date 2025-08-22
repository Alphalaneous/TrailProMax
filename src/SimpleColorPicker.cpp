#include "SimpleColorPicker.hpp"
#include "RGBPopup.hpp"

ccColor3B SimpleColorPicker::cc4Bto3B(ccColor4B color) {
    return {color.r, color.g, color.b};
}

ccColor4B SimpleColorPicker::cc3Bto4B(ccColor3B color) {
    return {color.r, color.g, color.b, m_opacity};
}

SimpleColorPicker* SimpleColorPicker::create(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback){
    auto ret = new SimpleColorPicker();
    if (ret->init(color, callback)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool SimpleColorPicker::init(const cocos2d::ccColor4B& color, std::function<void(const cocos2d::ccColor4B&)> callback) {

    m_callback = std::move(callback);
    m_opacity = color.a;
    m_colorPicker = CCControlColourPicker::colourPicker();
    m_colorPicker->setScale(0.8f);
    m_colorPicker->setColorValue(cc4Bto3B(color));
    m_colorPicker->setDelegate(this);
    m_colorPicker->ignoreAnchorPointForPosition(false);
    m_colorPicker->setID("color-picker");

    ignoreAnchorPointForPosition(false);

    auto pickerSize = m_colorPicker->getScaledContentSize();

    setContentSize({pickerSize.width, pickerSize.height + 25});
    auto size = getScaledContentSize();

    m_colorPicker->setPosition(getContentSize());

    addChild(m_colorPicker);

    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");
    settingsSpr->setScale(0.6f);
    m_settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(SimpleColorPicker::onSettings));
    m_settingsBtn->setPosition({m_settingsBtn->getContentSize().width/2 + 15, m_settingsBtn->getContentSize().height/2 + 5});
    m_settingsBtn->setCascadeColorEnabled(true);

    m_settingsBtn->setColor(cc4Bto3B(color));

    auto menu = CCMenu::create();
    menu->setContentSize(size);
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint({0.5f, 0.5f});
    menu->setPosition(size/2);

    menu->addChild(m_settingsBtn);

    addChild(menu);

    auto alphaLabel = CCLabelBMFont::create("A:", "bigFont.fnt");
    alphaLabel->setScale(0.3f);
    alphaLabel->setAnchorPoint({1.f, 0.5f});

    addChild(alphaLabel);

    geode::TextInput* input = geode::TextInput::create(60, "255");
    input->setCommonFilter(CommonFilter::Int);
    #ifdef GEODE_IS_ANDROID
    input->setMaxCharCount(4);
    #else
    input->setMaxCharCount(3);
    #endif

    input->setString(numToString(color.a));
    input->setScale(0.5f);
    input->setCallback([this, input] (const std::string& str) {
        auto res = numFromString<int>(str);
        m_opacity = std::clamp(res.unwrapOr(255), 0, 255);
        if (!input->getString().empty() && res.isOk()) {
            input->setString(numToString(m_opacity));
        }
        colorValueChanged(m_colorPicker->getColorValue());
    });

    input->setPosition({size.width - input->getScaledContentWidth()/2 - 15, 15});
    alphaLabel->setPosition({input->getPositionX() - input->getScaledContentWidth()/2 - 4, 15});

    addChild(input);

    return true;
}

void SimpleColorPicker::colorValueChanged(cocos2d::ccColor3B color) {
    if (m_callback) m_callback(cc3Bto4B(color));
    m_settingsBtn->setColor(color);
}

void SimpleColorPicker::setColor(const cocos2d::ccColor4B& color) {
    m_colorPicker->setColorValue(cc4Bto3B(color));
}

void SimpleColorPicker::onSettings(CCObject* sender) {
    RGBPopup::create(this)->show();
}

ccColor4B SimpleColorPicker::getColor() {
    return cc3Bto4B(m_colorPicker->getColorValue());
}
