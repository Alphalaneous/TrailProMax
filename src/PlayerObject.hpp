#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "Config.hpp"
#include "Utils.hpp"

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {

    void update(float p0) {
        PlayerObject::update(p0);
        if (!PlayLayer::get()) return;

        if (m_isShip) checkSetting(Gamemode::SHIP);
        else if (m_isBird) checkSetting(Gamemode::UFO);
        else if (m_isBall) checkSetting(Gamemode::BALL);
        else if (m_isDart) checkSetting(Gamemode::WAVE);
        else if (m_isRobot) checkSetting(Gamemode::ROBOT);
        else if (m_isSpider) checkSetting(Gamemode::SPIDER);
        else if (m_isSwing) checkSetting(Gamemode::SWING);
        else checkSetting(Gamemode::CUBE);
    }

    void checkSetting(Gamemode gamemode) {
        setStreak(gamemode);
        checkParticlesEnabled(gamemode);
        checkParticlesColor(gamemode);
    }

    void setStreak(Gamemode gamemode) {
        if (!PlayLayer::get() || !isVisible()) return;
        queueInMainThread([self = Ref(this), gamemode] {
            auto& gamemodeData = Config::get().getGamemodeData(gamemode);
            if (gamemodeData.isTrailEnabled()) {
                self->activateStreak();
                return;
            }
            if (gamemodeData.isTrailForceDisabled() || defaultTrailStateForGamemode(gamemode)) {
                self->deactivateStreak(true);
            }
        });
    }

    bool getParticleEnabled(Gamemode gamemode, TrailType type) {
        auto& config = Config::get();
        auto& gamemodeData = config.getGamemodeData(gamemode);
        auto& trailData = gamemodeData.getTrailData(type);
        return trailData.isParticlesEnabled();
    }

    void checkParticlesEnabled(Gamemode gamemode) {
        m_playerGroundParticles->setVisible(getParticleEnabled(gamemode, TrailType::GROUND));
        m_ufoClickParticles->setVisible(getParticleEnabled(gamemode, TrailType::UFO_CLICK));
        m_dashParticles->setVisible(getParticleEnabled(gamemode, TrailType::DASH));
        m_robotBurstParticles->setVisible(getParticleEnabled(gamemode, TrailType::ROBOT_BURST));
        m_trailingParticles->setVisible(getParticleEnabled(gamemode, TrailType::TRAILING));
        m_shipClickParticles->setVisible(getParticleEnabled(gamemode, TrailType::SHIP_CLICK));
        m_vehicleGroundParticles->setVisible(getParticleEnabled(gamemode, TrailType::VEHICLE_GROUND));
        m_landParticles0->setVisible(getParticleEnabled(gamemode, TrailType::LAND));
        m_landParticles1->setVisible(getParticleEnabled(gamemode, TrailType::LAND));
        m_swingBurstParticles1->setVisible(getParticleEnabled(gamemode, TrailType::SWING_BURST));
        m_swingBurstParticles2->setVisible(getParticleEnabled(gamemode, TrailType::SWING_BURST));
    }

    void checkParticlesColor(Gamemode gamemode) {
        setParticleColor(m_playerGroundParticles, gamemode, TrailType::GROUND);
        setParticleColor(m_ufoClickParticles, gamemode, TrailType::UFO_CLICK);
        setParticleColor(m_dashParticles, gamemode, TrailType::DASH);
        setParticleColor(m_robotBurstParticles, gamemode, TrailType::ROBOT_BURST);
        setParticleColor(m_trailingParticles, gamemode, TrailType::TRAILING);
        setParticleColor(m_shipClickParticles, gamemode, TrailType::SHIP_CLICK);
        setParticleColor(m_vehicleGroundParticles, gamemode, TrailType::VEHICLE_GROUND);
        setParticleColor(m_landParticles0, gamemode, TrailType::LAND);
        setParticleColor(m_landParticles1, gamemode, TrailType::LAND);
        setParticleColor(m_swingBurstParticles1, gamemode, TrailType::SWING_BURST);
        setParticleColor(m_swingBurstParticles2, gamemode, TrailType::SWING_BURST);
    }

    void setParticleColor(CCParticleSystemQuad* particles, Gamemode gamemode, TrailType type) {
        auto& config = Config::get();
        auto& gamemodeData = config.getGamemodeData(gamemode);
        auto& trailData = gamemodeData.getTrailData(type);

        if (!trailData.isParticlesEnabled()) {
            const DefaultColors& colors = defaultColorsForType(type);
            particles->setStartColor(colors.start);
            particles->setEndColor(colors.end);
            return;
        }

        if (trailData.isRainbowEnabled()) {
            if (trailData.getCurrentHue() >= 360) trailData.setCurrentHue(0);
            ccColor4F color = trailData.getRainbowColor();
            trail::utils::hueShift(color, trailData.getCurrentHue());
            particles->setStartColor(color);
            particles->setEndColor(color);
            trailData.setCurrentHue(trailData.getCurrentHue() + trailData.getHueShift()/240.f);
        }
        else if (trailData.isColorEnabled()){
            ccColor4F start = trailData.getStartColor();
            ccColor4F end = trailData.getEndColor();
            particles->setStartColor(start);
            particles->setEndColor(end);
        }
        else {
            DefaultColors defaults = defaultColorsForType(type);
            particles->setStartColor(defaults.start);
            particles->setEndColor(defaults.end);
        }
    }
};
