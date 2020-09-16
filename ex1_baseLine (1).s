        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset:

 

// Load CMU base address
ldr r1 , =CMU_BASE

// load current value of HFPERCLK ENABLE
ldr r2 , [r1 , #CMU_HFPERCLKEN0]

// set bit for GPIO clk
mov r3 , #1
lsl r3 , r3 , #CMU_HFPERCLKEN0_GPIO
orr r2 , r2 , r3

//store new value
str r2 , [r1 , #CMU_HFPERCLKEN0]


//set LED HIGH drive strenght
ldr r1 , =GPIO_PA_BASE
ldr r2 , = #0x2
str r2 , [r1 , #GPIO_CTRL]

//set GPIO PA to out
ldr r2 , = #0x55555555
str r2 , [r1 , #GPIO_MODEH]

//Set LED VALUE
// load current value of GPIO_PA_DOUT
ldr r2 , [r1 , #GPIO_DOUT]

// set led value
ldr r3 , = #0xff00
orr r2 , r2 , r3
	
// save new value
str r2 , [r1 , #GPIO_DOUT]

	
// set button on gpio pc
ldr r1 , =GPIO_PC_BASE
ldr r2 , = #0x33333333
str r2 , [r1 , #GPIO_MODEL]
	

ldr r2 , =#0xff
str r2 , [r1 , #GPIO_DOUT]



// -----------------------------------------------------------------------------------
// connecting button to LED
main_loop:

// set diode value
//load GPIO BASE
ldr r1 , =GPIO_PA_BASE

// load current value of GPIO_PA_DOUT(LED)
ldr r2 , [r1 , #GPIO_DOUT]

// set led value
ldr r3 , =GPIO_PC_BASE
ldr r4 , [ r3 , #GPIO_DIN] //Button
lsl r4 , r4 , #8 
	
// save new value
str r4 , [r1 , #GPIO_DOUT]


				
	      b main_loop  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
