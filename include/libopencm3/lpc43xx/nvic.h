/* --- IRQ channel numbers-------------------------------------------------- */

/* Cortex M4 System Interrupts */
#define NVIC_NMI_IRQ                    -14
#define NVIC_HARD_FAULT_IRQ             -13
#define NVIC_MEM_MANAGE_IRQ             -12
#define NVIC_BUS_FAULT_IRQ              -11
#define NVIC_USAGE_FAULT_IRQ            -10
/* irq numbers -6 to -9 are reserved */
#define NVIC_SV_CALL_IRQ                -5
#define DEBUG_MONITOR_IRQ               -4
/* irq number -3 reserved */
#define NVIC_PENDSV_IRQ                 -2
#define NVIC_SYSTICK_IRQ                -1

/* LPC43xx M4 specific user interrupts */
#define NVIC_M4_DAC_IRQ                 0
#define NVIC_M4_M0CORE_IRQ              1
#define NVIC_M4_DMA_IRQ                 2
#define NVIC_M4_ETHERNET_IRQ            5
#define NVIC_M4_SDIO_IRQ                6
#define NVIC_M4_LCD_IRQ                 7
#define NVIC_M4_USB0_IRQ                8
#define NVIC_M4_USB1_IRQ                9
#define NVIC_M4_SCT_IRQ                 10
#define NVIC_M4_RITIMER_IRQ             11
#define NVIC_M4_TIMER0_IRQ              12
#define NVIC_M4_TIMER1_IRQ              13
#define NVIC_M4_TIMER2_IRQ              14
#define NVIC_M4_TIMER3_IRQ              15
#define NVIC_M4_MCPWM_IRQ               16
#define NVIC_M4_ADC0_IRQ                17
#define NVIC_M4_I2C0_IRQ                18
#define NVIC_M4_I2C1_IRQ                19
#define NVIC_M4_SPI_IRQ                 20
#define NVIC_M4_ADC1_IRQ                21
#define NVIC_M4_SSP0_IRQ                22
#define NVIC_M4_SSP1_IRQ                23
#define NVIC_M4_USART0_IRQ              24
#define NVIC_M4_UART1_IRQ               25
#define NVIC_M4_USART2_IRQ              26
#define NVIC_M4_USART3_IRQ              27
#define NVIC_M4_I2S0_IRQ                28
#define NVIC_M4_I2S1_IRQ                29
#define NVIC_M4_SPIFI_IRQ               30
#define NVIC_M4_SGPIO_IRQ               31
#define NVIC_M4_PIN_INT0_IRQ            32
#define NVIC_M4_PIN_INT1_IRQ            33
#define NVIC_M4_PIN_INT2_IRQ            34
#define NVIC_M4_PIN_INT3_IRQ            35
#define NVIC_M4_PIN_INT4_IRQ            36
#define NVIC_M4_PIN_INT5_IRQ            37
#define NVIC_M4_PIN_INT6_IRQ            38
#define NVIC_M4_PIN_INT7_IRQ            39
#define NVIC_M4_GINT0_IRQ               40
#define NVIC_M4_GINT1_IRQ               41
#define NVIC_M4_EVENTROUTER_IRQ         42
#define NVIC_M4_C_CAN1_IRQ              43
#define NVIC_M4_ATIMER_IRQ              46
#define NVIC_M4_RTC_IRQ                 47
#define NVIC_M4_WWDT_IRQ                49
#define NVIC_M4_C_CAN0_IRQ              51
#define NVIC_M4_QEI_IRQ                 52

/* LPC43xx M0 specific user interrupts */
//TODO
