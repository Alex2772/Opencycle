#pragma once

#include "Display/IDisplayDriver.h"

class App {
private:
    IDisplayDriver* mDisplay;

    App() = default;

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();
};