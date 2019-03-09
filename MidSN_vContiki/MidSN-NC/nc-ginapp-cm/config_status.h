void statusToBuffer();


//PROCESS_NAME(config_status_sensor);
//-----------data struct-----------//
//extern struct msg_config;

struct msg_config
{
//	uint16_t rssi;
	uint32_t cpu;
	uint32_t lpm;
	uint32_t transmit;
	uint32_t listen;
//	uint16_t best_neighbor;
//	uint16_t best_neighbor_etx;
//	uint16_t best_neighbor_rtmetric;
//	uint16_t battery_voltage;
//  	uint16_t battery_indicator;
  	uint16_t tx, rx;
//  	uint16_t reliabletx, reliablerx, rexmit, acktx, noacktx, ackrx;
        uint16_t timedout, badackrx;
  /* Reasons for dropping incoming packets: */
  	uint16_t toolong, tooshort, badsynch, badcrc;
  	uint16_t contentiondrop, /* Packet dropped due to contention */
    sendingdrop; /* Packet dropped when we were sending a packet */
  	uint16_t lltx, llrx;
};

void composeStatus(struct msg_config *msg_config) ;
void set_config_status_etimer(uint32_t timeIntervalInMillis) ;

void reset_config_status_etimer();
int config_status_etimer_expired();

