

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "contiki.h"
#include "net/rime/mesh.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "dev/sht11.h"
#include "net/rime/timesynch.h"
#include "net/rime/neighbor.h"
#include "dev/battery-sensor.h"

#include "dev/cc2420.h"


#include "config_status.h"
#include "events.h"


//static unsigned long last_cpu=0, last_lpm=0, last_transmit=0, last_listen=0;
//static uint16_t data_seq=0;

/*

#define NUM_SAMPLES 4
struct spectrum {
  int channel[16];
};


static struct spectrum rssi_samples[NUM_SAMPLES];

#define MAX(a, b) ((a) > (b)? (a): (b))
#define MIN(a, b) ((a) < (b)? (a): (b))

 */
//-------END:data struct-----------//

static struct etimer config_status_timer;

void set_config_status_etimer(uint32_t timeIntervalInMillis) {
    etimer_set(&config_status_timer, ev_samplingTicks(timeIntervalInMillis));
}

void reset_config_status_etimer() {
    etimer_reset(&config_status_timer);
}

int config_status_etimer_expired() {
    return etimer_expired(&config_status_timer);
}

void composeStatus(struct msg_config *msg_config) {
    /*
    //	uint16_t rssi;
            uint16_t cpu;
            uint16_t lpm;
            uint16_t transmit;
            uint16_t listen;
     */
    //	struct neighbor *n = NULL;

    //	static uint16_t data_seq=0;

    //	struct msg_config msg_config;

    energest_flush();


    //		rssi = do_rssi();
    /*		cpu = energest_type_time(ENERGEST_TYPE_CPU) - last_cpu;
                    lpm = energest_type_time(ENERGEST_TYPE_LPM) - last_lpm;
                    transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT) - last_transmit;
                    listen = energest_type_time(ENERGEST_TYPE_LISTEN) - last_listen;

                    last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
                    last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
                    last_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
                    last_listen = energest_type_time(ENERGEST_TYPE_LISTEN);

     */
    //                msg_config->rssi = rssi;

    msg_config->cpu = energest_type_time(ENERGEST_TYPE_CPU);
    msg_config->lpm = energest_type_time(ENERGEST_TYPE_LPM);
    msg_config->transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
    msg_config->listen = energest_type_time(ENERGEST_TYPE_LISTEN);
    /*
                    msg_config->best_neighbor = 0;
                    msg_config->best_neighbor_etx = 0;
                    msg_config->best_neighbor_rtmetric = 0;

                    n = neighbor_best();
                    if(n != NULL) {
     *((char *)(&msg_config->best_neighbor)) = n->addr.u8[0];
     *(((char *)(&msg_config->best_neighbor))+1) = n->addr.u8[1];
                            msg_config->best_neighbor_etx = neighbor_etx(n);
                            msg_config->best_neighbor_rtmetric = n->rtmetric;
                    }
     */
    //		msg_config->battery_voltage = battery_sensor.value(0);
    //  		msg_config->battery_indicator = sht11_sreg() & 0x40? 1: 0;
    msg_config->tx = rimestats.tx;
    msg_config->rx = rimestats.rx;
    //		msg_config->reliabletx = rimestats.reliabletx;
    //      		msg_config->reliablerx = rimestats.reliablerx;
    //      		msg_config->rexmit = rimestats.rexmit;
    //      		msg_config->acktx = rimestats.acktx;
    //      		msg_config->noacktx = rimestats.noacktx;
    //      		msg_config->ackrx = rimestats.ackrx;
    msg_config->timedout = rimestats.timedout;
    msg_config->badackrx = rimestats.badackrx;
    msg_config->toolong = rimestats.toolong;
    msg_config->tooshort = rimestats.tooshort;
    msg_config->badsynch = rimestats.badsynch;
    msg_config->badcrc = rimestats.badcrc;
    msg_config->contentiondrop = rimestats.contentiondrop;
    msg_config->sendingdrop = rimestats.sendingdrop;
    msg_config->lltx = rimestats.lltx;
    msg_config->llrx = rimestats.llrx;

    //	return msg_config;

}

void resetRimeStats() {

    rimestats.tx = 0;
    rimestats.rx = 0;
    rimestats.reliabletx = 0;
    rimestats.reliablerx = 0;
    rimestats.rexmit = 0;
    rimestats.acktx = 0;
    rimestats.noacktx = 0;
    rimestats.ackrx = 0;
    rimestats.timedout = 0;
    rimestats.badackrx = 0;
    rimestats.toolong = 0;
    rimestats.tooshort = 0;
    rimestats.badsynch = 0;
    rimestats.badcrc = 0;
    rimestats.contentiondrop = 0;
    rimestats.sendingdrop = 0;
    rimestats.lltx = 0;
    rimestats.llrx = 0;
}
//-------------------------GLOBAL_VARIABLES-------------------------//



//---------------------END:RECEIVE_FUNCTION-------------------------//


/*
 int do_rssi(void)
{
  static int sample;
  int channel;
  
  rime_mac->off(0);

  cc2420_on();
  for(channel = 11; channel <= 26; ++channel) {
    cc2420_set_channel(channel);
    rssi_samples[sample].channel[channel - 11] = cc2420_rssi() + 53;
  }
  
  rime_mac->on();
  
  sample = (sample + 1) % NUM_SAMPLES;

  {
    int channel, tot;
    tot = 0;
    for(channel = 0; channel < 16; ++channel) {
      int max = -256;
      int i;
      for(i = 0; i < NUM_SAMPLES; ++i) {
        max = MAX(max, rssi_samples[i].channel[channel]);
      }
      tot += max / 20;
    }
    return tot;
  }
}
 */

