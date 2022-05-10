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
    float mCurrentSpeed = 0.f;
    std::uint16_t mMotorPower = 0;
    float mTmp = 0.f;
    kt::Config mConfig;

    float wheelDiameterInch() const;
};


