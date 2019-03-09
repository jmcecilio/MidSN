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
#include "dev/sht11.h"
/* Light */
#include "dev/light.h"
/* Battery */
#include "dev/battery-sensor.h"

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





#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
#define USE_SAMPLE_GAP TRUE	
	// stream has a sample_rate (s-sr) and sensors have a possibly different sample_rate (se-sr)
	// the number of elements of the window that is relevant for a stream is s-sr/se-sr * s.window_sz
	// the number of elements to collect for a stream may be all (USE_SAMPLE_GAP=FALSE) (COUNT = s-sr/se-sr * s.window_sz
	// or only the subset that is needed (USE_SAMPLE_GAP=TRUE) s.window_sz

static uint16_t valuesToSend [MAX_SELECT_DATA_SIZE]; 

static uint16_t * valuesToSendp; 


 static struct statistics stats_sensors[NUMBER_SENSORS];


static struct valuesToSendArrayStruct valuesToSendArrayStruct;


struct valuesToSendArrayStruct *  comp_compute(struct event * eventp, struct stream * streamp) {



//	printf("envio do stream %d %d\n",streamp->streamID, currentTimeMilliseconds()-streamp->last_send);

	valuesToSendp = valuesToSend;

//	streamp->last_send = currentTimeMilliseconds();

/*************************************************************************************************************/
        
        static struct field select_field;

	static int i = 0;

        static int condition = FALSE;

        init_statistics();

        condition = compute_where_fields(eventp, streamp);

        if (condition == TRUE) {
            i = 0;
            for (i = 0; i < MAXFORMATFIELDS; i++) {
                select_field = streamp->select[i];
                if (select_field.sensor == FFIELD_sensor_END)
                    break;

                //PRINTF("stream.streamID = %d cur_field.sensor = %u, cur_field.metric = %u\n",streamp->streamID, cur_field.sensor, cur_field.metric);
/*
                printf("Start computation...\n");
*/
                valuesToSendp = compute(&select_field, valuesToSendp, eventp, streamp);
/*
                printf("End computation...\n");
*/

            }

            valuesToSendArrayStruct.valuesPointer = valuesToSend;
            valuesToSendArrayStruct.size = valuesToSendp - valuesToSend;

            //	for(i=0;i<valuesToSendArrayStruct.size;i++)
            //		printf("\nvaluesToSend[%d] = %u\n",i,valuesToSend[i]);


            /*
                    return &valuesToSendArrayStruct;
             */
        } else {
            valuesToSendArrayStruct.valuesPointer = valuesToSend;
            valuesToSendArrayStruct.size = 0;
        }
        return &valuesToSendArrayStruct;
}

uint16_t compute_where_fields(struct event * eventp, struct stream * streamp){
    
    static struct where where_field;
    static struct field cur_field;

    static uint16_t where_values [MAX_SELECT_DATA_SIZE];
    static uint16_t * where_valuesp;
    static int i=0;

    where_valuesp = where_values;



    for (i = 0; i < MAXALARMCONDITIONS; i++) {

        where_field = streamp->where[i];
        cur_field = where_field.field1;

        if (cur_field.sensor == FFIELD_sensor_END)
            break;

        where_valuesp = compute(&cur_field, where_valuesp, eventp, streamp);

        cur_field = where_field.field2;
        where_valuesp = compute(&cur_field, where_valuesp, eventp, streamp);

        if (where_field.operator == OPERATOR_EQUAL) {
            if (where_values[i + i] == where_values[i + i + 1])
                continue;
            else
                return FALSE;
        }

        if (where_field.operator == OPERATOR_GREATER) {
            if (where_values[i + i] > where_values[i + i + 1])
                continue;
            else
                return FALSE;
        }

        if (where_field.operator == OPERATOR_SMALLER) {
            if (where_values[i + i] < where_values[i + i + 1])
                continue;
            else
                return FALSE;
        }
        if (where_field.operator == OPERATOR_GREATER_OR_EQUAL) {
            if (where_values[i + i] >= where_values[i + i + 1])
                continue;
            else
                return FALSE;
        }

        if (where_field.operator == OPERATOR_SMALLER_OR_EQUAL) {
            if (where_values[i + i] <= where_values[i + i + 1])
                continue;
            else
                return FALSE;
        }

        
    }
    return TRUE;
 
}

