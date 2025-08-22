#include "RGBPopup.hpp"

RGBPopup* RGBPopup::create(SimpleColorPicker* picker) {
    auto ret = new RGBPopup();
    if (ret->initAnchored(100.f, 140.f, picker, "geode.loader/GE_square02.png")) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool RGBPopup::setup(SimpleColorPicker* picker) {
    setTitle("RGB Input");

    m_colorPicker = picker;
    m_color = picker->getColor();

    CCNode* contentNode = CCNode::create();
    contentNode->ignoreAnchorPointForPosition(false);
    contentNode->setContentSize({m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight() - 40});
    contentNode->setAnchorPoint({0.5f, 0.5f});
    contentNode->setPosition({m_mainLayer->getContentWidth()/2, m_mainLayer->getContentHeight()/2 - 10});

    m_mainLayer->addChild(contentNode);

    contentNode->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::End)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setGap(2.5f)
    );

    contentNode->addChild(createInput(m_color.r, "R:", [this] (GLubyte byte) {
        m_color.r = byte;
        m_colorPicker->colorValueChanged(m_colorPicker->cc4Bto3B(m_color));
    }));
    contentNode->addChild(createInput(m_color.g, "G:", [this] (GLubyte byte) {
        m_color.g = byte;
        m_colorPicker->colorValueChanged(m_colorPicker->cc4Bto3B(m_color));
    }));
    contentNode->addChild(createInput(m_color.b, "B:", [this] (GLubyte byte) {
        m_color.b = byte;
        m_colorPicker->colorValueChanged(m_colorPicker->cc4Bto3B(m_color));
    }));

    contentNode->updateLayout();

    return true;
};

CCNode* RGBPopup::createInput(GLubyte value, const std::string& labelStr, std::function<void(GLubyte byte)> fn) {

    auto node = CCNode::create();
    node->ignoreAnchorPointForPosition(false);
    node->setAnchorPoint({0.5f, 0.5f});
    node->setContentSize({90, 30});

    auto size = node->getContentSize();

    auto label = CCLabelBMFont::create(labelStr.c_str(), "bigFont.fnt");
    label->setScale(0.5f);
    label->setAnchorPoint({1.f, 0.5f});

    node->addChild(label);

    geode::TextInput* input = geode::TextInput::create(60, "255");
    input->setCommonFilter(CommonFilter::Int);
    input->setMaxCharCount(3);
    #ifdef GEODE_IS_ANDROID
    input->setMaxCharCount(4);
    #else
    input->setMaxCharCount(3);
    #endif

    input->setString(numToString(value));
    input->setScale(0.7f);
    input->setCallback([this, input, fn] (const std::string& str) {
        auto res = numFromString<int>(str);
        int newValue = std::clamp(res.unwrapOr(255), 0, 255);
        if (!input->getString().empty() && res.isOk()) {
            input->setString(numToString(newValue));
        }
        fn(newValue);
    });

    input->setPosition({size.width - input->getScaledContentWidth()/2 - 15, 15});
    label->setPosition({input->getPositionX() - input->getScaledContentWidth()/2 - 4, 15});

    node->addChild(input);

    return node;
}