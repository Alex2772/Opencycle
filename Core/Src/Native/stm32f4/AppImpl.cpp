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
    static std::uint32_t lastEvent = 0;

    if (lastEvent > HAL_GetTick()) {
        return;
    }
    lastEvent = HAL_GetTick() + 200;

    auto state = HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) ? KeyState::PRESSED : KeyState::RELEASED;
    switch (GPIO_Pin) {
        case GPIO_PIN_10:
            App::inst().onInput(Key::UP, state);
            break;
        case GPIO_PIN_2:
            App::inst().onInput(Key::DOWN, state);
            break;
        case GPIO_PIN_12:
            App::inst().onInput(Key::LEFT, state);
            break;
        case GPIO_PIN_13:
            App::inst().onInput(Key::RIGHT, state);
            break;

    }
}

std::uint32_t App::tick() {
    return HAL_GetTick();
}

/**
  * @brief  Hall Trigger detection callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (&htim2 == htim) {
        App::inst().scheduleStateUpdate();
    }
}