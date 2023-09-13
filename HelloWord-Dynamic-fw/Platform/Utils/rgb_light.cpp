#include "rgb_light.h"


void RGB::SetRgbBuffer(uint8_t _keyId, RGB::Color_t &_color) {
    for (int i = 0; i < 8; i++) {
        uint8_t t = static_cast<uint8_t>(_color.g * brightnessFactor);
        rgbBuffer[_keyId][0][i] = t & (0x80 >> i) ? WS_HIGH : WS_LOW;
        t = static_cast<uint8_t>(_color.r * brightnessFactor);
        rgbBuffer[_keyId][1][i] = t & (0x80 >> i) ? WS_HIGH : WS_LOW;
        t = static_cast<uint8_t>(_color.b * brightnessFactor);
        rgbBuffer[_keyId][2][i] = t & (0x80 >> i) ? WS_HIGH : WS_LOW;
    }
}

void RGB::SetLights(float brightness) {
    if (brightness >= 0) {
        brightnessFactor = brightness;
    }
}

void RGB::SyncLights() {
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi3, (uint8_t *) rgbBuffer, LED_NUMBER * 3 * 8);
    while (isRgbTxBusy);
    isRgbTxBusy = true;
    HAL_SPI_Transmit_DMA(&hspi3, wsCommit, 64);
}