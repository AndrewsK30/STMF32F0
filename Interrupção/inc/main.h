#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_nucleo.h"
#include "stm32f0xx_hal_uart.h"
#include "stm32f0xx_hal_gpio.h"

/* Size of buffer */
#define BUFFERSIZE                      50

void  funcaoTratamentoInterrupcao(uint16_t GPIO_Pin);

#endif /* __MAIN_H */
