#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
int tones[15] = {127,182,226,251,252,227,184,129,74,29,3,2,25,69};
int note[168] = { 127, 132, 136, 141, 146, 151, 155, 160, 164, 169, 173, 178, 182, 186, 191, 195, 199, 202, 206, 210, 213, 217, 220, 223, 226, 229, 232, 235, 237, 240, 242, 244, 246, 247, 249, 250, 251, 252, 253, 254, 254, 254, 254, 254, 254, 254, 253, 252, 251, 250, 249, 248, 246, 244, 242, 240, 238, 235, 233, 230, 227, 224, 221, 218, 214, 211, 207, 204, 200, 196, 192, 188, 183, 179, 175, 170, 166, 161, 157, 152, 147, 143, 138, 133, 128, 124, 119, 114, 109, 105, 100, 95, 91, 86, 82, 77, 73, 69, 65, 60, 56, 53, 49, 45, 41, 38, 35, 31, 28, 25, 23, 20, 17, 15, 13, 11, 9, 7, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 5, 6, 8, 9, 11, 13, 15, 18, 20, 23, 26, 29, 32, 35, 39, 42, 46, 49, 53, 57, 61, 65, 69, 74, 78, 83, 87, 92, 96, 101, 105, 110, 115, 120};



/*
 * TODO calculate the appropriate sample period for the sound wave(s) you 
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits. 
 */
/*
 * The period between sound samples, in clock cycles 
 */
#define   SAMPLE_PERIOD   270

/*
 * Declaration of peripheral setup functions 
 */
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void setupGPIO();
void DAC_play(int data);
void tone(period,amp);
void critialHealth();
void noteGenerator(int step);
void noteGenerator_2(int step);
void playNoteAndPause(int step,int wait, int pause );


/*
 * Your code will start executing here 
 */
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
	int steps_index [9] = {523.25, 587.33, 659.25, 698.46,784,880,987,1046.50};
	int increment=0;
	
	while (1)
	{
	
	//tone(270,2000);

	  /*
	  for(i = 500; i < 750; i++){
	    playNoteAndPause(i);
	  }
	  */

	  
	  if(*TIMER1_CNT == SAMPLE_PERIOD){
	    playNoteAndPause(steps_index[increment],85,100000);
	    increment += 1;
    	  }
	  if(increment > 7 ){
	    increment = 0;
	    critialHealth();
	  }
	 
	  
	 
	}

	return 0;
}
void noteGenerator(int step){
  //Outdated version can be ignored
	  if(*TIMER1_CNT == SAMPLE_PERIOD)
	  {
	    for(int i = 0; i<1000;i++){
	        DAC_play(i/100);		
	    }

	    //  DAC_PLAY(0);
	    for(int i = 1000; i>0;i--){
	        DAC_play(i/100);		
	    }
	  }	
}

void noteGenerator_2(int step){
  // if(*TIMER1_CNT == SAMPLE_PERIOD){
  // Takes the step for a desired pitch experience. Changing the step will change the resulting pitch
  // The increments keeps the notes playing, building up an amplitude then building down at half the binary loudness. (i/100) and (i/200) [half the amplitude]...
	    for(int i = 0; i <step; i++){
	        DAC_play(i/100);		
	    }
	     for(int i = step ;i > 0 ; i--){
	        DAC_play(i/200);		
	        }
}

void playNoteAndPause(int step, int wait, int pause){
  // if(*TIMER1_CNT == SAMPLE_PERIOD){
  // Takes the desired step and forwards it to noteGenerator_2 while maintaining this note for the desired i = wait (based on experience).
  // Be ce careful on changing wait as this is how long it will hold the note. The pause is the pause after the note.
	  for(int i = 0; i < wait; i++) {
	    noteGenerator_2(step);
	  }

	  for(int i = 0; i < pause ; i++){
	    // DAC_play(0);
	  }
	  //  }
}



void critialHealth(){
  //Outdated Can be Ignored
	if(*TIMER1_CNT == SAMPLE_PERIOD)
		{
			*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
			/*
			DAC_play(note[i]);
			i++;
			if (i == 168)
			{
				i = 0;
			}
			*/
			for(int n = 3; n < 5;	n++)
			{
			  for(int d = 0; d < 2500; d++){
				for(int k = 0; k <20; k++)
				{
				  	DAC_play(5*k/n);
					DAC_play(5*k/d/n);
					for(int i = 0; i<2;i++){
					  DAC_play(0);		
					}			
				}
			  }
			}
		}

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

void tone(int period,int amp)
{

	if(*TIMER1_CNT == SAMPLE_PERIOD)
		{
			*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
			for(int i = 0; i < 10; i++)
			{
			DAC_play(amp);
			DAC_play(amp*2);
			}
		}
	setupTimer(period);
	if(*TIMER1_CNT == SAMPLE_PERIOD)
		{
			*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
			for(int i = 0; i < 10; i++)
			{
			DAC_play(0);
			}
		}
}

void DAC_play(int data)// sends data to DAC
{	
  *DAC0_CH0DATA = data*6; //Make up for the game somewhere...
  *DAC0_CH1DATA = data*6;
}
/*void playNote(uint16_t wavelengt, uint16_t duration)
{
uint16_t elapsed;

uint16_t i;
	for(elapsed = 0; elapsed < duration; elsapsed += wavelenght)
	{
		//for loop with variable delay selects the pitch
		
		for(i = 0; i < wavelength; i++)
		{
			
		}
		*DAC0_CH0DATA ^= (1<< SPEAKER);
	}
}
/*

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
