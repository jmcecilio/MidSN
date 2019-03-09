/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

/* Temperature and Humidity*/
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"


#include <stdio.h>
#include <string.h>

#include "midsn-nc.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-aa/aquisition.h"
#include "nc-ginapp-gp/events.h"
#include "nc-ginapp-gp/compute.h"
#include "nc-ginapp-gp/clockMidSN.h"



static struct stream streams[NUMBER_STREAMS];

static struct sensor sensors[NUMBER_SENSORS];

void initialize_sensors() {
    int i=0;
    for(i=0;i<NUMBER_SENSORS;i++)
        sensors[i].type=SENSOR_NONE;

}

void initialize_streams() {
    int i=0;
    for(i=0;i<NUMBER_STREAMS;i++) {
        streams[i].streamID=STREAM_NONE;
        streams[i].countSensorSamplingRates=0;
    }

}

void initialize_stream(struct stream *stream) {
	stream->streamID=STREAM_NONE;

	stream->isAlarm = FALSE;	//=FALSE;

	stream->send_rate = 1000;	//=1000;
        stream->send_rate = 1000;
	stream->window_size = 1;	//=1;

        int i = 0;
        for (i=0;i<MAXFORMATFIELDS; i++)
	stream->select[i].sensor = FFIELD_sensor_END;
        for (i=0;i<MAXALARMCONDITIONS; i++)
	stream->select[i].sensor = FFIELD_sensor_END;

	stream->nSeq = 0;

        stream->rleOn = FALSE; // =FALSE;
	stream->rleIndex=0;
	stream->rleMaxSize = MAXRLESIZE;
	stream->rleAliveTime = 10000; // maximum time to keep rle values
	stream->timeStartRle = currentTimeMilliseconds(); // started rle time

	stream->last_sendTS=0; // register of last send time

	stream->rleInterval = 0; //não sei se é para acrescentar :D
	stream->isIntervalRel = FALSE; // TRUE rleInterval relative to previous FALSE: relative to absolute
				// previous +- rleInterval VS value /rleInterval
        stream->countSensorSamplingRates = 0; //sò é utilizado para a 2a versão do código

        stream->last_computeInterval =0; // only time to compute
        stream->last_sendInterval=0 ;  // only send command
};

int aq_dropSensor(int sensorID) {
    int i=0;
    for(i=0;i<NUMBER_SENSORS;i++)
        if(sensors[i].type==sensorID){
            sensors[i].type=SENSOR_NONE;
            return i;
        }
    return -1;
}

int aq_dropStream(int i) {
/*
    int i=0;
    for(i=0;i<NUMBER_STREAMS;i++)
        if(streams[i].streamID==streamID){
            streams[i].streamID=STREAM_NONE;
            return i;
        }
    return -1;
*/
    streams[i].streamID=STREAM_NONE;
    return i;
}




int aq_SensorPosition(int sensorID) {
    int i=0;
    for(i=0;i<NUMBER_SENSORS;i++)
        if(sensors[i].type==sensorID)
            return i;

    return -1;
}

int aq_StreamPosition(int streamID) {
    int i=0;
    for(i=0;i<NUMBER_STREAMS;i++)
        if(streams[i].streamID==streamID)
            return i;
    
    return -1;

}



int aq_changeStreamWindowSize(uint16_t streamID, uint16_t value){

    int pos = aq_StreamPosition(streamID);
    if (pos != -1){
        streams[pos].window_size = value;
         return 0;
    }
    return -1;
}


int aq_resetNseqStreams(uint16_t streamID){

    int pos = aq_StreamPosition(streamID);
    if (pos != -1){
        streams[pos].nSeq = 0;
         return 0;
    }
    return -1;
}

int aq_resetCountSamples(uint16_t sensorType){

    int pos = aq_SensorPosition(sensorType);
    if (pos != -1){
        sensors[pos].nsamples = 0;
         return 0;
    }
    return -1;
}

int aq_getCountSamples(uint16_t sensorType){

    int pos = aq_SensorPosition(sensorType);
    if (pos != -1){
        return sensors[pos].nsamples;
    }
    return -1;
}


void aq_set_aquisitionInterval(uint16_t id, uint32_t timeToInterval, uint32_t now){
    sensors[id].last_aquisitionInterval= now - timeToInterval;
}

void aq_set_computeInterval(uint16_t id, uint32_t timeToInterval, uint32_t now){
    streams[id].last_computeInterval= now - timeToInterval;
}

void aq_set_SendInterval(uint16_t id, uint32_t timeToInterval, uint32_t now){
    streams[id].last_sendInterval= now - timeToInterval;
}


uint16_t aq_get_aquisitionInterval(uint16_t sensorType){

    int pos = aq_SensorPosition(sensorType);
    if (pos != -1){
        return sensors[pos].last_aquisitionInterval;
    }
    return 0;
}

uint16_t aq_get_computeInterval(uint16_t streamID){
    int pos = aq_StreamPosition(streamID);
    if (pos != -1){
        return streams[pos].last_computeInterval;
    }
    return 0;
}

uint16_t aq_get_SendInterval(uint16_t streamID){
    int pos = aq_StreamPosition(streamID);
    if (pos != -1){
        return streams[pos].last_sendInterval;
    }
    return 0;
}




void aq_addSensor(int i, struct sensor *sensor) {
    //	sensors[i] = sensor;
    memcpy(&sensors[i], sensor, sizeof (struct sensor));
}

void aq_addStream(int i, struct stream *stream) {
    memcpy(&streams[i], stream, sizeof (struct stream));

    // printf("streamID %d %d \n", streams[i].streamID, streams[i].ipPort);
}

struct sensor * aq_getSensorPointer(int i) {
    int j = aq_SensorPosition(i);
    return &(sensors[j]);
}

struct stream * aq_getStreamPointer(int i) {
    return &(streams[i]);
}



void aq_adquire(int i) {
/*
    printf("Start aquisition...\n");
*/
    aq_placeInSensorWindow(i, adquire(sensors[i].type));
/*
    printf("End aquisition...\n");
*/
 //   printf("i %u type %u v=%u", i, sensors[i].type, adquire(sensors[i].type) );
}

void aq_placeInSensorWindow(int i, uint16_t value) {

    if (sensors[i].window_pos >= MAXWINDOWSIZE)
        sensors[i].window_pos = 0;

    sensors[i].window_array[sensors[i].window_pos] = value;

//   printf("Acq: sensors[%d].window_pos[%d] = %u\n",i,sensors[i].window_pos,sensors[i].window_array[sensors[i].window_pos]);

    sensors[i].window_pos++;
    sensors[i].nsamples++;  // incrementa o numero de aquisições

//    printf("nsamples = %u\n",sensors[i].nsamples);
    
    
    
}



