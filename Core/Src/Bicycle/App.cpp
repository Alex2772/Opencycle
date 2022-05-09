#include "App.h"
#include "Config.h"
#include "DeviceManager.h"
#include "Calendar.h"
#include "Driver/LCD2004/LCD2004.h"


extern I2C_HandleTypeDef hi2c1;

App::App():
    mI2C(hi2c1)
{
    DeviceManager::inst();
}

void App::run() {
    // TODO embed to some config
    mDisplay = new LCD2004(hi2c1, LCD2004_I2C_ADDR);

    mDisplay->prePaintMainScreen(mState);

    DeviceManager::init();

    // main loop
    for (;;) {
        DeviceManager::updateState(mState);
        mDisplay->paintMainScreen(mState);

        // wait for interrupt
        __WFI();
    }
}

void App::reboot() {
    HAL_NVIC_SystemReset();
}


void App::onInput(Key key, KeyState state) {
    mKeyStates[static_cast<int>(key)] = state;
}
