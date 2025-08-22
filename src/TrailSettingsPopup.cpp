#include "TrailSettingsPopup.hpp"
#include "Config.hpp"
#include "TrailCell.hpp"

TrailSettingsPopup* TrailSettingsPopup::create() {
    auto ret = new TrailSettingsPopup();
    if (ret->initAnchored(300.f, 280.f, "geode.loader/GE_square02.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool TrailSettingsPopup::setup() {

    setTitle("Trail Pro Max");
    auto size = m_mainLayer->getContentSize();

    m_selectedGamemode = Gamemode::CUBE;
    m_gamemodesMenu = CCMenu::create();
    m_gamemodesMenu->setContentSize({size.width-80, 70});
    m_gamemodesMenu->ignoreAnchorPointForPosition(false);
    m_gamemodesMenu->setAnchorPoint({0.5f, 1.f});
    m_gamemodesMenu->setPosition({size.width/2, size.height - 35});

    auto background = CCScale9Sprite::create("square02b_001.png");
    background->setColor({0, 0, 0});
    background->setAnchorPoint({0.5f, 0.f});
    background->setOpacity(127);
    background->setContentSize({280, 130});
    background->setPosition({size.width/2, 35});
    m_mainLayer->addChild(background);

    generateGamemodeButtons();

    auto toggleLabel = CCLabelBMFont::create("Show Trail", "bigFont.fnt");
    toggleLabel->setAnchorPoint({0.f, 0.5f});
    toggleLabel->setScale(0.4f);
    auto forceToggleLabel = CCLabelBMFont::create("Always Hide", "bigFont.fnt");
    forceToggleLabel->setAnchorPoint({0.f, 0.5f});
    forceToggleLabel->setScale(0.4f);

    m_trailToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(TrailSettingsPopup::onToggleTrail), 0.5f);
    m_trailToggler->setPosition({20, 20});
    m_trailToggler->setCascadeColorEnabled(true);
    m_trailToggler->setCascadeOpacityEnabled(true);

    toggleLabel->setPosition({32, 20});

    m_hideTrailToggler = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(TrailSettingsPopup::onToggleDisableTrail), 0.5f);
    m_hideTrailToggler->setPosition({size.width/2 + 10, 20});
    m_hideTrailToggler->setCascadeColorEnabled(true);
    m_hideTrailToggler->setCascadeOpacityEnabled(true);

    forceToggleLabel->setPosition({size.width/2 + 22, 20});

    m_buttonMenu->addChild(m_trailToggler);
    m_buttonMenu->addChild(m_hideTrailToggler);

    updateTogglers();

    m_mainLayer->addChild(m_gamemodesMenu);
    m_mainLayer->addChild(toggleLabel);
    m_mainLayer->addChild(forceToggleLabel);

    m_scollLayer = geode::ScrollLayer::create({280, 130});
    m_scollLayer->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::End)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setGap(2.5f)
    );

    m_scollLayer->setPosition({size.width/2, 35 + m_scollLayer->getContentHeight()/2});

    m_mainLayer->addChild(m_scollLayer);

    m_scollLayer->ignoreAnchorPointForPosition(false);
    m_scollLayer->m_contentLayer->setContentHeight(130);

    generateTrailCells();

    return true;
}

CCMenuItemSpriteExtra* TrailSettingsPopup::createForGamemode(Gamemode gamemode) {

    std::string gamemodeStr;
    if (gamemode == m_selectedGamemode) {
        gamemodeStr = fmt::format("gj_{}Btn_on_001.png", gamemodeToStringRob(gamemode));
    }
    else {
        gamemodeStr = fmt::format("gj_{}Btn_off_001.png", gamemodeToStringRob(gamemode));
    }

    auto spr = CCSprite::createWithSpriteFrameName(gamemodeStr.c_str());
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(TrailSettingsPopup::onGamemodeButton));
    btn->setTag(static_cast<int>(gamemode));
    return btn;
}

