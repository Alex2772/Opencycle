//
// Created by Alex2772 on 5/9/2022.
//

#include "Kt.h"
#include "Bicycle/Uart.h"


bool Kt::init() {
    return true;
}

void Kt::updateState(State& state) {
    uart::asyncTransmit(mCurrentLcdToKtPayload);

    if (mNeedDispatchPayload) {
        mNeedDispatchPayload = false;
        if (auto dispatched = kt::dispatch(mCurrentKtToLcdPayload)) {
            mCurrentSpeed = dispatched->speed;
        }
    }

    uart::asyncReceive(mCurrentKtToLcdPayload, [this] {
        mNeedDispatchPayload = true;
    });

    state.currentSpeed = mCurrentSpeed;
}
