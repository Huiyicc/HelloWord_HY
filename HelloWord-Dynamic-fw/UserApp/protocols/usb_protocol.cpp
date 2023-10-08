#include "usart.h"

uint8_t Buffer[1];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance == USART2)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)Buffer, 1, 0xffff);
        HAL_UART_Receive_IT(&huart2, (uint8_t *)Buffer, 1);
    }
}