void TrailSettingsPopup::generateGamemodeButtons() {

    m_gamemodesMenu->removeAllChildren();
    auto size = m_gamemodesMenu->getContentSize();

    auto cube = createForGamemode(CUBE);
    auto ship = createForGamemode(SHIP);
    auto ball = createForGamemode(BALL);
    auto ufo = createForGamemode(UFO);
    auto wave = createForGamemode(WAVE);
    auto robot = createForGamemode(ROBOT);
    auto spider = createForGamemode(SPIDER);
    auto swing = createForGamemode(SWING);

    cube->setPosition({15, 15});
    ship->setPosition({50, 37});
    ball->setPosition({90, 50});

    ufo->setPosition({80, 15});
    wave->setPosition({size.width-80, 15});

    robot->setPosition({size.width-90, 50});
    spider->setPosition({size.width-50, 37});
    swing->setPosition({size.width-15, 15});

    m_gamemodesMenu->addChild(cube);
    m_gamemodesMenu->addChild(ship);
    m_gamemodesMenu->addChild(ball);
    m_gamemodesMenu->addChild(ufo);
    m_gamemodesMenu->addChild(wave);
    m_gamemodesMenu->addChild(robot);
    m_gamemodesMenu->addChild(spider);
    m_gamemodesMenu->addChild(swing);
}

void TrailSettingsPopup::generateTrailCells() {
    m_scollLayer->m_contentLayer->removeAllChildren();
    CCNode* gap1 = CCNode::create();
    gap1->setContentSize({0.f, 2.5f});
    m_scollLayer->m_contentLayer->addChild(gap1);

    for (int i = 0; i < TrailType::SWING_BURST + 1; i++) {
        auto cell = TrailCell::create(this, m_selectedGamemode, static_cast<TrailType>(i));
        m_scollLayer->m_contentLayer->addChild(cell);
    }

    CCNode* gap2 = CCNode::create();
    gap2->setContentSize({0.f, 2.5f});
    m_scollLayer->m_contentLayer->addChild(gap2);

    m_scollLayer->m_contentLayer->setContentHeight(130);
    m_scollLayer->m_contentLayer->updateLayout();
    m_scollLayer->scrollToTop();
}

void TrailSettingsPopup::onGamemodeButton(CCObject* object) {
    m_selectedGamemode = static_cast<Gamemode>(object->getTag());

    generateGamemodeButtons();
    generateTrailCells();
    updateTogglers();
}

void TrailSettingsPopup::toggleForceToggler() {
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_selectedGamemode);

    if (gamemode.isTrailEnabled()) {
        m_hideTrailToggler->setOpacity(150);
        m_hideTrailToggler->setColor({150,150,150});
        m_hideTrailToggler->setEnabled(false);
    }
    else {
        m_hideTrailToggler->setOpacity(255);
        m_hideTrailToggler->setColor({255,255,255});
        m_hideTrailToggler->setEnabled(true);
    }
}

void TrailSettingsPopup::onToggleTrail(CCObject* object) {
    auto toggler = static_cast<CCMenuItemToggler*>(object);
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_selectedGamemode);
    gamemode.setTrailEnabled(!toggler->isToggled());

    toggleForceToggler();
}

void TrailSettingsPopup::onToggleDisableTrail(CCObject* object) {
    auto toggler = static_cast<CCMenuItemToggler*>(object);
    Config::get().getGamemodeData(m_selectedGamemode).setTrailForceDisabled(!toggler->isToggled());
}

void TrailSettingsPopup::updateTogglers() {
    auto& config = Config::get();
    auto& gamemode = config.getGamemodeData(m_selectedGamemode);
    m_trailToggler->toggle(gamemode.isTrailEnabled());
    m_hideTrailToggler->toggle(gamemode.isTrailForceDisabled());

    toggleForceToggler();
}
