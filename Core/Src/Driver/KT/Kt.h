#pragma once


#include "Bicycle/IDevice.h"
#include "KtConfig.hpp"

class Kt: public IDevice {
public:
    bool init() override;

    void updateState(State& state) override;

    ~Kt() override = default;

private:
    kt::KtToLcdPayload mCurrentKtToLcdPayload;
    kt::LcdToKtPayload mCurrentLcdToKtPayload;
    bool mNeedDispatchPayload = false;
    double mCurrentSpeed = 0.f;
    std::uint16_t mMotorPower = 0;
    std::uint8_t mMotorTemperature = 0;
    double mTmp = 0.f;
    kt::Config mConfig;

    double wheelDiameterInch() const;
};


