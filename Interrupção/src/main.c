
     #include "main.h"
	 #include <string.h>
	/* Struct de configuraзгo da comunicaco serial definido em  stm32f0xx_hal_uart.h*/
	UART_HandleTypeDef UartHandle;

	/* Buffer de Transmissa  */
	char Buffer[BUFFERSIZE] = "\n\r ";


	static void SystemClock_Config(void);
	static void Error_Handler(void);

	/* Struct definida em  stm32f0xx_hal_gpio.h para configrar o port*/
	static GPIO_InitTypeDef  GPIO_InitStruct;
	 int Debouce_func(GPIO_TypeDef* GPIO_botao,uint16_t GPIO_Pino_bota);
	static void SystemClock_Config(void);
	static void Error_Handler(void);
	int contador=0;
	int valorVelhoContador=-1;

     int main(void) {

       /* Inicializa as bibliotecas HAL */
       HAL_Init();

       /* Configura o clock do sistema */
       SystemClock_Config();

       /* Habilita o Clock no port do led,  funчуo definida em stm32f0xx_hal_rcc.h*/
        __GPIOA_CLK_ENABLE();

        /* Configura  o pino do botгo como um pino de interrupзгo externa*/
                          GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING;
                          GPIO_InitStruct.Pull  = GPIO_NOPULL;
                          GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
                          GPIO_InitStruct.Pin   = GPIO_PIN_7 ||GPIO_PIN_6 ;

                          HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


         /* Habilita e conigura a priodidade da interrupчуo  EXTI line 4_15  */
         HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
         HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
         /* Habilita o Clock nos ports A,  função definida em stm32f0xx_hal_rcc.h*/

            /*Configura os pinos GPIOs  para a função alternativa de RX e TX */
              GPIO_InitStruct.Pin       = GPIO_PIN_2|GPIO_PIN_3;  // Pino de TX verificar no  esquemático
              GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
              GPIO_InitStruct.Pull      = GPIO_NOPULL;
              GPIO_InitStruct.Speed     = GPIO_SPEED_LOW;
              GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
              HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

              /* habilita o clock da  USART2 */
             __USART2_CLK_ENABLE();
              /*Configuração do Periférico USART
               	  - modo assincrono (UART Mode)
                  - Word  = 8 Bits
                  - Stop Bit = One Stop bit
                  - Parity = None
                  - BaudRate = 9600 baud
                  - Hardware flow control disabled (RTS and CTS signals) */

              UartHandle.Instance        = USART2;
              UartHandle.Init.BaudRate   = 9600;
              UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
              UartHandle.Init.StopBits   = UART_STOPBITS_1;
              UartHandle.Init.Parity     = UART_PARITY_NONE;
              UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
              UartHandle.Init.Mode       = UART_MODE_TX_RX;


              if(HAL_UART_Init(&UartHandle) != HAL_OK)
              {
                Error_Handler();
              }
              itoa(contador,Buffer,10);
                   	    		strcat(Buffer, " \n\r");
              HAL_UART_Transmit(&UartHandle,(uint8_t*)Buffer , BUFFERSIZE, 5000);
        /* Infinite loop */
         while (1)
        {


        }

         return 0;
     }



     void  funcaoTratamentoInterrupcao(uint16_t GPIO_Pin){
     	/* Se ocorreu a interrupчуo externa */
     	if(__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {

     		//Limpa a flag de interrupчуo
     		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

     	    if (GPIO_Pin == GPIO_PIN_7) {

     	    	if(Debouce_func(GPIOA,GPIO_Pin) && contador!=255){
     	    		contador++;
     	    		itoa(contador,Buffer,10);
     	    		strcat(Buffer, " \n\r");
     	    		HAL_UART_Transmit(&UartHandle,(uint8_t*)Buffer , BUFFERSIZE, 5000);
     	    	}
     	    }else{
     	    	if(Debouce_func(GPIOA,GPIO_Pin) && contador!=0){
     	    	     	    		contador--;
     	    	     	    		itoa(contador,Buffer,10);
     	    	     	    		strcat(Buffer, " \n\r");
     	    	     	    		HAL_UART_Transmit(&UartHandle,(uint8_t*)Buffer , BUFFERSIZE, 5000);
     	    	     	    	}
     	    }
     	}


     }

     int Debouce_func(GPIO_TypeDef* GPIO_botao,uint16_t GPIO_Pino_bota){
     			/* funзгo definida em stm32f0xx_hal_gpio.h */
     		   int aux=0,i;
                for(i=0;i<10;i++){
                   if(HAL_GPIO_ReadPin (GPIO_botao, GPIO_Pino_bota) == 1) aux++;
                   if(aux==3)return 1;


                }
     	      return 0;

     }



     /* Funчуo de configuraчуo do clock - Atщ o momento iremos utiliza-la apenas, no futuro estudaremos as configuracoes */
     static void SystemClock_Config(void)
     {
       RCC_ClkInitTypeDef RCC_ClkInitStruct;
       RCC_OscInitTypeDef RCC_OscInitStruct;

       /* No HSE Oscillator on Nucleo, Activate PLL with HSI/2 as source */
       RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
       RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
       RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
       RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
       RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
       if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
       {
         Error_Handler();
       }

       /* Select PLL as system clock source and configure the HCLK, PCLK1 clocks dividers */
       RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
       RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
       RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
       RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
       if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
       {
         Error_Handler();
       }
     }


     /* Funчуo chamada no caso de erro na configuraчуo */
     static void Error_Handler(void)
     {
        while(1);
     }
