#pragma once


#include <stm32f4xx_hal.h>
#include <optional>
#include <cstdint>
#include <Bicycle/IDevice.h>

/**
 * Driver for temperature + humidity sensor
 */
class AM2320: public IDevice {
private:
    static const uint8_t  ADDRESS   = 0x5c << 1;
    const static uint32_t MAX_DELAY = 100;

public:
    bool init() override;

    ~AM2320() override = default;

    void updateState(State& state) override;

};


