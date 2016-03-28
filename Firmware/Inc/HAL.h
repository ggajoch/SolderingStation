#include "stm32f1xx_hal.h"
#include "Buffer.h"
#include "CLI.h"
#include "LCD_trans.h"
#include "cyclicBuffer.h"
#include "Encoder.h"
#include "HAL_LL_Init.h"

namespace HAL {
    void Init() {
        HAL_Init();
        SystemClock_Config();

        GPIO_Init();
        ADC1_Init();
        I2C1_Init();
        TIM1_Init();
        TIM2_Init();
        TIM3_Init();
        TIM4_Init();
        USART1_Init();
    }

    Buffer<char, 100> uart_rx_buf;
    CyclicBuffer_data<uint8_t, 100> uart_tx_buf;


    void setBacklight(float percent) {
        TIM3->CCR4 = static_cast<uint16_t>(10.0 * percent);
    }

    void setContrast(float percent) {
        TIM4->CCR4 = static_cast<uint16_t>(100.0 * percent);
    }

    void setHeating(float percent) {
        TIM1->CCR1 = static_cast<uint16_t>(20.0 * percent);
    }

    void putch(uint8_t ch) {
        uart_tx_buf.append(ch);
        USART1->CR1 |= USART_CR1_TXEIE;
//        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE); //causing HardFault, for some reason
    }

    void encPlus() {
        printf("PLUS\r\n");
    }

    void encMinus() {
        printf("MINUS\r\n");
    }

