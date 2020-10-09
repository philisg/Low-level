#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "Sound.c"




/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define   SAMPLE_PERIOD   318 // =44khz=318

/*
 * Declaration of peripheral setup functions 
 */
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void setupGPIO();
void DAC_PLAY(int data);

 // Your code will start executing here 
 
int main(void)
{
	/*
	 * Call the peripheral setup functions 
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);
	

	/*
	 * Enable interrupt handling 
	 */
	setupNVIC();

	/*
	 * TODO for higher energy efficiency, sleep while waiting for
	 * interrupts instead of infinite loop for busy-waiting 
	 */
	uint16_t i = 0;

	
	while (1)
	{
	
		if(*TIMER1_CNT == SAMPLE_PERIOD)
		{
		*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
			if(*GPIO_PC_DIN == 0b11111110)
			{
				DAC_PLAY(G5[i]<<4);
				if(i >= size_G5){
				i = 0;
				}
				else{i++;}
			}
			else if(*GPIO_PC_DIN == 0b11111101)
			{
				DAC_PLAY(D5[i]<<4);
				if(i >= size_D5){
				i = 0;
				}
				else{i++;}
			}
			else if(*GPIO_PC_DIN == 0b11111011)
			{	
				DAC_PLAY(E5[i]<<4);
				if(i >= size_E5){
				i = 0;
				}
				else{i++;}
			}
			else if(*GPIO_PC_DIN == 0b11110111)
			{	
				DAC_PLAY(F5[i]<<4);
				if(i >= size_F5){
				i = 0;
				}
				else{i++;}
			}
		}
		
	}

	return 0;
}


void setupNVIC()
{
	/*
	 * TODO use the NVIC ISERx registers to enable handling of
	 * interrupt(s) remember two things are necessary for interrupt
	 * handling: - the peripheral must generate an interrupt signal - the
	 * NVIC must be configured to make the CPU handle the signal You will
	 * need TIMER1, GPIO odd and GPIO even interrupt handling for this
	 * assignment. 
	 */
	 
}



void DAC_PLAY(int data)// sends data to DAC
{	
				*DAC0_CH0DATA = data/2;
				*DAC0_CH1DATA = data/2;
}


/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler 
 */
 
