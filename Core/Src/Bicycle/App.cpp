#include "App.h"
#include "Config.h"
#include "DeviceManager.h"
#include "Calendar.h"
#include "Driver/LCD2004/LCD2004.h"


extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;

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
        if (mNeedRepaint) {
            mNeedRepaint = false;
            DeviceManager::updateState(mState);

            const double TIMER_PERIOD_SEC = 0.5;

            mState.distance += mState.currentSpeed / 3.6 * TIMER_PERIOD_SEC;
            mState.consumedPowerKWh += mState.motorPower / 1000.0 * TIMER_PERIOD_SEC / 60.0 / 60.0;

            mDisplay->paintMainScreen(mState);
        }
        // wait for interrupt
        __WFI();
    }
}

void App::reboot() {
    HAL_NVIC_SystemReset();
}


void App::onInput(Key key, KeyState state) {
    mKeyStates[static_cast<int>(key)] = state;

    if (state == KeyState::PRESSED) {
        switch (key) {
            case Key::UP:
                mLight = !mLight;
                __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, mLight ? 1000 : 0);
                __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, mLight ? 1000 : 0);
                break;
        }
    }
}
