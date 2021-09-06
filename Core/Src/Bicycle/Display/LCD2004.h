#pragma once

#include <stm32f4xx.h>
#include <stm32f4xx_hal_i2c.h>
#include <cassert>

#include "IDisplayDriver.h"

class LCD2004: public IDisplayDriver {
private:
    const static uint8_t PIN_RS       = (1 << 0);
    const static uint8_t PIN_EN       = (1 << 2);
    const static uint8_t BACKLIGHT    = (1 << 3);
    const static uint8_t DELAY_MS =  5;


    I2C_HandleTypeDef& mI2CHandle;
    uint8_t mI2CAddress;

    HAL_StatusTypeDef sendInternal(uint8_t data, uint8_t flags);

    HAL_StatusTypeDef sendCommand(uint8_t command) {
        return sendInternal(command, 0);
    }

    void setPosition(uint8_t x, uint8_t y) {
        assert(y <= 4 && x <= 20);
        const uint8_t rowOffsets[] = { 0x00, 0x40, 0x14, 0x54 };
        sendCommand(0b10000000 | (x + rowOffsets[y]));
    }
    void sendData(uint8_t data) {
        sendInternal(data, PIN_RS);
    }
    void sendString(const char* str) {
        for (; *str != '\0'; ++str) {
            sendData(static_cast<uint8_t>(*str));
        }
    }

public:
    LCD2004(I2C_HandleTypeDef& i2cHandle, uint8_t i2cAddress);
    void paintMainScreen(const State& state) override;

    ~LCD2004() override = default;
};