void init_statistics() {

    static int i = 0;

    for (i = 0; i < NUMBER_SENSORS; i++)
            computeStatisticsSTART(i);

}





uint16_t * compute(struct field *cur_field, uint16_t *valuesToSend, struct event * eventp, struct stream * streamp) {
	
    
//PRINTF("stream->streamID = %d cur_field->sensor = %u, cur_field->metric = %u\n",streamp->streamID, cur_field->sensor, cur_field->metric);
	if(cur_field->sensor == FFIELD_sensor_STREAMID) 
		{ valuesToSend[0]=streamp->streamID;valuesToSend++; return valuesToSend;}
        
	if(cur_field->sensor == FFIELD_sensor_NSEQSTREAM) 
		{ valuesToSend[0]=streamp->nSeq;valuesToSend++; return valuesToSend;}

	if(cur_field->sensor == FFIELD_sensor_NODEID) 
		{ valuesToSend[0]=get_nodeID();valuesToSend++; return valuesToSend;}

	if(cur_field->sensor == FFIELD_sensor_VALUE)
		{ valuesToSend[0]=cur_field->metric;valuesToSend++; return valuesToSend;}

	if(cur_field->sensor == FFIELD_sensor_LASTSENDINTERVAL)
		{valuesToSend[0]=currentTimeMilliseconds() - streamp->last_sendTS;valuesToSend++; return valuesToSend;}

	if(cur_field->sensor == FFIELD_sensor_LASTSENDTS)
		{ ((uint32_t *)valuesToSend)[0]=streamp->last_sendTS;((uint32_t *)valuesToSend)++; return valuesToSend;}


	if(cur_field->sensor == FFIELD_sensor_NOW)
		{ ((uint32_t *)valuesToSend)[0]=currentTimeMilliseconds();((uint32_t *)valuesToSend)++; return valuesToSend;}

        if(cur_field->sensor == FFIELD_sensor_LAST_COMPUTE_INTERVAL)
		{ valuesToSend[0]= aq_get_computeInterval(streamp->streamID);valuesToSend++; return valuesToSend;}

        if(cur_field->sensor == FFIELD_sensor_LAST_SEND_INTERVAL)
		{ valuesToSend[0]=aq_get_SendInterval(streamp->streamID);valuesToSend++; return valuesToSend;}


// valid sensor fields now can only be sensors
	if(cur_field->sensor<0 || cur_field->sensor>= NUMBER_SENSORS )
		return valuesToSend;

        if(cur_field->metric == FFIELD_sensor_NSEQSENSOR)
		{ valuesToSend[0]=aq_getCountSamples(cur_field->sensor);valuesToSend++; return valuesToSend;}

        if(cur_field->metric == FFIELD_LAST_AQUISITION_INTERVAL)
		{ valuesToSend[0]=aq_get_aquisitionInterval(cur_field->sensor);valuesToSend++; return valuesToSend;}


	if(cur_field->metric==FFIELD_VALUE || cur_field->metric==FFIELD_LAST)
		{ valuesToSend[0]=lastValue(aq_getSensorPointer(cur_field->sensor));valuesToSend++; return valuesToSend;}



	// remaining operations require numberOfSamples
	int numberOfSamples = eventp->sample_rate / ev_getSensorEventSampleRate(cur_field->sensor) * streamp->window_size;
	setSampleGap( eventp->sample_rate / ev_getSensorEventSampleRate(cur_field->sensor)-1);

//PRINTF("ns sg: %d %d \n", numberOfSamples,  getSampleGap());


	if(cur_field->metric==FFIELD_COUNT)
		{ if(USE_SAMPLE_GAP==FALSE) 
			valuesToSend[0]=numberOfSamples;
		  else 	
			valuesToSend[0]=streamp->window_size;
		  valuesToSend++; 
		  return valuesToSend;
		}

	if(cur_field->metric>=0 && cur_field->metric<=100) // percentiles
		{ 
			if(stats_sensors[cur_field->sensor].isFilledWithOrder==FALSE) 
				valuesToSend = visitToCompute(cur_field->sensor, streamp, numberOfSamples, valuesToSend, VISIT_COMPUTE_ORDER_AND_FILL);			
		 

			uint16_t index2= ( stats_sensors[cur_field->sensor].percentile_index*cur_field->metric )/100;
			int index = (int)index2-1;
			if(index<0) index=0;
			

/*
printf("stats_sensor: %u %u \n",index, stats_sensors[cur_field->sensor].percentile_index);
int j=0;
for(j=0;j<stats_sensors[cur_field->sensor].percentile_index;j++)
	printf("%u ",stats_sensors[cur_field->sensor].percentile_array[j]);
printf("end \n");
*/


			valuesToSend[0]=stats_sensors[cur_field->sensor].percentile_array[index];
			valuesToSend++; 
			return valuesToSend;						

		}		



	// MERGE AND RLE
	if( cur_field->metric==FFIELD_EACH_VALUE || cur_field->metric == FFIELD_MERGE) {
		valuesToSend = visitToCompute(cur_field->sensor, streamp, numberOfSamples, valuesToSend, VISIT_COMPUTE_MERGE);
		return valuesToSend;
	}


	if( cur_field->metric == FFIELD_RLE) {
		valuesToSend = visitToCompute(cur_field->sensor, streamp, numberOfSamples, valuesToSend, VISIT_COMPUTE_RLE);
		return valuesToSend;
	}



	// arrived at this point, we may assume the rest need window_array visitor, which computes statistics (MIN, MAX, AVG, variance) and places
	// data in ordered array for percentiles
	if( cur_field->metric !=  FFIELD_SUM && cur_field->metric !=  FFIELD_MAX &&  cur_field->metric !=  FFIELD_MIN && cur_field->metric !=  FFIELD_AVG && cur_field->metric !=  FFIELD_FIRST && cur_field->metric !=  FFIELD_VARIANCE && cur_field->metric != FFIELD_COUNTITERATIONS)
		return valuesToSend;
 
	// visit the guy, computing statistics and creating ordered array 
	if(stats_sensors[cur_field->sensor].isFilled==FALSE) 
			visitToCompute(cur_field->sensor, streamp, numberOfSamples, valuesToSend, VISIT_COMPUTE_STATS);
	if(cur_field->metric==FFIELD_COUNTITERATIONS) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].count;valuesToSend++; return valuesToSend;}	
	if(cur_field->metric==FFIELD_MAX) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].max;valuesToSend++; return valuesToSend;}
	if(cur_field->metric==FFIELD_MIN) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].min;valuesToSend++; return valuesToSend;}
	if(cur_field->metric==FFIELD_AVG) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].avg;valuesToSend++; return valuesToSend;}	
	if(cur_field->metric==FFIELD_SUM) 
		{ ((uint32_t *)valuesToSend)[0]=stats_sensors[cur_field->sensor].sum;((uint32_t *)valuesToSend)++; return valuesToSend;}	
	if(cur_field->metric==FFIELD_FIRST) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].first;valuesToSend++; return valuesToSend;}
	
	// THE ONLY CASE left is variance
	if( cur_field->metric !=  FFIELD_VARIANCE )
		return valuesToSend;


	if(cur_field->metric==FFIELD_VARIANCE && stats_sensors[cur_field->sensor].isFilledWithVariance==FALSE) 
		visitToCompute(cur_field->sensor, streamp, numberOfSamples, valuesToSend, VISIT_COMPUTE_VARIANCE);		

	if(cur_field->metric==FFIELD_VARIANCE) 
		{ valuesToSend[0]=stats_sensors[cur_field->sensor].variance;valuesToSend++; return valuesToSend;}

	return valuesToSend;

}




