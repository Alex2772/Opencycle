#pragma once

#include <cmath>
#include <cstdint>
#include "Display/IDisplayDriver.h"

class App {
private:
    IDisplayDriver* mDisplay;
    State mState;

    App() = default;

    uint16_t getMsCounterValueAndReset();

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

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
};