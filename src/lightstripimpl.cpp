#include "lightstripimpl.h"

#include <Arduino.h>

LightStripImpl::LightStripImpl(uint16_t count)
    : m_strip(count, 2), m_pixelCount(count) {
    m_strip.Begin();
    setColor(0, 0, 0);
    m_strip.Show();
}
LightStripImpl::~LightStripImpl() {}

void LightStripImpl::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    setColor(RgbColor(red, green, blue));
    m_animationType = AnimationType::None;
    m_animationTicks = 0;
}
void LightStripImpl::setColorFor(uint16_t id, uint8_t red, uint8_t green,
                                 uint8_t blue) {
    m_strip.SetPixelColor(id, RgbColor(red, green, blue));
    m_animationType = AnimationType::None;
    m_animationTicks = 0;
}

void LightStripImpl::setColor(const RgbColor &color) {
    m_currentColor.R = color.R;
    m_currentColor.G = color.G;
    m_currentColor.B = color.B;

    for (int16_t i = 0; i < m_pixelCount; i++) {
        m_strip.SetPixelColor(i, m_currentColor);
    }
}

void LightStripImpl::fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                                 uint32_t ticks) {
    m_nextColor.R = red;
    m_nextColor.G = green;
    m_nextColor.B = blue;

    m_animationType = AnimationType::Fade;
    m_prevTicks = millis();

    m_animationTicks = ticks;
}

void LightStripImpl::rotateRight(uint32_t ticks) {
    m_animationType = AnimationType::RotateRight;
    m_animationTicks = ticks;
    m_prevTicks = millis();
}

void LightStripImpl::rotateLeft(uint32_t ticks) {
    m_animationType = AnimationType::RotateLeft;
    m_animationTicks = ticks;
    m_prevTicks = millis();
}

void LightStripImpl::update() {
    switch (m_animationType) {
    case AnimationType::Fade: {
        uint32_t delta = millis() - m_prevTicks;
        if (delta >= m_animationTicks) {
            setColor(m_nextColor);
        } else {
            float progress = static_cast<float>(delta) /
                             static_cast<float>(m_animationTicks);
            setColor(
                RgbColor::LinearBlend(m_currentColor, m_nextColor, progress));
        }
        break;
    }
    case AnimationType::RotateRight: {
        uint32_t delta = millis() - m_prevTicks;
        if (m_animationTicks < delta) {
            m_strip.RotateRight(1);
            m_prevTicks = millis();
        }
        break;
    }
    case AnimationType::RotateLeft: {
        uint32_t delta = millis() - m_prevTicks;
        if (m_animationTicks < delta) {
            m_strip.RotateLeft(1);
            m_prevTicks = millis();
        }
        break;
    }
    case AnimationType::None:
    default: { break; }
    }
    m_strip.Show();
}
