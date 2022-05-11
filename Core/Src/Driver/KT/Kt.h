#pragma once


#include "Bicycle/IDevice.h"
#include "KtConfig.hpp"

class Kt: public IDevice {
public:
    bool init() override;

    void updateState(State& state) override;

    ~Kt() override = default;

private:
    char mReceiveBuffer[32] __attribute__ ((aligned(32)));
    kt::LcdToKtPayload mCurrentLcdToKtPayload  __attribute__ ((aligned(32)));
    double mCurrentSpeed = 0.f;
    std::uint16_t mMotorPower = 0;
    std::uint8_t mMotorTemperature = 0;
    double mTmp = 0.f;
    kt::Config mConfig;
    bool mIsThrottling = false;

    std::uint32_t mLastPacketReceivedTime = 0;

    double wheelDiameterInch() const;

    void transmit() const;
};


