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

#include <node-id.h>

#include "powertrace.h"


#include "midsn-nc.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-aa/aquisition.h"
#include "nc-ginapp-gp/events.h"
#include "nc-ginapp-gp/compute.h"
#include "ioadapter/midsn_packet.h"
#include "drivers/communication_protocol.h"
#include "ioadapter/communication.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-gp/clockMidSN.h"

#define CONFIG_STACK_MONITOR 1

/* Set to 1 to activate debug mode */
#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


PROCESS(midsn, "MidSN-NC");
AUTOSTART_PROCESSES(&midsn, &clockMidSN_process);

u8_t msgWaiting = 0;

#if CONFIG_STACK_MONITOR
extern uint8_t __bss_end; // lowest stack address
extern uint8_t __stack; // highest stack address
#endif

void stack_size(u8_t position) // available stack  Free (4689)
{
    //printf("\nStack at position %u has %u bytes free!\n", position, (uint16_t) & __stack - (uint16_t) & __bss_end + 1);
}

void init_MidSN_Config() {


    struct stream stream;
    stream.streamID = 1;
    stream.isIp = FALSE;

    sprintf(stream.ipTo, "localhost");

    stream.ipPort = 9000;
    stream.isAlarm = FALSE;
    stream.rimeTo = 0x0001;
    stream.send_rate = 1000;
    stream.window_size = 1; //1; //10; //50; //100; //1000;
    stream.rleOn = FALSE;
    stream.rleInterval = 100;
    stream.rleIndex = MAXINT;
    stream.last_sendTS = currentTimeMilliseconds();
    stream.rleAliveTime = 10000;
    stream.timeStartRle = currentTimeMilliseconds();
    stream.rleMaxSize = 25;
    stream.isIntervalRel = TRUE;
    stream.nSeq = 0;

    stream.is_one_time_query = FALSE;


    //stream.select[0].sensor = FFIELD_sensor_LASTSENDINTERVAL;

    stream.select[0].sensor = SENSOR_TEMPERATURE;
    //FFIELD_VALUE; //FFIELD_AVG; //FFIELD_MAX; //FFIELD_MIN; //FFIELD_COUNT; //FFIELD_SUM; //FFIELD_VARIANCE; //90; 
    stream.select[0].metric = FFIELD_VALUE;
    //stream.select[1].metric = FFIELD_AVG;


    /*    stream.where[0].field1.sensor = SENSOR_TEMPERATURE;
        stream.where[0].field1.metric = FFIELD_VALUE;
        stream.where[0].operator = OPERATOR_GREATER_OR_EQUAL;
        stream.where[0].field2.sensor = FFIELD_sensor_VALUE;
        stream.where[0].field2.metric = 6000;
     */
    stream.where[0].field1.sensor = FFIELD_sensor_END;



    /*
     *
     * Para testes de tempos dos sensores
     *
     *
    stream.select[0].sensor=FFIELD_sensor_LASTSENDINTERVAL;

    stream.select[0].sensor=SENSOR_NOTHING;
    stream.select[0].metric=FFIELD_VALUE;

    stream.select[2].sensor=SENSOR_TEMPERATURE;
    stream.select[2].metric=FFIELD_VALUE;

    stream.select[0].sensor=SENSOR_HUMIDITY;
    stream.select[0].metric=FFIELD_VALUE;


    stream.select[0].sensor=SENSOR_LIGHT;
    stream.select[0].metric=FFIELD_VALUE;

    stream.select[0].sensor=SENSOR_LIGHT2;
    stream.select[0].metric=FFIELD_VALUE;
     */




    stream.select[1].sensor = FFIELD_sensor_END;
    ev_addStreamAndEvent(&stream, 1000);

    /*

         // MERGE
        stream.streamID = 2;
        stream.ipPort = 9000;
        stream.isAlarm = FALSE;
        stream.send_rate = 100; //para já não uso
        stream.window_size = 10;
        stream.rleOn = TRUE;
        stream.rleInterval = 100;
        stream.rleIndex = MAXINT;
        stream.last_sendTS = currentTimeMilliseconds();
        stream.rleAliveTime = 10000;
        stream.timeStartRle = currentTimeMilliseconds();
        stream.rleMaxSize = 5;
        stream.isIntervalRel = TRUE;
        stream.nSeq = 0;

        stream.select[0].sensor = SENSOR_NOTHING;
        stream.select[0].metric = FFIELD_MERGE;
        stream.select[1].sensor = FFIELD_sensor_END;

        ev_addStreamAndEvent(&stream, 10000);

     */


    struct sensor sensor;


    sensor.nsamples = 0;
    sensor.type = SENSOR_TEMPERATURE;
    sensor.window_pos = 0;
    ev_addSensorAndEvent(&sensor, 1000);



    /* 
            sensor.nsamples = 0;
            sensor.type = SENSOR_NOTHING;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 1000);

            sensor.nsamples = 0;
            sensor.type = SENSOR_TEMPERATURE;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 1000);

            sensor.nsamples = 0;
            sensor.type = SENSOR_HUMIDITY;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 5000);

            sensor.nsamples = 0;
            sensor.type = SENSOR_BATTERY;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 1000);

            sensor.nsamples = 0;
            sensor.type = SENSOR_LIGHT2;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 5000);

            sensor.nsamples = 0;
            sensor.type = SENSOR_LIGHT2;
            sensor.window_pos = 0;
            ev_addSensorAndEvent(&sensor, 1000);
     */


    /***********************  announcement of reboot ***************************/

    //comm_sendWakeUpInfo();

    /***********************  announcement of reboot END ***************************/


    /*
        etimer_set(&et, CLOCK_SECOND);

        //    etimer_set(&etscs, CLOCK_SECOND * 5);
        set_config_status_etimer(5000);
     */

}

PROCESS_THREAD(midsn, ev, data) {

    PROCESS_BEGIN();

    /* Print on process startup */
    PRINTF("MidSN-NC process started\n");
    /* Start powertracing, once every minute. */
    //powertrace_start(CLOCK_SECOND * 60);


    stack_size(1);

    init_protocol();

    PRINTF("I'm not the sink node\n");
    // initialize sensors and streams;
    ev_initializeAll();

    init_MidSN_Config();

    while (1) {

        PROCESS_WAIT_EVENT();


        if (msgWaiting == 1) {

            stack_size(2);

            struct midsn_packet * pkt = midsn_get_receive_packet();
            comm_receive_handler(pkt);
            stack_size(3);
            msgWaiting = 0;
        }

        if (getAllEventsAreRunningSetRestartIfStarts() == FALSE) continue;
        ev_handleEvents();


    }
    PRINTF("PROCESS END");
    PROCESS_END();
}








