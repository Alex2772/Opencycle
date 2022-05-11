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
    bool mLight = false;
    bool mNeedRepaint = true;

    App();

    std::array<KeyState, 4> mKeyStates;

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

    bool isLight() {
        return mLight;
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

    void scheduleRepaint() {
        mNeedRepaint = true;
    }
};