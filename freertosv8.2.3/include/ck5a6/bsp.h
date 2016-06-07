#ifndef  __BSP_FILE__
#define  __BSP_FILE__

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdint.h>

#include  "portmacro.h"
#include  "FreeRTOS.h"
#include  "semphr.h"


#define APB_DEFAULT_FREQ       	50000000   
#define AHB_DEFAULT_FREQ      	50000000   

#define OS_TICK_FREQ           		1000  

#define RAM_ADDR_START 		0x08000000
#define RAM_ADDR_END  		 	0x08800000



void BSP_Init (void);


#endif                                                        
