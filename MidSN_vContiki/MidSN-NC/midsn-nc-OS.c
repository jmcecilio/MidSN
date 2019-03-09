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


PROCESS(midsn, "MidSN-NC");
AUTOSTART_PROCESSES(&midsn);



PROCESS_THREAD(midsn, ev, data) {

    PROCESS_BEGIN();

    /* Print on process startup */
    PRINTF("MidSN-NC process started\n");
    
    while (1) {

        PROCESS_WAIT_EVENT();


    }
    PRINTF("PROCESS END");
    PROCESS_END();
}