uint16_t lastIndex(struct sensor *sensor) {
	int last=sensor->window_pos-1;
	if(last<0) last=MAXWINDOWSIZE;
	return last;
}

uint16_t previousIndex(uint16_t currentIndex) {
	int last=currentIndex-1;
	if(last<0) last=MAXWINDOWSIZE-1;
	return last;
}
	
uint16_t lastValue(struct sensor *sensor) {
	int last=lastIndex(sensor);
	return sensor->window_array[last];
}

/*******************************************/
int gap_index=0;
int sampleGap=0;

int getSampleGap() {
	return sampleGap;
}

void setSampleGap(int i) {
	sampleGap=i;
	if(i<0) sampleGap=0;
}

void initGapIndex() {
	gap_index=sampleGap;
}

void incGap() {
	if(gap_index==sampleGap)
		gap_index=0;
	else
		gap_index++;
}

int isGap() {
	if(USE_SAMPLE_GAP==FALSE)
		return FALSE;	
	if(gap_index==sampleGap)
		return FALSE;
	return TRUE;
}
/***************************************/




void resetStatistics(uint16_t sensorID) {
	stats_sensors[sensorID].isFilled=FALSE;
	stats_sensors[sensorID].isFilledWithVariance=FALSE;
	stats_sensors[sensorID].isFilledWithOrder=FALSE;
	stats_sensors[sensorID].min=MAXINT;
	stats_sensors[sensorID].max=0;
	stats_sensors[sensorID].count=0;
	stats_sensors[sensorID].sum=0;
	stats_sensors[sensorID].avg=0;
	stats_sensors[sensorID].variance=0;
	stats_sensors[sensorID].percentile_index=0;
	stats_sensors[sensorID].first=0;
}


