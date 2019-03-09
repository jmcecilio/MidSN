/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"


#include <stdio.h>
#include <string.h>



#include "midsn-nc.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-aa/aquisition.h"


#include "nc-ginapp-gp/events.h"
#include "nc-ginapp-gp/compute.h"
#include "nc-ginapp-gp/clockMidSN.h"
#include "ioadapter/midsn_packet.h"
#include "ioadapter/communication.h"



#define PRINTVALUES  0

static int allEventsAreRunning = TRUE;

static struct event eventsSensors[NUMBER_SENSORS];

static struct event eventsStreams[NUMBER_STREAMS];

static uint16_t sensorSamplingRate;

void setAllEventsAreRunning(int i) {
    allEventsAreRunning = i;
}

int getAllEventsAreRunningSetRestartIfStarts() {
    if (delayedStart_etimer_expired()) {
        if (allEventsAreRunning == FALSE)
            ev_restart_all_events();
        allEventsAreRunning = TRUE;
    }

    return allEventsAreRunning;
}

void set_delayedStart_etimer(uint32_t timeIntervalInMillis) {
    set_delayedStart_so_timer(ev_samplingTicks(timeIntervalInMillis));
}

int delayedStart_etimer_expired() {
    return delayedStart_so_timer_expired();
}

void ev_stop_all_events() {
    allEventsAreRunning = FALSE;
    ev_stop_all_event(eventsSensors, NUMBER_SENSORS);
    ev_stop_all_event(eventsStreams, NUMBER_STREAMS);
}

void ev_restart_all_events() {
    ev_restart_all_event(eventsSensors, NUMBER_SENSORS);
    ev_restart_all_event(eventsStreams, NUMBER_STREAMS);
    allEventsAreRunning = TRUE;
}

int ev_addEvent(struct event * eventsPointer, int size, int eventType, uint16_t sampling) {
    int i = 0;
    for (i = 0; i < size; i++)
        if (eventsPointer[i].isThing == eventNONE) {
            eventsPointer[i].sample_rate = sampling;
            eventsPointer[i].isThing = eventType;
            set_so_timer(&eventsPointer[i].et, ev_samplingTicks(sampling));
            // ev_printEvent(events[i]);
            return i;
        }
    return -1;
}

void ev_initialize_event(struct event * eventsPointer, int size) {
    int i;
    for (i = 0; i < size; i++) {
        eventsPointer[i].isThing = eventNONE;
        stop_so_timer(&eventsPointer[i].et);
    }
}

void ev_stop_all_event(struct event * eventsPointer, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (eventsPointer[i].isThing != eventNONE)
            stop_so_timer(&eventsPointer[i].et);
    }
}

void ev_restart_all_event(struct event * eventsPointer, int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (eventsPointer[i].isThing != eventNONE)
            set_so_timer(&eventsPointer[i].et, ev_samplingTicks(eventsPointer[i].sample_rate));
    }
}


static struct valuesToSendArrayStruct * valuesToSendArrayStructPointer;

void ev_sendValues(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer) {

#if PRINTVALUES
    int i = 0;
    for (i = 0; i < valuesToSendArrayStructPointer->size; i++)
        printf("\nvaluesToSend[%d] = %u\n", i, valuesToSendArrayStructPointer->valuesPointer[i]);
#endif
    comm_send_values(streamp, valuesToSendArrayStructPointer);
    // (     valuesToSendArrayStructPointer->valuesPointer, valuesToSendArrayStructPointer->size)
    streamp->nSeq++;
    streamp->last_sendTS = currentTimeMilliseconds();
}

int ev_handleEventNoThrills() {

    if (!sensorTimer_so_timer_expired())
        return -1;

    reset_sensorTimer_so_timer();

    int i = 0;
    for (i = 0; i < NUMBER_SENSORS; i++) {

        if (sensors[i].type == SENSOR_NONE)
            continue;
        //        int expirou=etimer_expired(&eventsPointer[i].et);
        //        printf("i %d expirou? %d type %d\n",i,expirou,type);


        aq_adquire(i);
    }


    for (i = 0; i < NUMBER_STREAMS; i++) {

        if (aq_getStreamPointer(i)->streamID == STREAM_NONE)
            continue;

        aq_getStreamPointer(i)->countSensorSamplingRates++;

        printf("ssr %u cssr %u sr %u\n", sensorSamplingRate, aq_getStreamPointer(i)->countSensorSamplingRates, aq_getStreamPointer(i)->send_rate);
        if (sensorSamplingRate * aq_getStreamPointer(i)->countSensorSamplingRates >= aq_getStreamPointer(i)->send_rate) {

            valuesToSendArrayStructPointer = comp_compute(&(eventsStreams[i]), aq_getStreamPointer(i));


            ev_sendValues(aq_getStreamPointer(i), valuesToSendArrayStructPointer);

            aq_getStreamPointer(i)->countSensorSamplingRates = 0;

        }
    }

    return 0;

}

