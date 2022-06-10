#pragma once

#include <Bicycle/State.h>
#include "Bicycle/Menu/ListMenu.h"

class IDisplayDriver {
public:
    virtual void prePaintMainScreen(const State& state) = 0;
    virtual void paintMainScreen(const State& state) = 0;
    virtual void paintListMenu(const ListMenu::State& state) = 0;
    virtual void paintValueChangingMenu(std::string_view str) = 0;

    virtual ~IDisplayDriver() = default;
};

