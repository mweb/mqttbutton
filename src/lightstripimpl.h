/* -*- C++ -*- */
#pragma once

#include "lightstrip.h"

#include <NeoPixelBus.h>

enum class AnimationType { None, Fade, RotateLeft, RotateRight };

class LightStripImpl : public LightStrip {
  public:
    LightStripImpl(uint16_t count);
    virtual ~LightStripImpl();

    void setColor(uint8_t red, uint8_t green, uint8_t blue) override;
    void setColor(const RgbColor &color);
    void setColorFor(uint16_t id, uint8_t red, uint8_t green,
                     uint8_t blue) override;

    void fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                     uint32_t ticks) override;

    void rotateRight(uint32_t ticks) override;
    void rotateLeft(uint32_t ticks) override;

    void update() override;

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> m_strip;

    RgbColor m_currentColor;
    RgbColor m_nextColor;

    AnimationType m_animationType;

    uint32_t m_animationTicks;
    uint32_t m_prevTicks;

    uint16_t m_pixelCount;
};
