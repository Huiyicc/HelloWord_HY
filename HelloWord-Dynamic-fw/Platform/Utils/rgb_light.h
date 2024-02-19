#ifndef HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H
#define HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H

#include "spi.h"

struct Color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color_t(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}

    explicit Color_t(const uint32_t& _color) {
      r = (_color >> 16) & 0xFF;
      g = (_color >> 8) & 0xFF;
      b = _color & 0xFF;
    }

    [[nodiscard]] int GetColor() const {
      return (r << 16) | (g << 8) | b;
    }

};

class RGB {
public:
    explicit RGB(SPI_HandleTypeDef *_spi) :
        spiHandle(_spi) {
    }



    enum SpiWs2812Byte_t : uint8_t {
        WS_HIGH = 0xF8,
        WS_LOW = 0x80
    };
    static const uint8_t LED_NUMBER = 4;
    volatile bool isRgbTxBusy;

    void SetRgbBuffer(uint8_t _keyId, Color_t _color, float _brightness);

    void SetRgbBuffer(Color_t _color, float _brightness);

    void SyncLights();

private:
    SPI_HandleTypeDef *spiHandle;
    uint8_t rgbBuffer[LED_NUMBER][3][8]{};
    uint8_t wsCommit[64] = {0};
    uint8_t brightnessPrediv = 2;

};

#endif //HELLOWORD_DYNAMIC_FW_RGB_LIGHT_H
