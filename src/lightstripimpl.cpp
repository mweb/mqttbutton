#include <lightstripimpl.h>

LightStripImpl::LightStripImpl(uint16_t count)
    : m_strip(count, 2), m_pixelCount(count) {
    m_strip.Begin();
    setColor(0, 0, 0);
    m_strip.Show();
}
LightStripImpl::~LightStripImpl() {}

void LightStripImpl::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    for (int16_t i = 0; i < m_pixelCount; i++) {
        m_strip.SetPixelColor(i, RgbColor(red, green, blue));
    }
}
void LightStripImpl::setColorFor(uint16_t id, uint8_t red, uint8_t green,
                                 uint8_t blue) {
    m_strip.SetPixelColor(id, RgbColor(red, green, blue));
}

void LightStripImpl::fadeToColor(uint8_t red, uint8_t green, uint8_t blue,
                                 uint16_t ticks) {}

void LightStripImpl::update(uint8_t ticks) { m_strip.Show(); }
