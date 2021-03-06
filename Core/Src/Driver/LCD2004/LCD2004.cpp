#include <cstdio>
#include <cmath>
#include <Bicycle/App.h>
#include "LCD2004.h"
#include "Bicycle/Calendar.h"

extern "C" TIM_HandleTypeDef htim1;
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

    res = HAL_I2C_IsDeviceReady(&mI2CHandle, mI2CAddress, 1, MAX_DELAY);
    if(res != HAL_OK) {
        App::inst().reboot();
    }

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|BACKLIGHT|flags|PIN_EN;
    data_arr[1] = up|BACKLIGHT|flags;
    data_arr[2] = lo|BACKLIGHT|flags|PIN_EN;
    data_arr[3] = lo|BACKLIGHT|flags;

    res = HAL_I2C_Master_Transmit(&mI2CHandle, mI2CAddress, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
    HAL_Delay(DELAY_MS);
    return res;
}

void LCD2004::prePaintMainScreen(const State& state) {
    // numeric speed display block
    {
        // point for speed display
        print(18, 2, '.');

        // unit display
        print(18, 0, "km");
        print(19, 1, 'h');
    }

    print(2, 0, "oC");
    print(2, 1, "%");

    // visual speed display block
    {
        print(0, 3, '[');
        print(19, 3, ']');
        //print(3, 2, "10| 20| 30|");
        //print(1, 2, "5| 15| 25|");
    }
}

void LCD2004::paintMainScreen(const State& state) {
    // numeric speed display block

    {
        // display speed
        int digitAfterPoint = int(std::fmod(state.currentSpeed * 10.0, 10.0));
        int digit0 = int(std::fmod(state.currentSpeed, 10.0));
        int digit00 = int(std::fmod(state.currentSpeed / 10.0, 10.0));

        // lowest digit
        printBigDigit(16, 0, digit0);

        // higher digit, we don't need to display zero
        if (digit00 != 0) {
            printBigDigit(14, 0, digit00);
        } else {
            clearRect(14, 0, 2, 3);
        }

        // digit after point
        {
            char buf[4];
            std::sprintf(buf, "%d", digitAfterPoint);
            print(19, 2, buf);
        }
    }

    // visual speed display block
    //[#############             ]

    setPosition(1, 3);
    int threshold = int((state.currentSpeed - 1) / 5.0 * 2.0 * 2.0);
/*
    {
        // bullshit here because of slow i2c controller and we decided to make a feature from bug by creating a transition.
        {
            bool speedIncreased = state.prevSpeed < state.currentSpeed;

            for (unsigned i = speedIncreased ? 0 : 17; i < 18; speedIncreased ? ++i : --i) {
                if (!speedIncreased) {
                    setPosition(i + 1, 3);
                }
                int delta = i * 2 - threshold;
                if (delta < 0) {
                    sendData(BLACK_BLOCK);
                } else if (delta == 0 && state.currentSpeed > 0.01f) {
                    sendData(GRAY_BLOCK);
                } else {
                    sendData(EMPTY_BLOCK);
                }
                HAL_Delay(5);
            }
        }
    }
*/
    // paint temperature
    {
        char buf[64];
        sprintf(buf, "%2.0f", state.temperature);
        print(0, 0, buf);
        sprintf(buf, "%2.0f", state.humidity);
        print(0, 1, buf);
    }

    // distance
    {
        char buf[64];
        sprintf(buf, "%lum", std::uint32_t(state.distance));
        print(4, 1, buf);
    }

    // e-bike indicators
    {
        char buf[64];
        sprintf(buf, "%04dW %.1fWh/h", state.motorPower, state.consumedPowerWhh);
        print(0, 2, buf);
    }

    // paint clock
    {
        char buf[64];
        auto currentTime = Calendar::get();
        sprintf(buf, "% 2lu:%02lu", currentTime.hours, currentTime.minutes);
        print(9, 0, buf);
    }

    // light and throttling
    {
        char buf[8];
        sprintf(buf, "%c%c", state.isThrottling ? 'T' : ' ', App::inst().isLightEnabled() ? 'L' : ' ');
        print(6, 0, buf);
    }
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
    const uint8_t NO = EMPTY_BLOCK;

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

void LCD2004::clearRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    for (uint8_t iY = 0; iY < height; ++iY) {
        setPosition(x, y + iY);
        for (uint8_t iX = 0; iX < width; ++iX) {
            sendData(EMPTY_BLOCK);
        }
    }
}

void LCD2004::paintListMenu(const ListMenu::State& state) {
    int selectedItemIndex = state.selectedItem - state.items.begin();
    auto firstDisplayedItemIndex = std::clamp(selectedItemIndex - 1, 0, int(state.items.size() - 1));
    for (int i = 0; i < 4; ++i) {
        auto index = firstDisplayedItemIndex + i;
        if (index >= state.items.size()) break;
        print(1, i, state.items[index].name.c_str());
    }
    print(0, selectedItemIndex - firstDisplayedItemIndex, '>');
}

void LCD2004::paintValueChangingMenu(std::string_view str) {
    print(10, 1, '^');
    print(10 - str.length() / 2, 2, str.data());
    print(10, 3, 'v');
}

