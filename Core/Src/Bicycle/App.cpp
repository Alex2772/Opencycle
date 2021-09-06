#include "App.h"
#include "Display/LCD2004.h"
#include "Config.h"

extern I2C_HandleTypeDef hi2c1;

extern "C" void app_run() {
    App::inst().run();
}

void App::run() {
    // TODO embed to some config
    mDisplay = new LCD2004(hi2c1, LCD2004_I2C_ADDR);
}
