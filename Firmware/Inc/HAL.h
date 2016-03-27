#include "stm32f1xx_hal.h"
#include "Buffer.h"
#include "CLI.h"
#include "LCD_trans.h"
#include "cyclicBuffer.h"

class WellerHAL {
public:
    WellerHAL() {
        HAL_Init();
        SystemClock_Config();

        GPIO_Init();
        ADC1_Init();
        I2C1_Init();
        TIM1_Init();
        TIM3_Init();
        TIM4_Init();
        USART1_Init();
    }

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
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
    }

    uint16_t getThermocoupleReading() {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1000);
        return HAL_ADC_GetValue(&hadc1);
    }

    static ADC_HandleTypeDef hadc1;
    static I2C_HandleTypeDef hi2c1;
    static TIM_HandleTypeDef htim1;
    static TIM_HandleTypeDef htim3;
    static TIM_HandleTypeDef htim4;
    static UART_HandleTypeDef huart1;

private:
    void SystemClock_Config(void) {
        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_PeriphCLKInitTypeDef PeriphClkInit;

        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.HSICalibrationValue = 16;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);

        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

        /* SysTick_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    }

    void ADC1_Init() {
        ADC_ChannelConfTypeDef sConfig;
        hadc1.Instance = ADC1;
        hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
        hadc1.Init.ContinuousConvMode = DISABLE;
        hadc1.Init.DiscontinuousConvMode = DISABLE;
        hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        hadc1.Init.NbrOfConversion = 1;
        HAL_ADC_Init(&hadc1);

        sConfig.Channel = ADC_CHANNEL_0;
        sConfig.Rank = 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    }

    void I2C1_Init() {
        hi2c1.Instance = I2C1;
        hi2c1.Init.ClockSpeed = 100000;
        hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
        hi2c1.Init.OwnAddress1 = 0;
        hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
        hi2c1.Init.OwnAddress2 = 0;
        hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
        hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
        HAL_I2C_Init(&hi2c1);
    }

    void TIM1_Init() {
        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim1.Instance = TIM1;
        htim1.Init.Prescaler = 0;
        htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim1.Init.Period = 2000;
        htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = 0;
        HAL_TIM_Base_Init(&htim1);

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

        HAL_TIM_PWM_Init(&htim1);

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

        sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
        sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
        sBreakDeadTimeConfig.DeadTime = 0;
        sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
        sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
        sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
        HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
        sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    }

    void TIM3_Init() {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim3.Instance = TIM3;
        htim3.Init.Prescaler = 1;
        htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim3.Init.Period = 1000;
        htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        HAL_TIM_Base_Init(&htim3);

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

        HAL_TIM_PWM_Init(&htim3);

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 50;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);

        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

        __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    }

    void TIM4_Init() {

        TIM_ClockConfigTypeDef sClockSourceConfig;
        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        htim4.Instance = TIM4;
        htim4.Init.Prescaler = 1;
        htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim4.Init.Period = 10000;
        htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        HAL_TIM_Base_Init(&htim4);

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

        HAL_TIM_PWM_Init(&htim4);

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 50;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    }

    void USART1_Init() {
        huart1.Instance = USART1;
        huart1.Init.BaudRate = 115200;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        HAL_UART_Init(&huart1);

        __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);

        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    }

    void GPIO_Init(void) {

        GPIO_InitTypeDef GPIO_InitStruct;

                __GPIOA_CLK_ENABLE();
                __GPIOB_CLK_ENABLE();
                __GPIOC_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_ENC_BTN_Pin | GPIO_ENC_N_Pin | GPIO_ENC_P_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_Detector_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIO_Detector_GPIO_Port, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_LCD_D7_Pin | GPIO_LCD_D6_Pin | GPIO_LCD_D5_Pin | GPIO_LCD_D4_Pin
                              | GPIO_LCD_D3_Pin | GPIO_LCD_D2_Pin | GPIO_LCD_D1_Pin | GPIO_LCD_D0_Pin
                              | GPIO_LCD_EN_Pin | GPIO_LCD_RS_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        DBGMCU->CR &= (~(DBGMCU_CR_TRACE_IOEN)); //disable SWO trace to release PB3 pin
    }

    static Buffer<char, 100> uart_rx_buf;
    static CyclicBuffer_data<uint8_t, 100> uart_tx_buf;

public:
    static void __UART1_IRQ_RXNE(char ch) {
        uart_rx_buf.push(ch);
        if (ch == '\n') {
            extern CLI *cli;
            cli->parse_line(uart_rx_buf.get());
            uart_rx_buf.clear();
        }
    }

    static void __UART1_IRQ_TXE() {
        if (uart_tx_buf.isEmpty()) {
            __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
        } else {
            huart1.Instance->DR = uart_tx_buf.get();
        }
    }

    static void __TIM3_IRQ_UPDATE() {
        HD44780::TimeTick();
    }
};

ADC_HandleTypeDef WellerHAL::hadc1;
I2C_HandleTypeDef WellerHAL::hi2c1;
TIM_HandleTypeDef WellerHAL::htim1;
TIM_HandleTypeDef WellerHAL::htim3;
TIM_HandleTypeDef WellerHAL::htim4;
UART_HandleTypeDef WellerHAL::huart1;
Buffer<char, 100> WellerHAL::uart_rx_buf;
CyclicBuffer_data<uint8_t, 100> WellerHAL::uart_tx_buf;

extern "C" {
void __io_putchar(char ch) {
	extern WellerHAL *hal;
    hal->putch((uint8_t)ch);
}
void USART1_IRQHandler(void) {
    uint32_t tmp_flag, tmp_it_source;

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_PE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_PE);
    /* UART parity error interrupt occurred ------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_PEFLAG(&WellerHAL::huart1);
        WellerHAL::huart1.ErrorCode |= HAL_UART_ERROR_PE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_FE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_ERR);
    /* UART frame error interrupt occurred -------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_FEFLAG(&WellerHAL::huart1);
        WellerHAL::huart1.ErrorCode |= HAL_UART_ERROR_FE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_NE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_ERR);
    /* UART noise error interrupt occurred -------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_NEFLAG(&WellerHAL::huart1);
        WellerHAL::huart1.ErrorCode |= HAL_UART_ERROR_NE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_ORE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_ERR);
    /* UART Over-Run interrupt occurred ----------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_OREFLAG(&WellerHAL::huart1);
        WellerHAL::huart1.ErrorCode |= HAL_UART_ERROR_ORE;
    }

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_RXNE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_RXNE);
    /* UART in mode Receiver ---------------------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_FLAG(&WellerHAL::huart1, UART_FLAG_RXNE);
        WellerHAL::__UART1_IRQ_RXNE(USART1->DR);
    }

    tmp_flag = __HAL_UART_GET_FLAG(&WellerHAL::huart1, UART_FLAG_TXE);
    tmp_it_source = __HAL_UART_GET_IT_SOURCE(&WellerHAL::huart1, UART_IT_TXE);
    /* UART in mode Transmitter ------------------------------------------------*/
    if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
        __HAL_UART_CLEAR_FLAG(&WellerHAL::huart1, UART_FLAG_TXE);
        WellerHAL::__UART1_IRQ_TXE();
    }
}
void TIM3_IRQHandler(void) {
    __HAL_TIM_CLEAR_FLAG(&WellerHAL::htim3, TIM_FLAG_UPDATE);
    WellerHAL::__TIM3_IRQ_UPDATE();
}
}