uint16_t * visitToCompute(uint16_t sensorID, struct stream *stream, uint16_t numberOfVisits, uint16_t * valuesToSend, int computeWhat) {
	static uint16_t value=0;	
	static uint16_t index=0;
        

	uint32_t variance=0;
	
	initGapIndex();

	if(computeWhat==VISIT_COMPUTE_STATS || (computeWhat==VISIT_COMPUTE_ORDER_AND_FILL && stats_sensors[sensorID].isFilled==FALSE) )	
		computeStatisticsSTART(sensorID);

	if(computeWhat==VISIT_COMPUTE_VARIANCE)	{
		if( stats_sensors[sensorID].isFilled!=TRUE ) return valuesToSend;
		if( stats_sensors[sensorID].isFilledWithVariance==TRUE ) return valuesToSend;	
	}

	if(computeWhat==VISIT_COMPUTE_ORDER_AND_FILL)	{
		if( stats_sensors[sensorID].isFilledWithPercentile==TRUE ) return valuesToSend;	
	}


	for( index=window_visitor_start(sensorID,numberOfVisits) ; ;  index=window_visitor_nextIndex(index), incGap() )  {
//printf("index: %d\n",index);
		value = window_visitor_value(sensorID, index);

                 

//                if (!check_each_value (value, stream)) continue;



		if(isGap()==TRUE && value!=MAXINT) continue;

		if(value==MAXINT) { // ended visit
                    
			if(computeWhat==VISIT_COMPUTE_STATS || (computeWhat==VISIT_COMPUTE_ORDER_AND_FILL && stats_sensors[sensorID].isFilled==FALSE))
				computeStatisticsEND(sensorID, value);
			if(computeWhat==VISIT_COMPUTE_VARIANCE)
				computeVarianceEND(sensorID, value, variance);	
			break;			
		} else { // one more value
                    
                    
			if(computeWhat==VISIT_COMPUTE_STATS || (computeWhat==VISIT_COMPUTE_ORDER_AND_FILL && stats_sensors[sensorID].isFilled==FALSE)) 
				computeStatisticsSTEP(sensorID, value);
			if(computeWhat==VISIT_COMPUTE_VARIANCE)
				variance=computeVarianceSTEP(sensorID, value, variance);
			if(computeWhat==VISIT_COMPUTE_ORDER_AND_FILL)
				computeOrderSTEP(sensorID, value);
			if(computeWhat==VISIT_COMPUTE_MERGE)
				valuesToSend = computeMergeSTEP(value, valuesToSend);
			if(computeWhat==VISIT_COMPUTE_RLE) {
//				printf("step %d value %u\n",index,value);
				valuesToSend = computeRleSTEP(stream, value, valuesToSend);
			}

		}
		
	}
	return valuesToSend;

}

