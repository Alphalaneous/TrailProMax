#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum TrailType : int {
    GROUND, UFO_CLICK, DASH, ROBOT_BURST, TRAILING, SHIP_CLICK, VEHICLE_GROUND, LAND, SWING_BURST
};

enum Gamemode : int {
    CUBE, SHIP, BALL, UFO, WAVE, ROBOT, SPIDER, SWING
};

static const std::string& trailTypeToString(TrailType type) {
    static std::unordered_map<TrailType, std::string> types = {
        {GROUND, "ground"},
        {UFO_CLICK, "ufo-click"},
        {DASH, "dash"},
        {ROBOT_BURST, "robot-burst"},
        {TRAILING, "trailing"},
        {SHIP_CLICK, "ship-click"},
        {VEHICLE_GROUND, "vehicle-ground"},
        {LAND, "land"},
        {SWING_BURST, "swing-burst"},
    };

    return types[type];
}


static const std::string& trailTypeToReadableString(TrailType type) {
    static std::unordered_map<TrailType, std::string> types = {
        {GROUND, "Ground"},
        {UFO_CLICK, "UFO Click"},
        {DASH, "Dash"},
        {ROBOT_BURST, "Robot Burst"},
        {TRAILING, "Trailing"},
        {SHIP_CLICK, "Ship Click"},
        {VEHICLE_GROUND, "Vehicle Ground"},
        {LAND, "Land"},
        {SWING_BURST, "Swing Burst"},
    };

    return types[type];
}

static TrailType trailTypeFromString(const std::string& str) {
    static std::unordered_map<std::string, TrailType> types = {
        {"ground", GROUND},
        {"ufo-click", UFO_CLICK},
        {"dash", DASH},
        {"robot-burst", ROBOT_BURST},
        {"trailing", TRAILING},
        {"ship-click", SHIP_CLICK},
        {"vehicle-ground", VEHICLE_GROUND},
        {"land", LAND},
        {"swing-burst", SWING_BURST},
    };

    return types[str];
}

static const std::string& gamemodeToString(Gamemode type) {
    static std::unordered_map<Gamemode, std::string> modes = {
        {CUBE, "cube"},
        {SHIP, "ship"},
        {BALL, "ball"},
        {UFO, "ufo"},
        {WAVE, "wave"},
        {ROBOT, "robot"},
        {SPIDER, "spider"},
        {SWING, "swing"},
    };

    return modes[type];
}

static const std::string& gamemodeToStringRob(Gamemode type) {
    static std::unordered_map<Gamemode, std::string> modes = {
        {CUBE, "icon"},
        {SHIP, "ship"},
        {BALL, "ball"},
        {UFO, "bird"},
        {WAVE, "dart"},
        {ROBOT, "robot"},
        {SPIDER, "spider"},
        {SWING, "swing"},
    };

    return modes[type];
}

static Gamemode gamemodeFromString(const std::string& str) {
    static std::unordered_map<std::string, Gamemode> modes = {
        {"cube", CUBE},
        {"ship", SHIP},
        {"ball", BALL},
        {"ufo", UFO},
        {"wave", WAVE},
        {"robot", ROBOT},
        {"spider", SPIDER},
        {"swing", SWING},
    };

    return modes[str];
}

struct DefaultColors {
	ccColor4F start;
	ccColor4F end;
};

static const DefaultColors& defaultColorsForType(TrailType type) {
    static std::unordered_map<TrailType, DefaultColors> defaults = {
        { GROUND,  {{ 0.3882353, 0.24705882, 0.5803922, 1 }, { 0, 0, 0, 1 }} },
        { UFO_CLICK, { { 0.3882353, 0.24705882, 0.5803922, 1 }, { 0.3882353, 0.24705882, 0.5803922, 1 } } },
        { DASH, { { 1, 0.98039216, 0.49803922, 1 }, { 1, 0.98039216, 0.49803922, 0.5 } } },
        { ROBOT_BURST, { { 1, 0, 0, 1 }, { 1, 1, 0, 1 } } },
        { TRAILING, { { 1, 0.39215687, 0, 1 }, { 1, 0, 0, 1 } } },
        { SHIP_CLICK, { { 1, 0.74509805, 0, 1 }, { 1, 0, 0, 1 } } },
        { VEHICLE_GROUND, { { 1, 1, 1, 1 }, { 0, 0, 0, 1 } } },
        { LAND, { { 0.3882353, 0.24705882, 0.5803922, 1 }, { 0, 0, 0, 1 } } },
        { SWING_BURST, { { 1, 0.39215687, 0, 1 }, { 1, 0, 0, 1 } } },
    };

    return defaults[type];
}

