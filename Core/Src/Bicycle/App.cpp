#include <cmath>
#include "App.h"
#include "Display/LCD2004.h"
#include "Config.h"

extern I2C_HandleTypeDef hi2c1;

void App::run() {
    // TODO embed to some config
    mDisplay = new LCD2004(hi2c1, LCD2004_I2C_ADDR);

    mDisplay->prePaintMainScreen(mState);

    // main loop
    for (;;) {
        // TODO async
        mDisplay->paintMainScreen(mState);

        // wait for interrupt
        __WFI();

        // calculate speed
        auto timeDeltaMs = mLastRevolutionTimestamp - mPrevRevolutionTimestamp;
        if (timeDeltaMs == 0) continue;
        const float WHEEL_LENGTH = 27.f // wheel diameter (inch)
                                 * 2.54f // convert inch to cm
                                 / 10.f  // convert cm to m
                                 * M_PI  // calculate circle length by diameter
                                 ;
        float speedMSec = WHEEL_LENGTH / (timeDeltaMs / 1000.f);
        mState.speed = speedMSec * 3.6f;
    }
}
