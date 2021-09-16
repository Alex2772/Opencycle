#pragma once

#include <cstdint>
#include "Display/IDisplayDriver.h"

class App {
private:
    IDisplayDriver* mDisplay;
    State mState;
    uint32_t mTimer = 0;
    uint32_t mPrevRevolutionTimestamp = 0;
    uint32_t mLastRevolutionTimestamp = 0;

    App() = default;

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

    inline void onWheelRevolution() {
        mPrevRevolutionTimestamp = mLastRevolutionTimestamp;
        mLastRevolutionTimestamp = mTimer;
    }

    inline void onMillisecondTimerElapsed() {
        mTimer += 1;
    }
};