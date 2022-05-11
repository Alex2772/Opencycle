#pragma once

#include <cmath>
#include <cstdint>
#include <stm32f4xx_hal.h>
#include <array>
#include "State.h"
#include "Input.h"
#include "IDisplayDriver.h"
#include "Config.h"

class App {
public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

    bool isLightEnabled() {
        return mLightEnabled;
    }

    bool isKeyDown(Key key) {
        return mKeyStates[static_cast<int>(key)] == KeyState::PRESSED;
    }

    [[nodiscard]]
    I2C_HandleTypeDef* getI2C() {
        return &mI2C;
    }

    /**
     * high_resolution_clock in ms.
     * @return
     */
    static std::uint32_t tick();

    void onInput(Key key, KeyState state);

    void reboot();

    void onTimer100ms();

    void waitForInterrupt() const;

    void setFrontLightEnabled(bool enabled) const {
        setFrontLightPwm(enabled ? LIGHT_FULL_PWM : 0);
    }
    void setBackLightEnabled(bool enabled) const {
        setBackLightPwm(enabled ? LIGHT_FULL_PWM : 0);
    }

private:
    IDisplayDriver* mDisplay;
    I2C_HandleTypeDef mI2C;
    State mState;
    bool mLightEnabled = false;
    bool mStopBlinkState = false;
    std::uint32_t mStopLightBlinkingUntilTime = 0;
    bool mNeedStateUpdate = true;
    std::uint8_t mStateUpdateCounterForRepaint = 0;

    App();

    std::array<KeyState, 4> mKeyStates;

    void setFrontLightPwm(std::uint32_t pwm) const;
    void setBackLightPwm(std::uint32_t pwm) const;
};