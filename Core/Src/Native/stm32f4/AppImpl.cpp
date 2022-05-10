/**
 * The best place for all of your low level interrupt handling shit
 */


#include <Bicycle/App.h>
#include "stm32f4xx_hal.h"

extern "C" TIM_HandleTypeDef htim2;

extern "C" void app_run() {
    App::inst().run();
}


extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    auto state = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) ? KeyState::PRESSED : KeyState::RELEASED;
    switch (GPIO_Pin) {
        case GPIO_PIN_3:
            App::inst().onSensorReportedWheelRevolution();
            break;

        case GPIO_PIN_10:
            App::inst().onInput(Key::UP, state);
            break;

    }
}
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    App::inst().onWheelStopped();
}

uint16_t App::getMsCounterValueAndReset() {
    HAL_TIM_Base_Stop(&htim2);
    uint16_t v = htim2.Instance->CNT;
    htim2.Instance->CNT = 0;
    HAL_TIM_Base_Start_IT(&htim2);

    return v;
}

