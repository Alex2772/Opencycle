#include <cstdio>
#include "LCD2004.h"

/*
 * This code is based on https://github.com/afiskon/stm32-i2c-lcd-1602
 */

LCD2004::LCD2004(I2C_HandleTypeDef& i2cHandle, uint8_t i2cAddress):
    mI2CHandle(i2cHandle),
    mI2CAddress(i2cAddress << 1)
{
    // 4-bit mode, 2 lines, 5x7 format
    sendCommand(0b00110000);
    // display & cursor home (keep this!)
    sendCommand(0b00000010);
    // display on, right shift, underline off, blink off
    sendCommand(0b00001100);
    // clear display (optional here)
    sendCommand(0b00000001);

    char buf[0x10];
    unsigned counter = 0;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 20; ++x) {
            setPosition(x, y);
            std::sprintf(buf, "%x", counter++ % 16);
            sendString(buf);
        }
    }
}

void LCD2004::paintMainScreen(const State& state) {

}

HAL_StatusTypeDef LCD2004::sendInternal(uint8_t data, uint8_t flags) {
    HAL_StatusTypeDef res;
    for(;;) {
        res = HAL_I2C_IsDeviceReady(&mI2CHandle, mI2CAddress, 1, HAL_MAX_DELAY);
        if(res == HAL_OK)
            break;
    }

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
    data_arr[1] = up|flags|BACKLIGHT;
    data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
    data_arr[3] = lo|flags|BACKLIGHT;

    res = HAL_I2C_Master_Transmit(&mI2CHandle, mI2CAddress, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
    HAL_Delay(DELAY_MS);
    return res;
}

