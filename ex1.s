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

cmu_base_addr:
		.long CMU_BASE
gpio_pa_base:
		.long GPIO_PA_BASE
gpio_pc_base:
		.long GPIO_PC_BASE

// Load CMU base address
ldr r1 , cmu_base_addr

// load current value of HFPERCLK ENABLE
ldr r2 , [r1 , #CMU_HFPERCLKEN0]

// set bit for GPIO clk
mov r3 , #1
lsl r3 , r3 , #CMU_HFPERCLKEN0_GPIO
orr r2 , r2 , r3

//store new value
str r2 , [r1 , #CMU_HFPERCLKEN0]


//set LED HIGH drive strenght
ldr r1 , gpio_pa_base
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
ldr r1 , gpio_pc_base
ldr r2 , =#0x33333333
str r2 , [r1 , GPIO_MODEL]
ldr r2 , =#0xff
str r2 , [r1 , #GPIO_DOUT]
ldr r2 , = #0x22222222
str r2 , [r1 , #GPIO_EXTIPSELL]

// Set to interupt ON FALLING AND RISING EDGE
ldr r2 , = #0xff
str r2 , [r1 , #GPIO_EXTIFALL]
str r2 , [r1 , #GPIO_EXTIRISE]
str r2 , [r1 , #GPIO_IEN]
ldr r2 , =#0x802
iser0:
	.long ISER0
ldr r3 , iser0
str r2 , [r3]
	






				
	      bx lr // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:

ldr r1 , gpio_pc_base
ldr r2 , [r1 , #GPIO_DIN]
lsl r2 , r2 , #8
ldr r3 , gpio_pa_base
str r2 , [r3 , #GPIO_DOUT]


ldr r1 , gpio_pc_base
ldr r2 , [r1 , #GPIO_IF]
str r2 , [r1 , #GPIO_IFC]


	      bx lr  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:

ldr r1 , gpio_pc_base
ldr r2 , [r1 , #GPIO_DIN]
lsl r2 , r2 , #8
ldr r3 , gpio_pa_base
str r2 , [r3 , #GPIO_DOUT]


ldr r1 , gpio_pc_base
ldr r2 , [r1 , #GPIO_IF]
str r2 , [r1 , #GPIO_IFC]
	
        bx lr // do nothing
