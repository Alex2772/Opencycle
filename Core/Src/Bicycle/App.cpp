#include "App.h"
#include "Display/LCD2004.h"
#include "Config.h"

extern I2C_HandleTypeDef hi2c1;

extern "C" void app_run() {
    App::inst().run();
}

void App::run() {
    // TODO embed to some config
    mDisplay = new LCD2004(hi2c1, LCD2004_I2C_ADDR);

    mDisplay->prePaintMainScreen(mState);
    for (;;) {
        // TODO async
        mDisplay->paintMainScreen(mState);
        mState.speed += 0.5f;
        HAL_Delay(300);
        if (mState.speed > 40) {
            mState.speed = 0.f;
        }
    }
}