    uint16_t getThermocoupleReading() {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1000);
        return HAL_ADC_GetValue(&hadc1);
    }


    Encoder::State getEncoderState() {
        Encoder::State now;
        now.p = HAL_GPIO_ReadPin(GPIO_ENC_P_GPIO_Port, GPIO_ENC_P_Pin);
        now.n = HAL_GPIO_ReadPin(GPIO_ENC_N_GPIO_Port, GPIO_ENC_N_Pin);
        return now;
    }


    Encoder::Encoder encoder(getEncoderState, encPlus, encMinus);





    void __UART1_IRQ_RXNE(char ch) {
        uart_rx_buf.push(ch);
        if (ch == '\n') {
            CLI::parse_line(uart_rx_buf.get());
            uart_rx_buf.clear();
        }
    }

    void __UART1_IRQ_TXE() {
        if (uart_tx_buf.isEmpty()) {
            __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
        } else {
            huart1.Instance->DR = uart_tx_buf.get();
        }
    }

    void __TIM3_IRQ_UPDATE() {
        HD44780::TimeTick();
        encoder.Tick();
    }

    volatile bool TIM_TICK;

    void __TIM2_IRQ_UPDATE() {
        TIM_TICK = true;
    }


    extern "C" {
    void __io_putchar(char ch) {
        HAL::putch((uint8_t) ch);
    }
    void USART1_IRQHandler(void) {
        uint32_t tmp_flag, tmp_it_source;

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_PE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_PE);
        /* UART parity error interrupt occurred ------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_PEFLAG(&HAL::huart1);
            HAL::huart1.ErrorCode |= HAL_UART_ERROR_PE;
        }

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_FE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_ERR);
        /* UART frame error interrupt occurred -------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_FEFLAG(&HAL::huart1);
            HAL::huart1.ErrorCode |= HAL_UART_ERROR_FE;
        }

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_NE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_ERR);
        /* UART noise error interrupt occurred -------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_NEFLAG(&HAL::huart1);
            HAL::huart1.ErrorCode |= HAL_UART_ERROR_NE;
        }

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_ORE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_ERR);
        /* UART Over-Run interrupt occurred ----------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_OREFLAG(&HAL::huart1);
            HAL::huart1.ErrorCode |= HAL_UART_ERROR_ORE;
        }

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_RXNE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_RXNE);
        /* UART in mode Receiver ---------------------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_FLAG(&HAL::huart1, UART_FLAG_RXNE);
            HAL::__UART1_IRQ_RXNE(USART1->DR);
        }

        tmp_flag = __HAL_UART_GET_FLAG(&HAL::huart1, UART_FLAG_TXE);
        tmp_it_source = __HAL_UART_GET_IT_SOURCE(&HAL::huart1, UART_IT_TXE);
        /* UART in mode Transmitter ------------------------------------------------*/
        if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
            __HAL_UART_CLEAR_FLAG(&HAL::huart1, UART_FLAG_TXE);
            HAL::__UART1_IRQ_TXE();
        }
    }
    void TIM2_IRQHandler(void) {
        __HAL_TIM_CLEAR_FLAG(&HAL::htim2, TIM_FLAG_UPDATE);
        HAL::__TIM2_IRQ_UPDATE();
    }
    void TIM3_IRQHandler(void) {
        __HAL_TIM_CLEAR_FLAG(&HAL::htim3, TIM_FLAG_UPDATE);
        HAL::__TIM3_IRQ_UPDATE();
    }


    void NMI_Handler(void) { while (1) { }}
    void HardFault_Handler(void) { while (1) { }}
    void MemManage_Handler(void) { while (1) { }}
    void BusFault_Handler(void) { while (1) { }}
    void UsageFault_Handler(void) { while (1) { }}

    void SVC_Handler(void) { while (1) { }}
    void DebugMon_Handler(void) { while (1) { }}
    void PendSV_Handler(void) { while (1) { }}
    void WWDG_IRQHandler(void) { while (1) { }}
    void PVD_IRQHandler(void) { while (1) { }}
    void TAMPER_IRQHandler(void) { while (1) { }}
    void RTC_IRQHandler(void) { while (1) { }}
    void FLASH_IRQHandler(void) { while (1) { }}
    void RCC_IRQHandler(void) { while (1) { }}
    void EXTI0_IRQHandler(void) { while (1) { }}
    void EXTI1_IRQHandler(void) { while (1) { }}
    void EXTI2_IRQHandler(void) { while (1) { }}
    void EXTI3_IRQHandler(void) { while (1) { }}
    void EXTI4_IRQHandler(void) { while (1) { }}
    void DMA1_Channel1_IRQHandler(void) { while (1) { }}
    void DMA1_Channel2_IRQHandler(void) { while (1) { }}
    void DMA1_Channel3_IRQHandler(void) { while (1) { }}
    void DMA1_Channel4_IRQHandler(void) { while (1) { }}
    void DMA1_Channel5_IRQHandler(void) { while (1) { }}
    void DMA1_Channel6_IRQHandler(void) { while (1) { }}
    void DMA1_Channel7_IRQHandler(void) { while (1) { }}
    void ADC1_2_IRQHandler(void) { while (1) { }}
    void USB_HP_CAN1_TX_IRQHandler(void) { while (1) { }}
    void USB_LP_CAN1_RX0_IRQHandler(void) { while (1) { }}
    void CAN1_RX1_IRQHandler(void) { while (1) { }}
    void CAN1_SCE_IRQHandler(void) { while (1) { }}
    void EXTI9_5_IRQHandler(void) { while (1) { }}
    void TIM1_BRK_IRQHandler(void) { while (1) { }}
    void TIM1_UP_IRQHandler(void) { while (1) { }}
    void TIM1_TRG_COM_IRQHandler(void) { while (1) { }}
    void TIM1_CC_IRQHandler(void) { while (1) { }}
    void TIM4_IRQHandler(void) { while (1) { }}
    void I2C1_EV_IRQHandler(void) { while (1) { }}
    void I2C1_ER_IRQHandler(void) { while (1) { }}
    void I2C2_EV_IRQHandler(void) { while (1) { }}
    void I2C2_ER_IRQHandler(void) { while (1) { }}
    void SPI1_IRQHandler(void) { while (1) { }}
    void SPI2_IRQHandler(void) { while (1) { }}
    void USART2_IRQHandler(void) { while (1) { }}
    void USART3_IRQHandler(void) { while (1) { }}
    void EXTI15_10_IRQHandler(void) { while (1) { }}
    void RTC_Alarm_IRQHandler(void) { while (1) { }}
    void USBWakeUp_IRQHandler(void) { while (1) { }}
    }
}
