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
    }
}
