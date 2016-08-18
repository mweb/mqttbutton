#include "lightstrip.h"
#include "lightstripimpl.h"

// LightStripImpl impl(12);

LightStrip *createLightStrip(uint16_t pixelCount) {
    return new LightStripImpl(pixelCount);
}
