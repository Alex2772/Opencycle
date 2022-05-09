#pragma once

#include <cmath>
#include <cstdint>
#include <stm32f4xx_hal.h>
#include <array>
#include "State.h"
#include "Input.h"
#include "IDisplayDriver.h"

class App {
private:
    IDisplayDriver* mDisplay;
    I2C_HandleTypeDef mI2C;
    State mState;

    App();

    uint16_t getMsCounterValueAndReset();

    std::array<KeyState, 4> mKeyStates;

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

    bool isKeyDown(Key key) {
        return mKeyStates[static_cast<int>(key)] == KeyState::PRESSED;
    }

    inline void onSensorReportedWheelRevolution() {
        // check for bounce

        // calculate speed
        auto timeDeltaMs = getMsCounterValueAndReset();
        if (timeDeltaMs < 2) return;
        const float WHEEL_LENGTH = 27.f // wheel diameter (inch)
                                   * 2.54f // convert inch to cm
                                   / 100.f  // convert cm to m
                                   * M_PI  // calculate circle length by diameter
        ;
        float speedMSec = WHEEL_LENGTH / (timeDeltaMs / 1000.f);
        mState.prevRevolutionSpeed = mState.currentSpeed;
        mState.currentSpeed = speedMSec * 3.6f;
    }

    inline void onWheelStopped() {
        mState.currentSpeed = 0;
    }

    [[nodiscard]]
    I2C_HandleTypeDef* getI2C() {
        return &mI2C;
    }

    void onInput(Key key, KeyState state);

    void reboot();
};