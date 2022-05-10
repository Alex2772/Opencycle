//
// Created by Alex2772 on 5/9/2022.
//

#include "Kt.h"
#include "Bicycle/Uart.h"
#include <cmath>

float Kt::wheelDiameterInch() const {
    switch (mConfig.wheelSize) {
        case kt::Config::WheelSize::INCH_10: return 10;
        case kt::Config::WheelSize::INCH_12: return 12;
        case kt::Config::WheelSize::INCH_16: return 16;
        case kt::Config::WheelSize::INCH_20: return 20;
        case kt::Config::WheelSize::INCH_22: return 22;
        case kt::Config::WheelSize::INCH_24: return 24;
        case kt::Config::WheelSize::INCH_26: return 26;
        case kt::Config::WheelSize::INCH_27_5: return 27.5;
    }
}

bool Kt::init() {
    mConfig.assistLevel = kt::Config::AssistLevel::L5;
    mCurrentLcdToKtPayload = kt::configToPlayload(mConfig);
    return true;
}

void Kt::updateState(State& state) {
    uart::asyncTransmit(mCurrentLcdToKtPayload);

    if (mNeedDispatchPayload) {
        mNeedDispatchPayload = false;
        if (auto dispatched = kt::dispatch(mCurrentKtToLcdPayload)) {
            auto currentSpeed = wheelDiameterInch() * 0.0254 * M_PI * 3.6 * 1000.0 / dispatched->period;
            if (currentSpeed < 2.0) {
                mCurrentSpeed = 0;
            } else {
                mCurrentSpeed = currentSpeed;
            }
        }
    }

    uart::asyncReceive(mCurrentKtToLcdPayload, [this] {
        mNeedDispatchPayload = true;
    });

    state.currentSpeed = mCurrentSpeed;
}
