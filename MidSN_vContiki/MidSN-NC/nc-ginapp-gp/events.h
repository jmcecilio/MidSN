


struct event {
	uint16_t sample_rate;
	uint16_t isThing; //eventNONE eventSTREAM eventSENSOR
	struct so_timer et;
};

extern struct so_timer et;
extern uint16_t sensorSamplingRate;



#define eventNONE 0
#define eventSTREAM 2
#define eventSENSOR 1

#define WAIT_TIME (uint32_t)60000

void setAllEventsAreRunning(int i);

int getAllEventsAreRunning();

int getAllEventsAreRunningSetRestartIfStarts();

int delayedStart_etimer_expired();

void ev_restart_all_events();

void ev_stop_all_events();

void ev_stop_all_event(struct event * eventsPointer, int size);

void ev_restart_all_event(struct event * eventsPointer, int size);


int ev_handleEventNoThrills();

int ev_addEvent(struct event * eventsPointer, int size, int eventType, uint16_t sampling);



void ev_initialize_event(struct event * eventsPointer, int size);



int ev_handleEvent(struct event * eventsPointer, int size, int type);


uint32_t ev_samplingTicks(uint32_t sampling_rate) ;

void ev_printEvent(struct event event) ;

/**********************************************************/

uint16_t ev_getSensorEventSampleRate(int sensorID) ;

int ev_addEvents(int eventType, uint16_t sampling);



int ev_addSensorAndEvent(struct sensor *sensor, uint16_t sampling_rate);

int ev_addStreamAndEvent(struct stream *stream, uint16_t sampling_rate) ;


int ev_dropSensorAndEvent(int id) ;

int ev_dropStreamAndEvent(int id);

void ev_dropEventSensor(int i) ;
void ev_dropEventStream(int i);

void ev_initialize_events() ;
void ev_initializeAll();

//void ev_sendValues(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);


void ev_handleEvents();
/**********************************/




int ev_changeSensorSampleRate(uint16_t sensorType, uint16_t value);

int ev_changeStreamSampleRate(uint16_t streamID, uint16_t value);

int ev_changeStreamWindowSize(uint16_t streamID, uint16_t value);





