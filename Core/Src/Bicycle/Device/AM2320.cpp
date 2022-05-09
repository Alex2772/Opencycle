//
// Created by Alex2772 on 9/16/2021.
//

#include <Bicycle/App.h>
#include "AM2320.h"

bool AM2320::init() {
    auto i2c = App::inst().getI2C();
    return HAL_I2C_IsDeviceReady(i2c, ADDRESS, 3, MAX_DELAY) == HAL_OK;
}

void AM2320::updateState(State& state) {
    auto i2c = App::inst().getI2C();
    HAL_I2C_Master_Transmit(i2c, ADDRESS, 0, 0, MAX_DELAY);
    HAL_Delay(1);

    uint8_t registers[] = { 0x03, 0x00, 0x04 };
    if (HAL_I2C_Master_Transmit(i2c, ADDRESS, registers, sizeof(registers), MAX_DELAY) != HAL_OK){
        return;
    }

    HAL_Delay(2);

    uint8_t data[8];
    if (HAL_I2C_Master_Receive(i2c, ADDRESS, data, sizeof(data), MAX_DELAY) != HAL_OK) {
        return;
    }
    if (data[1] != 0x04 && data[0] != 0x03) {
        return;
    }
    uint16_t temperature = data[5] | data[4] << 8;
    // handle negative values
    if (temperature & 0x8000) {
        temperature = -(int16_t)(temperature & 0x7fff);
    } else {
        temperature = (int16_t)temperature;
    }
    state.temperature = float(temperature) / 10.f;
    state.humidity = float(data[3] | data[2] << 8) / 10.f;
}