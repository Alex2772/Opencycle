#pragma once


#include "Bicycle/IDevice.h"
#include "KtConfig.hpp"

class Kt: public IDevice {
public:
    bool init() override;

    void updateState(State& state) override;

    ~Kt() override = default;

private:
    char mReceiveBuffer[sizeof(kt::KtToLcdPayload) * 2];
    kt::LcdToKtPayload mCurrentLcdToKtPayload;
    double mCurrentSpeed = 0.f;
    std::uint16_t mMotorPower = 0;
    std::uint8_t mMotorTemperature = 0;
    double mTmp = 0.f;
    kt::Config mConfig;

    std::uint32_t mLastPacketReceivedTime = 0;

    double wheelDiameterInch() const;

    void transmit() const;
};


