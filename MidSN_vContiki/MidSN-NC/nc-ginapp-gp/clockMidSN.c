/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 * TelosB UDP Client
 */
 
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include <stdio.h>
#include <string.h>


/* Set to 1 to activate debug mode */
#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static uint32_t clockCounter=0;
static uint32_t clockLastTicks=0;


void clockMidSNInit()
{
  clockCounter=0;
  clockLastTicks=clock_time();
}

uint32_t currentTimeMilliseconds()
{
	static uint32_t time = 0;
	static uint32_t clockNowTicks = 0;

	clockNowTicks = clock_time();
	if( clockLastTicks > clockNowTicks)			
		clockNowTicks = clockNowTicks + 65535;
	time = (( (clockNowTicks - clockLastTicks) * 1000 ) / CLOCK_SECOND);
	time = (((clockCounter * 420) * 1000) + time);
  return time;
}

uint32_t currentTimeSeconds(void)
{
  	static uint32_t time = 0;
  	static uint32_t clockNowTicks = 0;

	clockNowTicks = clock_time();
	if(clockLastTicks > clockNowTicks)			
		clockNowTicks = clockNowTicks + 65535;
	time = ( (clockNowTicks - clockLastTicks)  ) / CLOCK_SECOND;
	time = clockCounter * 420  + time;
  return time;
}


PROCESS(clockMidSN_process, "Local Clock");
//AUTOSTART_PROCESSES(&clockMidSN_process);



PROCESS_THREAD(clockMidSN_process, ev, data)
{
	static struct etimer et;

	PROCESS_BEGIN();
        clock_time_t time = ((clock_time_t) CLOCK_SECOND ) * ((clock_time_t) 420 );
        etimer_set(&et, time );
  while (1) {
		PROCESS_YIELD();
		if(etimer_expired( &et )) {
		 	clockCounter++;
			clockLastTicks = clock_time();		
		 	etimer_restart(&et);
		}
   }
   PRINTF("PROCESS END");
   PROCESS_END();
}
