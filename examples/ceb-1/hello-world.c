/**
 * \file
 *         Basic Test Event Timer example
 * \author
 *         Shahin Ahmadi <st_sh.ahmadi@urmia.ac.ir>
 * \		
 *			IoTClinic
 * \		
 *			UUCERT
 */
//-----------------------------Library----------------
#include "contiki.h"
#include <stdio.h>
//-----------------------------Library----------------
PROCESS(hello-world,"Hello World");
AUTOSTART_PROCESSES(&hello-world);
//----------------------------Threads-----------------
PROCESS_THREAD(hello-world , ev , data)
{
 PROCESS_BEGIN();
	  printf("Hello World.\n");
 PROCESS_END();
}