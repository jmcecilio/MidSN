/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */
 

// FORMAT FIELDS 0-99 ARE PERCENTILES, 100 IS VALUE, THEN IT IS STATS
#define FFIELD_VALUE	101 // current (last) sensor value
#define FFIELD_MAX	102 // 
#define FFIELD_MIN	103
#define FFIELD_AVG	104
#define FFIELD_COUNT	105 // no actual count, only expected number of samples
#define FFIELD_COUNTITERATIONS	106 // actual count from iterations
#define FFIELD_SUM	107
#define FFIELD_VARIANCE	108
#define FFIELD_EACH_VALUE	109 // each sensor value

#define NONE MAXINT

#define FFIELD_MERGE	110
#define FFIELD_RLE	111

#define FFIELD_LAST	200
#define FFIELD_FIRST	201


#define FFIELD_sensor_NODEID	20
#define FFIELD_sensor_NODEIDHIGH 21
#define FFIELD_sensor_VALUE	22
#define FFIELD_sensor_END	23
#define FFIELD_sensor_NOW	24
#define FFIELD_sensor_LASTSENDINTERVAL	25
#define FFIELD_sensor_LASTSENDTS 26
#define FFIELD_sensor_STREAMID	27
#define FFIELD_sensor_NSEQSTREAM 28
#define FFIELD_sensor_NSEQSENSOR 29

#define FFIELD_LAST_AQUISITION_INTERVAL        30
#define FFIELD_sensor_LAST_COMPUTE_INTERVAL    31
#define FFIELD_sensor_LAST_SEND_INTERVAL       32

#define VISIT_COMPUTE_STATS		1
#define VISIT_COMPUTE_VARIANCE		2
#define VISIT_COMPUTE_ORDER_AND_FILL	3
#define VISIT_COMPUTE_MERGE		4
#define VISIT_COMPUTE_RLE		5


#define OPERATOR_EQUAL              0
#define OPERATOR_GREATER            1
#define OPERATOR_SMALLER            2
#define OPERATOR_GREATER_OR_EQUAL   3
#define OPERATOR_SMALLER_OR_EQUAL   4


#define MAX_SELECT_DATA_SIZE 100

struct valuesToSendArrayStruct {
	uint16_t * valuesPointer;
	uint16_t size;
};



struct statistics {
	uint16_t isFilled; //=FALSE;
	uint16_t isFilledWithVariance; // =FALSE;
	uint16_t isFilledWithPercentile;
	uint16_t isFilledWithOrder; // =FALSE;
	uint16_t min;
	uint16_t max;
	uint16_t count;
	uint32_t sum;
	uint16_t avg;
	uint16_t variance;
	uint16_t percentile_array[MAXWINDOWSIZE];
	uint16_t percentile_index;
	uint16_t first;
};

extern struct statistics stats_sensors[NUMBER_SENSORS];


struct valuesToSendArrayStruct * comp_compute(struct event * eventp, struct stream * streamp);

uint16_t * compute(struct field *cur_field, uint16_t *valuesToSend, struct event * eventp, struct stream * streamp);

uint16_t lastIndex(struct sensor *sensor);

uint16_t previousIndex(uint16_t currentIndex);

uint16_t lastValue(struct sensor *sensor);

int getSampleGap();
void setSampleGap(int i);
void initGapIndex();
void incGap();

int isGap();


void computeStatisticsSTART(uint16_t sensorID);
void resetStatistics(uint16_t sensorID);



uint16_t * visitToCompute(uint16_t sensorID, struct stream *stream, uint16_t numberOfVisits, uint16_t * valuesToSend, int computeWhat);

uint16_t window_visitor_start(uint16_t sensorID,uint16_t numberOfVisits);

uint16_t window_visitor_value(uint16_t sensorID, uint16_t curIndex);

uint16_t window_visitor_nextIndex(uint16_t curIndex);

void computeStatisticsSTEP(uint16_t sensorID, uint16_t value);

void computeStatisticsEND(uint16_t sensorID, uint16_t value); 

void computeVarianceEND(uint16_t sensorID, uint16_t value, uint32_t variance);

uint32_t computeVarianceSTEP(uint16_t sensorID, uint16_t value, uint32_t variance);

void computeOrderSTEP(uint16_t sensorID, uint16_t value);

uint16_t * computeMergeSTEP(uint16_t value, uint16_t * valuesToSend);

uint16_t * computeRleSTEP(struct stream *stream, uint16_t value, uint16_t * valuesToSend);

uint16_t * sendRle(struct stream *stream, uint16_t * valuesToSend);

void init_statistics();
uint16_t compute_where_fields(struct event * eventp, struct stream * streamp);
uint16_t check_each_value (uint16_t value, struct stream *stream);