int ev_handleEvent(struct event * eventsPointer, int size, int type) {

    int i = 0;
    static uint32_t timeToInterval = 0;

    for (i = 0; i < size; i++) {

        if (eventsPointer[i].isThing == eventNONE)
            continue;
        /*
                int expirou=etimer_expired(&eventsPointer[i].et);
                printf("i %d expirou? %d type %d\n",i,expirou,type);
         */



        if (so_timer_expired(&eventsPointer[i].et)) {

            //            etimer_set(&eventsPointer[i].et, ev_samplingTicks(eventsPointer[i].sample_rate) );

            if (type == eventSENSOR) {
                timeToInterval = currentTimeMilliseconds();
                aq_adquire(i);
                aq_set_aquisitionInterval(i, timeToInterval, currentTimeMilliseconds());
            }
            else if (type == eventSTREAM) {

                timeToInterval = currentTimeMilliseconds();
                valuesToSendArrayStructPointer = comp_compute(&(eventsPointer[i]), aq_getStreamPointer(i));
                aq_set_computeInterval(i, timeToInterval, currentTimeMilliseconds());


                if (valuesToSendArrayStructPointer->size > 0) {
                    timeToInterval = currentTimeMilliseconds();
                    ev_sendValues(aq_getStreamPointer(i), valuesToSendArrayStructPointer);
                    aq_set_SendInterval(i, timeToInterval, currentTimeMilliseconds());
                }



            } else
                return -1;



            if (type == eventSTREAM && aq_getStreamPointer(i)->is_one_time_query == TRUE) {
                ev_dropStreamAndEvent(i);
                printf("IS ONE TIME QUERY = %d\n", aq_getStreamPointer(i)->is_one_time_query);
                printf("IS THING = %d\n", eventsStreams[i].isThing);
            } else
                reset_so_timer(&eventsPointer[i].et);


        }
    }

    return 0;

}

/**********************************/




uint32_t ev_samplingTicks(uint32_t sampling_rate) {
    uint32_t ticks = (sampling_rate * (uint32_t) CLOCK_SECOND) / (uint32_t) 1000;
    //	if(	((sampling_rate*(uint32_t)CLOCK_SECOND ) % (uint32_t)1000) != 0 ) ticks++; // rounding alternativo (estamos a round no aq_subtractTime
    return ticks;

    //	return (sampling_rate>>7);
}

/*********************************************************/

void ev_printEvent(struct event event) {
    printf("event: sr type: %d %d\n", event.sample_rate, event.isThing);
}

/**********************************************************/

uint16_t ev_getSensorEventSampleRate(int sensorID) {
    return eventsSensors[aq_SensorPosition(sensorID)].sample_rate;

}

int ev_addEvents(int eventType, uint16_t sampling) {

    if (eventType == eventSENSOR)
        return ev_addEvent(eventsSensors, NUMBER_SENSORS, eventSENSOR, sampling);
    if (eventType == eventSTREAM)
        return ev_addEvent(eventsStreams, NUMBER_STREAMS, eventSTREAM, sampling);

    return -1;
}

int ev_addSensorAndEvent(struct sensor *sensor, uint16_t sampling_rate) {
    int id = ev_addEvents(eventSENSOR, sampling_rate);
    if (id == -1)
        return -1;
    aq_addSensor(id, sensor);

    //    etimer_set(&sensorTimer, ev_samplingTicks(sampling_rate)); // PNF04112009
    sensorSamplingRate = sampling_rate;

    return id;
}

int ev_addStreamAndEvent(struct stream *stream, uint16_t sampling_rate) {
    int id = ev_addEvents(eventSTREAM, sampling_rate);
    if (id == -1)
        return -1;
    //printf("id de stream %d\n",id);
    //printf("id de stream (2) %d\n",streamIndexFromEvent(id));
    aq_addStream(id, stream);
    return id;
}

int ev_dropSensorAndEvent(int id) {
    int i = aq_dropSensor(id);
    if (i != -1)
        ev_dropEventSensor(i);

    return i;
}

int ev_dropStreamAndEvent(int id) {
    int i = aq_dropStream(id);
    //    printf("i = %d\n", i);
    if (i != -1)
        ev_dropEventStream(i);

    return i;
}

void ev_dropEventSensor(int i) {
    eventsSensors[i].isThing = eventNONE;
}

void ev_dropEventStream(int i) {
    //    printf("ev_dropEventStream OK\n");
    eventsStreams[i].isThing = eventNONE;

}

void ev_initialize_events() {
    ev_initialize_event(eventsSensors, NUMBER_SENSORS);

    ev_initialize_event(eventsStreams, NUMBER_STREAMS);
}

void ev_handleEvents() {
    ev_handleEvent(eventsSensors, NUMBER_SENSORS, eventSENSOR);
    ev_handleEvent(eventsStreams, NUMBER_STREAMS, eventSTREAM);
}

/**********************************/


void ev_initializeAll() {
    initialize_streams();
    initialize_sensors();
    ev_initialize_events();

}

int ev_changeSensorSampleRate(uint16_t sensorType, uint16_t value) {

    int pos = aq_SensorPosition(sensorType);

    if (pos != -1) {
        eventsSensors[pos].sample_rate = value;
        set_so_timer(&eventsSensors[pos].et, ev_samplingTicks(eventsSensors[pos].sample_rate));
        return 0;
    }
    return -1;
}

int ev_changeStreamSampleRate(uint16_t streamID, uint16_t value) {

    int pos = aq_StreamPosition(streamID);
    if (pos != -1) {
        eventsStreams[pos].sample_rate = value;
        set_so_timer(&eventsStreams[pos].et, ev_samplingTicks(eventsStreams[pos].sample_rate));
        return 0;
    }
    return -1;
}

int ev_changeStreamWindowSize(uint16_t streamID, uint16_t value) {

    int pos = aq_StreamPosition(streamID);
    if (pos != -1) {
        streams[pos].window_size = value;
        return 0;
    }
    return -1;
}







