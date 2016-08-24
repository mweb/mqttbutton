#pragma once

#include <inttypes.h>

/**
 * Access The Lightstrip, set color and add simple animations.
 */
class LightStrip {
  public:
    virtual ~LightStrip(){};
    /**
     * Set the color for all LEDs. The value for each color must have a values
     * between 0 and 128.
     */
    virtual void setColor(uint8_t red, uint8_t green, uint8_t blue) = 0;
    /**
    * Set the color for one LED. The value for each color must have a values
    * between 0 and 128. The Index starts with 0.
     */
    virtual void setColorFor(uint16_t id, uint8_t red, uint8_t green,
                             uint8_t blue) = 0;

    /**
     * Fade all LEDs from the previous set color with setColor to the new color
     * Use the number of miliseconds (ticks) to fade from the start color to
     * the destination color.
     */
    virtual void fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                             uint32_t ticks) = 0;

    /**
     * Rotate the current LED colors to the right. Wait between the rotate steps
     * the given number ob miliseconds (ticks).
     */
    virtual void rotateRight(uint32_t ticks) = 0;
    /**
     * Rotate the current LED colors to the left. Wait between the rotate steps
     * the given number ob miliseconds (ticks).
     */
    virtual void rotateLeft(uint32_t ticks) = 0;

    /**
     * Update all colors according to the current animation.
     */
    virtual void update() = 0;
};

/**
 * Create a new light strip object with the given number of LEDs.
 */
LightStrip *createLightStrip(uint16_t pixelCount);
