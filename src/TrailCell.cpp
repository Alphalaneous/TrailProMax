#include "TrailCell.hpp"
#include "Config.hpp"
#include "TrailSettings.hpp"

TrailCell* TrailCell::create(TrailSettingsPopup* popup, Gamemode gamemode, TrailType trailType) {
    auto ret = new TrailCell();
    if (ret->init(popup, gamemode, trailType)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool TrailCell::init(TrailSettingsPopup* popup, Gamemode gamemode, TrailType trailType) {
    if (!CCLayer::init()) return false;

    m_popup = popup;
    m_gamemode = gamemode;
    m_trailType = trailType;

    auto& config = Config::get();
    auto& gamemodeC = config.getGamemodeData(m_gamemode);
    auto& trail = gamemodeC.getTrailData(m_trailType);

    setAnchorPoint({0.5f, 0.5f});
    ignoreAnchorPointForPosition(false);
    setContentSize({270, 30});

    auto background = CCScale9Sprite::create("square02b_001.png");
    background->setColor({127, 127, 127});
    background->setOpacity(20);
    background->setContentSize(getContentSize());
    background->setPosition(getContentSize()/2);

    auto size = getContentSize();

    auto trailLabel = CCLabelBMFont::create(trailTypeToReadableString(trailType).c_str(), "bigFont.fnt");
    trailLabel->setScale(0.5f);
    trailLabel->setAnchorPoint({0.f, 0.5f});
    trailLabel->setPosition({10, size.height/2});

    auto resetSpr = CCSprite::createWithSpriteFrameName("geode.loader/reset-gold.png");
    resetSpr->setScale(0.4f);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(TrailCell::onReset));
    resetBtn->setPosition({size.width - 63, size.height/2});
    
    auto toggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(TrailCell::onToggle), 1.f);
    toggler->setScale(0.5f);
    toggler->setPosition({size.width - 40, size.height/2});
    toggler->toggle(trail.isParticlesEnabled());
    
    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    settingsSpr->setScale(0.4f);
    auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(TrailCell::onSettings));
    settingsBtn->setPosition({size.width - 15, size.height/2});

    auto menu = CCMenu::create();
    menu->setContentSize(size);
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint({0.5f, 0.5f});
    menu->setPosition(size/2);

    menu->addChild(toggler);
    menu->addChild(settingsBtn);
    menu->addChild(resetBtn);

    addChild(background);
    addChild(trailLabel);
    addChild(menu);

    return true;
}

void TrailCell::onToggle(CCObject* sender) {
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_gamemode);
    auto& trail = gamemode.getTrailData(m_trailType);

    trail.setParticlesEnabled(!toggler->isToggled());
}

void TrailCell::onSettings(CCObject* sender) {
    TrailSettings::create(this)->show();
}

void TrailCell::onReset(CCObject* sender) {
    createQuickPopup("Reset", "Are you sure you want to <cr>reset</c> the <cl>color settings</c> for this particle trail to <cy>default</c>?", "Cancel", "Reset", [this] (FLAlertLayer*, bool btn2) {
        if (btn2) {
            auto& config = Config::get();
            auto& gamemode = config.getGamemodeData(m_gamemode);
            auto& trail = gamemode.getTrailData(m_trailType);
            trail.setColorDefaults(defaultColorsForType(m_trailType));
        }
    }, true);
}

Gamemode TrailCell::getGamemode() {
    return m_gamemode;
}

TrailType TrailCell::getTrailType() {
    return m_trailType;
}