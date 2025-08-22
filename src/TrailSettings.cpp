#include "TrailSettings.hpp"
#include "Config.hpp"
#include "SimpleColorPicker.hpp"

TrailSettings* TrailSettings::create(TrailCell* cell) {
    auto ret = new TrailSettings();
    if (ret->initAnchored(310.f, 225.f, cell, "geode.loader/GE_square02.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool TrailSettings::setup(TrailCell* cell) {
    m_cell = cell;
    setTitle(fmt::format("{} Color Settings", trailTypeToReadableString(cell->getTrailType())));
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(cell->getGamemode());
    auto& trailData = gamemode.getTrailData(cell->getTrailType());

    auto size = m_mainLayer->getContentSize();

    m_colorsContainer = CCNode::create();
    m_colorsContainer->setAnchorPoint({0.5f, 1.f});
    m_colorsContainer->ignoreAnchorPointForPosition(false);
    m_colorsContainer->setPosition(size.width/2, size.height - 65);
    m_colorsContainer->setContentSize({size.width - 20, 150});

    m_mainLayer->addChild(m_colorsContainer);

    auto toggleColorLabel = CCLabelBMFont::create("Colors", "bigFont.fnt");
    toggleColorLabel->setAnchorPoint({0.f, 0.5f});
    toggleColorLabel->setScale(0.4f);
    auto toggleRainbowLabel = CCLabelBMFont::create("Rainbow", "bigFont.fnt");
    toggleRainbowLabel->setAnchorPoint({0.f, 0.5f});
    toggleRainbowLabel->setScale(0.4f);

    m_colorToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(TrailSettings::onToggleColors), 0.5f);
    m_colorToggler->setPosition({30, size.height - 50});
    m_colorToggler->setCascadeColorEnabled(true);
    m_colorToggler->setCascadeOpacityEnabled(true);

    toggleColorLabel->setPosition({42, size.height - 50});

    m_rainbowToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(TrailSettings::onToggleRainbow), 0.5f);
    m_rainbowToggler->setPosition({size.width/2 + 20, size.height - 50});
    m_rainbowToggler->setCascadeColorEnabled(true);
    m_rainbowToggler->setCascadeOpacityEnabled(true);

    toggleRainbowLabel->setPosition({size.width/2 + 32, size.height - 50});

    m_buttonMenu->addChild(m_colorToggler);
    m_buttonMenu->addChild(m_rainbowToggler);

    m_mainLayer->addChild(toggleColorLabel);
    m_mainLayer->addChild(toggleRainbowLabel);

    if (trailData.isColorEnabled()) {
        m_colorToggler->toggle(true);
        m_rainbowToggler->toggle(false);
    }
    else if (trailData.isRainbowEnabled()) {
        m_colorToggler->toggle(false);
        m_rainbowToggler->toggle(true);
    }

    generateColorsContainer();
    return true;
}

