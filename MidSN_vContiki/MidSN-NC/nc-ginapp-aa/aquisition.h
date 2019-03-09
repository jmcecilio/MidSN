/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */
 
#define NUMBER_STREAMS 		2
#define NUMBER_SENSORS 		2
#define MAXFORMATFIELDS 	2
#define MAXALARMCONDITIONS	1
#define MAXWINDOWSIZE 		100
#define MAXINT			30000
#define MAXRLESIZE  		1//MAXWINDOWSIZE/2


#define STREAM_NONE		1000

/*       Aquisition structs     */





struct field {
	uint16_t sensor; // which sensor or NOSENSOR
	uint16_t metric; // 0 to 100 is prcentile, 101-> other metrics
			// if NOSENSOR, this is a value...
};


struct where {
	struct field field1;
	uint16_t operator; // >,<,>=,<=,...
	struct field field2; // value or anorther sensor metric
};

struct rleItem {
	uint16_t value;
	uint16_t count;
};



struct stream {	
	uint16_t streamID;
        uint16_t isIp;  //TRUE = uIP stack; FALSE = rime stack
	uint16_t ipPort;
	char ipTo [10];
        uint16_t rimeTo;

	uint16_t isAlarm;	//=FALSE;	
        uint16_t sense_rate;	//=1000;
	uint16_t send_rate;	//=1000;
	uint16_t window_size;	//=1;

	struct field select[MAXFORMATFIELDS];
	struct where where[MAXALARMCONDITIONS];
	uint16_t nSeq;

	uint16_t reserved[5]; // reserved for future use

	struct rleItem rle[MAXRLESIZE];
	uint16_t rleOn; // =FALSE;
	uint16_t rleIndex;
	uint16_t rleMaxSize;		
	uint16_t rleAliveTime; // maximum time to keep rle values
	uint32_t timeStartRle; // started rle time

	uint32_t last_sendTS; // register of last send time

	uint16_t rleInterval; //não sei se é para acrescentar :D
	uint16_t isIntervalRel; // TRUE rleInterval relative to previous FALSE: relative to absolute
				// previous +- rleInterval VS value /rleInterval
        uint16_t countSensorSamplingRates; //sò é utilizado para a 2a versão do código

        uint16_t last_computeInterval; // only time to compute
        uint16_t last_sendInterval;  // only send command

        uint16_t is_one_time_query;  // indicates if run only one time
};



extern struct stream streams[NUMBER_STREAMS];


/********************** sensors ***********************/
struct sensor {
	uint16_t type;
	uint16_t window_pos;
	uint16_t window_array[MAXWINDOWSIZE];
	uint16_t last_sample_time; // register of last time
        uint16_t nsamples;
        uint16_t last_aquisitionInterval;

};


extern struct sensor sensors[NUMBER_SENSORS];


void initialize_stream(struct stream *stream);

void initialize_streams();
void initialize_sensors();

void aq_computeAndSend(int i);

void aq_placeInSensorWindow(int i,uint16_t value);

void aq_adquire(int i);

//aq_addSensor(int i,struct sensor sensor);
void aq_addSensor(int i,struct sensor *sensor);

void aq_addStream(int i,struct stream *stream);

// struct stream * aq_getStreamPointer ( int i );


int aq_dropSensor(int sensorID) ;

int aq_dropStream(int i) ;

struct sensor * aq_getSensorPointer(int i) ;

struct stream * aq_getStreamPointer(int i);

int aq_SensorPosition(int sensorID);

int aq_StreamPosition(int streamID);

int aq_changeStreamWindowSize(uint16_t streamID, uint16_t value);

int aq_resetNseqStreams(uint16_t streamID);

int aq_resetCountSamples(uint16_t sensorType);

int aq_getCountSamples(uint16_t sensorType);

void aq_set_aquisitionInterval(uint16_t id, uint32_t timeToInterval, uint32_t now);

void aq_set_computeInterval(uint16_t id, uint32_t timeToInterval, uint32_t now);

void aq_set_SendInterval(uint16_t id, uint32_t timeToInterval, uint32_t now);

uint16_t aq_get_aquisitionInterval(uint16_t id);

uint16_t aq_get_computeInterval(uint16_t id);

uint16_t aq_get_SendInterval(uint16_t id);
