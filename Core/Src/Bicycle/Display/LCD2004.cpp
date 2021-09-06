#include <cstdio>
#include "LCD2004.h"

/*
 * This code is based on following projects:
 *
 * https://github.com/afiskon/stm32-i2c-lcd-1602
 * https://github.com/johnrickman/LiquidCrystal_I2C
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

    createCustomChar(0, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00111,
        0b00100,
    });

    createCustomChar(1, {
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11100,
        0b00100,
    });

    createCustomChar(2, {
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00111,
            0b00100,
    });

    createCustomChar(3, {
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b11100,
        0b00100,
    });


    createCustomChar(4, {
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00111,
            0b00000,
    });

    createCustomChar(5, {
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b00100,
            0b11100,
            0b00000,
    });
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

void LCD2004::paintMainScreen(const State& state) {
    //printBigDigit(17, 0, 0);

    printBigDigit(0, 0, 0);
    printBigDigit(2, 0, 1);
    printBigDigit(4, 0, 2);
    printBigDigit(6, 0, 3);
    printBigDigit(8, 0, 4);
    printBigDigit(10, 0, 5);
    printBigDigit(12, 0, 6);
    printBigDigit(14, 0, 7);
    printBigDigit(16, 0, 8);
    printBigDigit(18, 0, 9);
}

void LCD2004::createCustomChar(uint8_t slot, const std::array<uint8_t, 8>& data) {
    assert((slot < 8 && "LCD2004 supports only 8 slots of custom chars"));
    sendCommand(SETCGRAMADDR | (slot << 3));
    for (auto& c : data) {
        sendData(c);
    }
}

void LCD2004::printBigDigit(uint8_t x, uint8_t y, uint8_t digitValue) {
    const uint8_t TL = CUSTOM_CHAR_1;
    const uint8_t TR = CUSTOM_CHAR_2;
    const uint8_t ML = CUSTOM_CHAR_3;
    const uint8_t MR = CUSTOM_CHAR_4;
    const uint8_t BL = CUSTOM_CHAR_5;
    const uint8_t BR = CUSTOM_CHAR_6;
    const uint8_t SR = 0b10100011;
    const uint8_t VL = 0b1111100;
    const uint8_t HL = 0b1011111;
    const uint8_t NO = ' ';

    uint8_t digits[10][6] = {
        // 0
        {
            TL, TR,
            VL, VL,
            BL, BR,
        },

        // 1
        {
            NO, TR,
            NO, VL,
            NO, VL,
        },

        // 2
        {
            TL, TR,
            TL, BR,
            BL, SR,
        },

        // 3
        {
            HL, TR,
            HL, MR,
            HL, BR,
        },

        // 4
        {
            NO, NO,
            BL, MR,
            NO, VL,
        },

        // 5
        {
            TL, HL,
            BL, TR,
            HL, BR,
        },

        // 6
        {
            TL, TR,
            ML, TR,
            BL, BR,
        },

        // 7
        {
            TL, TR,
            NO, VL,
            NO, VL,
        },

        // 8
        {
            TL, TR,
            ML, MR,
            BL, BR,
        },

        // 9
        {
            TL, TR,
            BL, MR,
            NO, BR,
        },
    };

    setPosition(x, y);
    sendData(digits[digitValue][0]);
    sendData(digits[digitValue][1]);

    setPosition(x, y + 1);
    sendData(digits[digitValue][2]);
    sendData(digits[digitValue][3]);

    setPosition(x, y + 2);
    sendData(digits[digitValue][4]);
    sendData(digits[digitValue][5]);
}

