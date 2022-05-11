//
// Created by Alex2772 on 5/9/2022.
//

#include "Bicycle/Uart.h"
#include <stm32f4xx_hal.h>
#include <cassert>

extern "C" UART_HandleTypeDef huart1;

static std::function<void()> gOnComplete;


/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (gOnComplete) {
        gOnComplete();
    }
}


void uart::asyncReceiveRepeat(std::uint8_t* dst, std::size_t bufferSize) {
    auto r = HAL_UART_Receive_DMA(&huart1, dst, bufferSize);
    assert(r == HAL_OK);
}

void uart::asyncReceive(std::uint8_t* dst, std::size_t bufferSize, std::function<void()> onComplete) {
    gOnComplete = std::move(onComplete);
    auto r = HAL_UART_Receive_DMA(&huart1, dst, bufferSize);
    assert(r == HAL_OK);
}

void uart::asyncTransmit(const std::uint8_t* src, std::size_t bufferSize) {
    HAL_UART_Transmit_DMA(&huart1, const_cast<std::uint8_t*>(src), bufferSize);
}