uint16_t check_each_value (uint16_t value, struct stream *stream){
    uint16_t i;
                for (i=0;i<MAXALARMCONDITIONS;i++){
                    if (stream->where[i].field1.metric == FFIELD_EACH_VALUE){
                        if (stream->where[i].operator == OPERATOR_EQUAL){
                            if (value == stream->where[i].field2.metric)
                                return TRUE;
                            else
                                return FALSE;
			}
                        else if (stream->where[i].operator == OPERATOR_GREATER){
                            if (value > stream->where[i].field2.metric)
                                return TRUE;
                            else
                                return FALSE;
			}
                        else if (stream->where[i].operator == OPERATOR_SMALLER){
                            if (value < stream->where[i].field2.metric)
                                return TRUE;
                            else
                                return FALSE;
			}
                        else if (stream->where[i].operator == OPERATOR_GREATER_OR_EQUAL){
                            if (value >= stream->where[i].field2.metric)
                                return TRUE;
                            else
                                return FALSE;
			}
                       else if (stream->where[i].operator == OPERATOR_SMALLER_OR_EQUAL){
                            if (value >= stream->where[i].field2.metric)
                                return TRUE;
                            else
                                return FALSE;
			}
		    }
		}
return FALSE;
}



static int visitorNumberOfVisits=0;
static int visitorVisits=0;


uint16_t window_visitor_start(uint16_t sensorID,uint16_t numberOfVisits) {
	visitorNumberOfVisits=numberOfVisits;
	visitorVisits=0;

	int visitorCurIndex=lastIndex(aq_getSensorPointer(sensorID));
	return visitorCurIndex; 
}

uint16_t window_visitor_value(uint16_t sensorID, uint16_t curIndex) {	
	if(visitorVisits==visitorNumberOfVisits)
		return MAXINT;
	visitorVisits++;
	return aq_getSensorPointer(sensorID)->window_array[curIndex];
}

uint16_t window_visitor_nextIndex(uint16_t curIndex) {	
	curIndex=previousIndex(curIndex);
	return curIndex;
}

/******************* VISITOR INTERFACES ***********************/

void computeStatisticsSTART(uint16_t sensorID) {
			resetStatistics(sensorID);
}

void computeStatisticsSTEP(uint16_t sensorID, uint16_t value) {

			if( value < stats_sensors[sensorID].min)stats_sensors[sensorID].min=value;
			if( value > stats_sensors[sensorID].max) stats_sensors[sensorID].max=value;
			stats_sensors[sensorID].count++;
			stats_sensors[sensorID].sum+=value;	
			stats_sensors[sensorID].first=value;	// will retain th elast one to go in, which is the first
}

void computeStatisticsEND(uint16_t sensorID, uint16_t value) {
			stats_sensors[sensorID].isFilled=TRUE;
			stats_sensors[sensorID].avg=stats_sensors[sensorID].sum/stats_sensors[sensorID].count;
			stats_sensors[sensorID].variance=0;
			
}
void computeVarianceEND(uint16_t sensorID, uint16_t value, uint32_t variance) {
			stats_sensors[sensorID].isFilledWithVariance=TRUE;
			stats_sensors[sensorID].variance=(uint16_t) (variance/ stats_sensors[sensorID].count );
}

uint32_t computeVarianceSTEP(uint16_t sensorID, uint16_t value, uint32_t variance) {
			variance += (value - stats_sensors[sensorID].avg) * (value - stats_sensors[sensorID].avg);
			return variance;
}