static bool defaultTrailStateForGamemode(Gamemode gamemode) {
    static std::unordered_map<Gamemode, bool> defaults = {
        {CUBE, false},
        {SHIP, true},
        {BALL, false},
        {UFO, true},
        {WAVE, true},
        {ROBOT, false},
        {SPIDER, false},
        {SWING, true},
    };

    return defaults[gamemode];
}

class ParticleTrailData {
protected:
    bool m_particlesEnabled = true;
    bool m_colorEnabled = false;
    bool m_rainbowEnabled = false;
    ccColor4F m_startColor;
    ccColor4F m_endColor;
    ccColor4F m_rainbowColor = {1, 0, 0, 1};
    float m_hueShift = 240.f;
    float m_currentHue = 0;
public:

    void setStartColor(const ccColor4B& particleColor) {
        m_startColor = ccc4FFromccc4B(particleColor);
    }

    void setStartColor(const ccColor4F& particleColor) {
        m_startColor = particleColor;
    }

    ccColor4F getStartColor() {
        return m_startColor;
    }

    void setEndColor(const ccColor4B& particleColor) {
        m_endColor = ccc4FFromccc4B(particleColor);
    }

    void setEndColor(const ccColor4F& particleColor) {
        m_endColor = particleColor;
    }

    ccColor4F getEndColor() {
        return m_endColor;
    }

    void setRainbowColor(const ccColor4B& particleColor) {
        m_rainbowColor = ccc4FFromccc4B(particleColor);
    }

    void setRainbowColor(const ccColor4F& particleColor) {
        m_rainbowColor = particleColor;
    }

    ccColor4F getRainbowColor() {
        return m_rainbowColor;
    }

    void setParticlesEnabled(bool enabled) {
        m_particlesEnabled = enabled;
    }

    bool isParticlesEnabled() {
        return m_particlesEnabled;
    }

    void setColorEnabled(bool enabled) {
        m_colorEnabled = enabled;
    }

    bool isColorEnabled() {
        return m_colorEnabled;
    }

    void setRainbowEnabled(bool enabled) {
        m_rainbowEnabled = enabled;
    }

    bool isRainbowEnabled() {
        return m_rainbowEnabled;
    }

    void setHueShift(float shift) {
        m_hueShift = shift;
    }

    float getHueShift() {
        return m_hueShift;
    }

    void setCurrentHue(float hue) {
        m_currentHue = hue;
    }

    float getCurrentHue() {
        return m_currentHue;
    }

    void setColorDefaults(DefaultColors defaults) {
        m_startColor = defaults.start;
        m_endColor = defaults.end;
        m_colorEnabled = false;
        m_rainbowEnabled = false;
        m_rainbowColor = {1, 0, 0, 1};
        m_hueShift = 240;
    }

    matjson::Value getSaveValue() const {
        matjson::Value value;
        value["color-enabled"] = m_colorEnabled;
        value["rainbow-enabled"] = m_rainbowEnabled;
        value["particles-enabled"] = m_particlesEnabled;
        value["start-color"] = ccc4BFromccc4F(m_startColor);
        value["end-color"] = ccc4BFromccc4F(m_endColor);
        value["rainbow-color"] = ccc4BFromccc4F(m_rainbowColor);
        value["hue-shift"] = m_hueShift;
        return value;
    }

