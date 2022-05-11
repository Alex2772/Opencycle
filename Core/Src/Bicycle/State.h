#pragma once

#include <cstdint>

struct State {
    /**
     * Speed in km/h.
     */
    double currentSpeed = 0;

    /**
     * Speed of previous revolution in km/h.
     */
    double prevRevolutionSpeed = 0.f;

    /**
     * Overall distance on this session. (m)
     */
    double distance = 0;

    /**
     * Consumed electric power on this session. (kWh/h)
     */
    double consumedPowerWhh = 0;

    double temperature = 0;
    double humidity = 0;

    std::uint16_t motorPower = 0;
    std::uint8_t motorTemperature = 0;
    bool isThrottling = false;
};