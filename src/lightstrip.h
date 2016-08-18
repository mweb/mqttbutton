#pragma once

#include <inttypes.h>

class LightStrip {
  public:
    virtual ~LightStrip(){};
    virtual void setColor(uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void setColorFor(uint16_t id, uint8_t red, uint8_t green,
                             uint8_t blue) = 0;

    virtual void fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                             uint16_t ticks) = 0;

    virtual void update(uint8_t ticks) = 0;
};

LightStrip *createLightStrip(uint16_t pixelCount);