    void loadFromValue(matjson::Value value) {
        m_colorEnabled = value["color-enabled"].asBool().unwrapOrDefault();
        m_rainbowEnabled = value["rainbow-enabled"].asBool().unwrapOrDefault();
        m_particlesEnabled = value["particles-enabled"].asBool().unwrapOrDefault();
        m_startColor = ccc4FFromccc4B(value["start-color"].as<ccColor4B>().unwrapOrDefault());
        m_endColor = ccc4FFromccc4B(value["end-color"].as<ccColor4B>().unwrapOrDefault());
        m_rainbowColor = ccc4FFromccc4B(value["rainbow-color"].as<ccColor4B>().unwrapOrDefault());
        m_hueShift = value["hue-shift"].asDouble().unwrapOrDefault();
    }
};

class GamemodeData {
protected:
    bool m_trailEnabled;
    bool m_trailForceDisabled = false;
    std::unordered_map<TrailType, ParticleTrailData> m_particleTrailData = {
        {GROUND, {}},
        {UFO_CLICK, {}},
        {DASH, {}},
        {ROBOT_BURST, {}},
        {TRAILING, {}},
        {SHIP_CLICK, {}},
        {VEHICLE_GROUND, {}},
        {LAND, {}},
        {SWING_BURST, {}},
    };

public:
    void setDefaults(TrailType type) {
        m_particleTrailData[type].setColorDefaults(defaultColorsForType(type));
    }

    void setTrailEnabled(bool enabled) {
        m_trailEnabled = enabled;
    }

    bool isTrailEnabled() {
        return m_trailEnabled;
    }

    void setTrailForceDisabled(bool disabled) {
        m_trailForceDisabled = disabled;
    }

    bool isTrailForceDisabled() {
        return m_trailForceDisabled;
    }

    ParticleTrailData& getTrailData(TrailType type) {
        return m_particleTrailData[type];
    }

    matjson::Value getSaveValue() const {
        matjson::Value value;
        value["trail-enabled"] = m_trailEnabled;
        value["trail-force-disabled"] = m_trailForceDisabled;
        for (const auto& [k, v] : m_particleTrailData) {
            const std::string& name = trailTypeToString(k);
            value[name] = v.getSaveValue();
        }
        return value;
    }

    void setColorDefaults() {
        for (auto& [k, v] : m_particleTrailData) {
            v.setColorDefaults(defaultColorsForType(k));
        }
    }

    void loadFromValue(matjson::Value value) {
        m_trailEnabled = value["trail-enabled"].asBool().unwrapOrDefault();
        m_trailForceDisabled = value["trail-force-disabled"].asBool().unwrapOrDefault();
        for (const auto& [k, v] : value) {
            if (!v.isObject()) continue;
            TrailType type = trailTypeFromString(k);
            m_particleTrailData[type].loadFromValue(v);
        }
    } 
};

class Config {

protected:
    std::unordered_map<Gamemode, GamemodeData> m_gamemodeData = {
        {CUBE, {}},
        {SHIP, {}},
        {BALL, {}},
        {UFO, {}},
        {WAVE, {}},
        {ROBOT, {}},
        {SPIDER, {}},
        {SWING, {}},
    };

public:
    static Config& get() {
        static Config instance;
        return instance;
    }

    void load() {
        if (Mod::get()->getSaveContainer().size() == 0) {
            for (auto& [k, v] : m_gamemodeData) {
                v.setColorDefaults();
                v.setTrailEnabled(defaultTrailStateForGamemode(k));
            }
        }
        else {
            for (const auto& [k, v] : Mod::get()->getSaveContainer()) {
                if (!v.isObject()) continue;
                Gamemode gamemode = gamemodeFromString(k);
                m_gamemodeData[gamemode].loadFromValue(v);
            }
        }
    }

    void save() {
        auto& saveContainer = Mod::get()->getSaveContainer();
        for (const auto& [k, v] : m_gamemodeData) {
            const std::string& name = gamemodeToString(k);
            saveContainer[name] = v.getSaveValue();
        }
    }

    GamemodeData& getGamemodeData(Gamemode gamemode) {
        return m_gamemodeData[gamemode];
    }
};