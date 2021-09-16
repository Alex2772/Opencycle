#pragma once

#include "Display/IDisplayDriver.h"

class App {
private:
    IDisplayDriver* mDisplay;
    State mState;


    App() = default;

public:

    static App& inst() {
        static App app;
        return app;
    }

    void run();

    inline void onWheelRevolution() {
        
    }
};