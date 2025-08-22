#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace trail::utils {
    static void hueShift(ccColor4F& color, float hueDegrees) {
        float r = color.r;
        float g = color.g;
        float b = color.b;

        float maxC = std::max({ r, g, b });
        float minC = std::min({ r, g, b });
        float delta = maxC - minC;

        float h = 0.f, s = 0.f, v = maxC;

        if (delta > 1e-6f) {
            s = delta / maxC;

            if (maxC == r)
                h = fmodf(((g - b) / delta), 6.f);
            else if (maxC == g)
                h = ((b - r) / delta) + 2.f;
            else
                h = ((r - g) / delta) + 4.f;

            h *= 60.f;
            if (h < 0.f) h += 360.f;
        }

        h = fmodf(h + hueDegrees, 360.f);
        if (h < 0.f) h += 360.f;

        float c = v * s;
        float x = c * (1 - fabsf(fmodf(h / 60.f, 2.f) - 1));
        float m = v - c;

        float rr, gg, bb;
        if      (h < 60)  { rr = c; gg = x; bb = 0; }
        else if (h < 120) { rr = x; gg = c; bb = 0; }
        else if (h < 180) { rr = 0; gg = c; bb = x; }
        else if (h < 240) { rr = 0; gg = x; bb = c; }
        else if (h < 300) { rr = x; gg = 0; bb = c; }
        else              { rr = c; gg = 0; bb = x; }

        color.r = std::max(0.f, std::min(1.f, rr + m));
        color.g = std::max(0.f, std::min(1.f, gg + m));
        color.b = std::max(0.f, std::min(1.f, bb + m));
    }
    static ccColor3B cc4Bto3B(ccColor4B color) {
        return {color.r, color.g, color.b};
    }

    static ccColor4B cc3Bto4B(ccColor3B color, GLubyte opacity) {
        return {color.r, color.g, color.b, opacity};
    }
}
