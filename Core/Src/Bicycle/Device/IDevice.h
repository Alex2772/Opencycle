//
// Created by Alex2772 on 9/16/2021.
//

#pragma once

#include <Bicycle/State.h>

class IDevice {
public:
    /**
     * Initializes the device.
     * @return true, when device initialized properly
     */
    virtual bool init() = 0;

    /**
     * Updates state.
     */
    virtual void updateState(State& state) = 0;

    virtual ~IDevice() = default;
};