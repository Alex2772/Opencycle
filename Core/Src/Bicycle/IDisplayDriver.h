#pragma once

#include <Bicycle/State.h>

class IDisplayDriver {
public:
    virtual void prePaintMainScreen(const State& state) = 0;
    virtual void paintMainScreen(const State& state) = 0;

    virtual ~IDisplayDriver() = default;
};

