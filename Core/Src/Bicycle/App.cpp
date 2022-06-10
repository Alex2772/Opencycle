#include "App.h"
#include "Config.h"
#include "DeviceManager.h"
#include "Calendar.h"
#include "Driver/LCD2004/LCD2004.h"
#include "Settings.h"
#include "Bicycle/Menu/SettingsMenu.h"


extern I2C_HandleTypeDef hi2c1;

App::App():
    mI2C(hi2c1)
{
    DeviceManager::inst();
}

void App::run() {

    {
        Settings s;
        s.backLightIntensity = 1;
        s.frontLightIntensity = 1;
        s.save();
    }
    // TODO embed to some config
    mDisplay = new LCD2004(hi2c1, LCD2004_I2C_ADDR);

    mDisplay->prePaintMainScreen(mState);

    DeviceManager::init();

    // main loop
    for (;;) {
        if (mNeedStateUpdate) {
            mNeedStateUpdate = false;
            DeviceManager::updateState(mState);


            const double TIMER_PERIOD_SEC = 0.1;

            mState.distance += mState.currentSpeed / 3.6 * TIMER_PERIOD_SEC;
            mState.consumedPowerWhh += mState.motorPower * TIMER_PERIOD_SEC / 60.0 / 60.0;

            mStateUpdateCounterForRepaint = (mStateUpdateCounterForRepaint + 1) % 2;
            if (mStateUpdateCounterForRepaint == 0) {
                mDisplay->paintMainScreen(mState);
            }

            auto speedDelta = mState.currentSpeed - mState.prevSpeed;
            if (speedDelta < -0.2) {
                mStopLightBlinkingUntilTime = tick() + 1000;
            }

            mState.prevSpeed = mState.currentSpeed;
        }
        waitForInterrupt();
    }
}


void App::reboot() {
    HAL_NVIC_SystemReset();
}


void App::onInput(Key key, KeyState state) {
    mKeyStates[static_cast<int>(key)] = state;

    if (!mMenuStack.empty()) {
        if (state == KeyState::PRESSED) {
            mMenuStack.top()->onKeyAction(key);
        }
    }

    if (state == KeyState::PRESSED) {
        switch (key) {
            case Key::UP:
                mLightEnabled = !mLightEnabled;
                setFrontLightEnabled(mLightEnabled);
                setBackLightEnabled(mLightEnabled);
                break;

            case Key::DOWN:
                reboot();
                break;

            case Key::LEFT:
                showMenu(std::make_unique<SettingsMenu>());
                break;
        }
    }
}

void App::onTimer100ms() {
    mNeedStateUpdate = true;

    if (mStopLightBlinkingUntilTime >= tick() && !mState.isThrottling) {
        mStopBlinkState = !mStopBlinkState;
        setBackLightEnabled(mStopBlinkState);
    } else {
        setBackLightEnabled(mLightEnabled);
    }
}