void TrailSettings::generateColorsContainer() {
    m_colorsContainer->removeAllChildren();

    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_cell->getGamemode());
    auto& trailData = gamemode.getTrailData(m_cell->getTrailType());

    auto background = CCScale9Sprite::create("square02b_001.png");
    background->setColor({0, 0, 0});
    background->setOpacity(127);
    background->setContentSize(m_colorsContainer->getContentSize());
    background->setPosition(m_colorsContainer->getContentSize()/2);
    m_colorsContainer->addChild(background);

    auto separator = CCSprite::createWithSpriteFrameName("floorLine_01_001.png");
    separator->setRotation(90);
    separator->setScaleX(0.275f);
    separator->setPosition(m_colorsContainer->getContentSize()/2);

    m_colorsContainer->addChild(separator);

    auto menu = CCMenu::create();
    menu->setContentSize(m_colorsContainer->getContentSize());
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint({0.5f, 0.5f});
    menu->setPosition(m_colorsContainer->getContentSize()/2);

    m_colorsContainer->addChild(menu);

    if (trailData.isColorEnabled()) {
        auto currentStartColor = ccc4BFromccc4F(trailData.getStartColor());
        auto currentEndColor = ccc4BFromccc4F(trailData.getEndColor());

        auto startColorPicker = SimpleColorPicker::create(currentStartColor, [&trailData] (const cocos2d::ccColor4B& color) {
            trailData.setStartColor(color);
        });

        startColorPicker->setScale(0.8f);
        startColorPicker->setPosition({startColorPicker->getScaledContentWidth()/2 + 10, m_colorsContainer->getContentHeight()/2 - 10});

        auto endColorPicker = SimpleColorPicker::create(currentEndColor, [&trailData] (const cocos2d::ccColor4B& color) {
            trailData.setEndColor(color);
        });

        endColorPicker->setScale(0.8f);
        endColorPicker->setPosition({m_colorsContainer->getContentWidth() - endColorPicker->getScaledContentWidth()/2 - 10, m_colorsContainer->getContentHeight()/2 - 10});

        m_colorsContainer->addChild(startColorPicker);
        m_colorsContainer->addChild(endColorPicker);

        auto startLabel = CCLabelBMFont::create("Start", "bigFont.fnt");
        startLabel->setScale(0.4f);
        startLabel->setPosition(startColorPicker->getScaledContentWidth()/2 + 10, m_colorsContainer->getContentHeight() - 10);

        auto endLabel = CCLabelBMFont::create("End", "bigFont.fnt");
        endLabel->setScale(0.4f);
        endLabel->setPosition(m_colorsContainer->getContentWidth() - endColorPicker->getScaledContentWidth()/2 - 10, m_colorsContainer->getContentHeight() - 10);

        m_colorsContainer->addChild(startLabel);
        m_colorsContainer->addChild(endLabel);

    }
    else if (trailData.isRainbowEnabled()) {
        auto currentRainbowColor = ccc4BFromccc4F(trailData.getRainbowColor());
        auto rainbowColorPicker = SimpleColorPicker::create(currentRainbowColor, [&trailData] (const cocos2d::ccColor4B& color) {
            trailData.setRainbowColor(color);
        });

        rainbowColorPicker->setScale(0.8f);
        rainbowColorPicker->setPosition({rainbowColorPicker->getScaledContentWidth()/2 + 10, m_colorsContainer->getContentHeight()/2 - 10});

        auto startLabel = CCLabelBMFont::create("Rainbow Start", "bigFont.fnt");
        startLabel->setScale(0.4f);
        startLabel->setPosition(rainbowColorPicker->getScaledContentWidth()/2 + 10, m_colorsContainer->getContentHeight() - 10);

        m_colorsContainer->addChild(rainbowColorPicker);
        m_colorsContainer->addChild(startLabel);

        
        auto settingsLabel = CCLabelBMFont::create("Settings", "bigFont.fnt");
        settingsLabel->setScale(0.4f);
        settingsLabel->setPosition(m_colorsContainer->getContentWidth() - rainbowColorPicker->getScaledContentWidth()/2 - 10, m_colorsContainer->getContentHeight() - 10);

        m_colorsContainer->addChild(settingsLabel);

        auto hueRateLabel = CCLabelBMFont::create("Hue Rate", "bigFont.fnt");
        hueRateLabel->setScale(0.35f);
        hueRateLabel->setAnchorPoint({0.f, 0.5f});
        hueRateLabel->setPosition(m_colorsContainer->getContentWidth()/2 + 10, m_colorsContainer->getContentHeight() - 35);

        geode::TextInput* input = geode::TextInput::create(60, "255");
        input->setCommonFilter(CommonFilter::Float);

        input->setString(numToString(trailData.getHueShift()));
        input->setScale(0.5f);
        input->setCallback([this, input, &trailData] (const std::string& str) {
            auto res = numFromString<float>(str);
            float rate = res.unwrapOr(240.f);
            trailData.setHueShift(rate);
        });

        input->setPosition({m_colorsContainer->getContentWidth() - input->getScaledContentWidth()/2 - 10, m_colorsContainer->getContentHeight() - 35});

        m_colorsContainer->addChild(input);
        m_colorsContainer->addChild(hueRateLabel);
        
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        infoSpr->setScale(0.4f);
        auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(TrailSettings::onHueRateInfo));
        infoBtn->setPosition(m_colorsContainer->getContentWidth()/2 + hueRateLabel->getScaledContentWidth() + infoBtn->getContentWidth()/2 + 15, m_colorsContainer->getContentHeight() - 35);

        menu->addChild(infoBtn);
    }
    else {
        auto noticeLabel = CCLabelBMFont::create("Select a Color Mode", "bigFont.fnt");
        noticeLabel->setScale(0.6f);
        noticeLabel->setPosition(m_colorsContainer->getContentSize()/2);
        m_colorsContainer->addChild(noticeLabel);
        separator->removeFromParent();
    }
}

void TrailSettings::onToggleRainbow(CCObject* object) {
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_cell->getGamemode());
    auto& trailData = gamemode.getTrailData(m_cell->getTrailType());

    if (!m_rainbowToggler->isToggled()) {
        m_colorToggler->toggle(false);
        trailData.setRainbowEnabled(true);
        trailData.setColorEnabled(false);
    }
    else {
        trailData.setRainbowEnabled(false);
    }

    generateColorsContainer();
}

void TrailSettings::onToggleColors(CCObject* object) {
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_cell->getGamemode());
    auto& trailData = gamemode.getTrailData(m_cell->getTrailType());

    if (!m_colorToggler->isToggled()) {
        m_rainbowToggler->toggle(false);
        trailData.setColorEnabled(true);
        trailData.setRainbowEnabled(false);
    }
    else {
        trailData.setColorEnabled(false);
    }

    generateColorsContainer();
}

void TrailSettings::onHueRateInfo(CCObject* object) {
    createQuickPopup("Hue Shift Info", "The rate at which the rainbow hue changes per second. For example, a value of <cg>240</c> means the hue shifts by <cg>240</c> out of a total of <cy>360</c> per second.", "OK", nullptr, nullptr, true);
}
