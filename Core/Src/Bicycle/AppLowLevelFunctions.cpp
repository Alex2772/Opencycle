/**
 * The best place for all of your low level interrupt handling shit
 */


#include "App.h"
#include "stm32f4xx_hal.h"

extern "C" void app_run() {
    App::inst().run();
}


extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3) {
        App::inst().onWheelRevolution();
    }
}
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    App::inst().onMillisecondTimerElapsed();
}