void computeOrderSTEP(uint16_t sensorID, uint16_t value) {
	// insert in ordered fashion
	uint16_t aux=0; uint16_t val=value;
	int i=0;

//printf("chegou value %u temos indice \n",value, stats_sensors[sensorID].percentile_index);
	if(stats_sensors[sensorID].percentile_index==0) {
		stats_sensors[sensorID].percentile_array[0]=val;
		stats_sensors[sensorID].percentile_index++;
		return;
	}
		
	for(i=0;i<stats_sensors[sensorID].percentile_index ;i++) 
	{
//printf("for %d parrayVal %u val %u\n",i,stats_sensors[sensorID].percentile_array[i],val);
		if(val<stats_sensors[sensorID].percentile_array[i]) {
//printf("for if val<parrayVal %d\n",i);
			aux = stats_sensors[sensorID].percentile_array[i];
			stats_sensors[sensorID].percentile_array[i]=val;
			val=aux;

		}
	}
	stats_sensors[sensorID].percentile_array[i]=val;

	stats_sensors[sensorID].percentile_index++;

}


uint16_t * computeMergeSTEP(uint16_t value, uint16_t * valuesToSend) {
PRINTF("valorMERGE %u\n",value);
			valuesToSend[0]=value;
			valuesToSend++;	
			return 	valuesToSend;	
}


uint16_t * computeRleSTEP(struct stream *stream, uint16_t value, uint16_t * valuesToSend) {

// printf("stream->rleIndex %u ",stream->rleIndex);
// printf("stream->rleIndex.count %u\n",stream->rle[stream->rleIndex].count);


	if(currentTimeMilliseconds() - stream->timeStartRle > stream->rleAliveTime) {	// maximum alive tim ehas passed, must send
//uint32_t t = currentTimeMilliseconds() - stream->timeStartRle;
//printf("ct %lu passed %lu tstartrle %lu stream->rleIndex %u \n",currentTimeMilliseconds(), t,stream->timeStartRle,stream->rleIndex);

//printf("alive %u \n",stream->rleAliveTime);

		valuesToSend = sendRle(stream, valuesToSend);

	}

//PRINTF("valorRLE %u\n",value);

	if(stream->rleIndex==NONE) {
		stream->rle[0].value=value;
		stream->rle[0].count=1;
		stream->rleIndex=0;
	}
	else {
		if(
	(stream->isIntervalRel==FALSE && stream->rle[stream->rleIndex].value/stream->rleInterval == value/stream->rleInterval)
	|| 
	(stream->isIntervalRel==TRUE 
			&& stream->rle[stream->rleIndex].value+stream->rleInterval >= value  
			&& stream->rle[stream->rleIndex].value-stream->rleInterval <= value 			
	)
		)
			stream->rle[stream->rleIndex].count++;
		else {
//PRINTF("muda anterior %u valor %u \n", stream->rle[stream->rleIndex].value, value);
			stream->rleIndex++;
			stream->rle[stream->rleIndex].value=value;
			stream->rle[stream->rleIndex].count=1;					
		}

	}

//PRINTF("stream->rleIndex %u\n",stream->rleIndex);
	if(stream->rleIndex>=stream->rleMaxSize-1) // reached size, send rle values
		valuesToSend = sendRle(stream, valuesToSend);
					
	return 	valuesToSend;
}

/******************* VISITOR INTERFACES end ***********************/

uint16_t * sendRle(struct stream *stream, uint16_t * valuesToSend) {
//PRINTF("mandarle\n");

	if(stream->rleIndex==NONE) { // no value is in the window
		valuesToSend[0]=0;
		valuesToSend++;	
		valuesToSend[0]=0;
		valuesToSend++;	
	} else {
		
		int i;	
		for(i=0;i<=stream->rleIndex;i++) {
			valuesToSend[0]=stream->rle[i].value;
			valuesToSend++;	
			valuesToSend[0]=stream->rle[i].count;
			valuesToSend++;		
		}
		
	}

        stream->rleIndex=NONE;

	stream->timeStartRle = currentTimeMilliseconds();

	return valuesToSend;
}

