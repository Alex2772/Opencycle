/**
 * The best place for all of your low level interrupt handling shit
 */


#include "App.h"
#include "stm32f4xx_hal.h"

extern "C" TIM_HandleTypeDef htim1;

extern "C" void app_run() {
    App::inst().run();
}


extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3) {
        App::inst().onSensorReportedWheelRevolution();
    }
}
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    App::inst().onWheelStopped();
}

uint16_t App::getMsCounterValueAndReset() {
    HAL_TIM_Base_Stop(&htim1);
    uint16_t v = htim1.Instance->CNT;
    htim1.Instance->CNT = 0;
    HAL_TIM_Base_Start_IT(&htim1);
    return v;
}
