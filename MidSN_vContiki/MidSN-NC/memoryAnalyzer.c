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


#include "midsn-nc.h"



#define CONFIG_STACK_MONITOR 1
/* Set to 1 to activate debug mode */
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


PROCESS(midsn, "MidSN-NC");
AUTOSTART_PROCESSES(&midsn);

#if CONFIG_STACK_MONITOR
extern uint8_t __bss_end; // lowest stack address
extern uint8_t __stack; // highest stack address
#endif

uint16_t stack_size(void) // available stack  Free (4689)
{
    return (uint16_t)&__stack - (uint16_t) & __bss_end + 1;
}



PROCESS_THREAD(midsn, ev, data) {

    static struct etimer et;

    PROCESS_BEGIN();

    /* Print on process startup */
    PRINTF("MidSN-NC process started\n");





    printf("\nStack %u bytes!\n", stack_size());

    static uint16_t sensors[2300];


    while (1) {

        /* Delay 2-4 seconds */
        etimer_set(&et, CLOCK_SECOND * 2 + random_rand() % (CLOCK_SECOND * 2));

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));



        PRINTF("Aqui...\n");


        printf("\nStack overflow within %u bytes!\n", stack_size());


        /*
                if (*(uint16_t *) (&__bss_end + 100) != 0x4242) {
                    printf(("\nStack Warning, overflow within 100 bytes!\n"));
                    if (__bss_end != 0x4242) {
                        __bss_end = 0x4242;
                        printf(("\n!!!!!!!Stack Overflow!!!!!!!!\n"));
                    }
         *(uint16_t *) (&__bss_end + 100) = 0x4242;
                }
         */



    }
    PRINTF("PROCESS END");
    PROCESS_END();
}








