//
// Created by Alex2772 on 5/9/2022.
//

#include "Kt.h"
#include "Bicycle/Uart.h"
#include "Bicycle/App.h"
#include <cmath>
#include <cstring>
#include <cassert>

double Kt::wheelDiameterInch() const {
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
    return 0;
}

bool Kt::init() {
    mCurrentLcdToKtPayload = kt::configToPlayload(mConfig);

    performReceive();

    return true;
}

void Kt::performReceive() {
    uart::asyncReceive(mReceiveBuffer, [this] {

        auto begin = std::begin(mReceiveBuffer);
        auto end = std::end(mReceiveBuffer);

        constexpr auto PACKET_BEGIN_MARKER = 0x41;

        auto packetBegin = std::find(begin, end, 0x41);

        if (begin >= end) {
            // not a kt packet; drop it
            return;
        } else if (packetBegin != begin) {
            // a shift occurred; need to compensate it
            std::size_t remainingPacketLength = packetBegin - begin;
            uart::asyncReceive(reinterpret_cast<uint8_t*>(mReceiveBuffer), remainingPacketLength, [this] {
                // compensated, try normal receipt again
                performReceive();
            });
            return;
        }

        if (auto dispatched = kt::dispatch(*reinterpret_cast<kt::KtToLcdPayload*>(begin))) {
            mMotorPower = dispatched->motorPower;
            mMotorTemperature = dispatched->motorTemperatureCelsius;
            mIsThrottling = dispatched->throttle;
            auto currentSpeed = wheelDiameterInch() * 0.0254 * M_PI * 3.6 * 1000.0 / dispatched->period;
            if (currentSpeed < 2.0) {
                mCurrentSpeed = 0;
            } else {
                mCurrentSpeed = currentSpeed;
            }
        }
        mLastPacketReceivedTime = App::tick();
    });
}

void Kt::updateState(State& state) {
    uart::asyncReceiveRepeat(mReceiveBuffer);
    transmit();

    if (App::tick() - mLastPacketReceivedTime >= 1000) {
        mMotorTemperature = 0;
        mMotorPower = 0;
        mCurrentSpeed = 0;
    }

    state.motorPower = mMotorPower;
    state.motorTemperature = mMotorTemperature;
    state.currentSpeed = mCurrentSpeed;
    state.isThrottling = mIsThrottling;
}

void Kt::transmit() const { uart::asyncTransmit(mCurrentLcdToKtPayload); }
