/* -*- C++ -*- */
#pragma once

#include "lightstrip.h"

#include <NeoPixelBus.h>

class LightStripImpl : public LightStrip {
  public:
    LightStripImpl(uint16_t count);
    ~LightStripImpl();

    virtual void setColor(uint8_t red, uint8_t green, uint8_t blue);
    virtual void setColorFor(uint16_t id, uint8_t red, uint8_t green,
                             uint8_t blue);

    virtual void fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                             uint16_t ticks);

    virtual void update(uint8_t ticks);

  private:
    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> m_strip;
    uint16_t m_pixelCount;
};
