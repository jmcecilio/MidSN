/* 
 * File:   so-driver.h
 * Author: cecilio
 *
 * Created on 2 de Agosto de 2012, 10:15
 */

#ifndef SO_DRIVER_H
#define	SO_DRIVER_H

struct so_timer {
    struct timer timer;
    struct etimer *next;
    struct process *p;
};


#define SENSOR_NOTHING			0
#define SENSOR_TEMPERATURE 		1
#define SENSOR_HUMIDITY 		2
#define SENSOR_BATTERY			3
#define SENSOR_BATTERY_INDICATOR	4
#define SENSOR_LIGHT 			5
#define SENSOR_LIGHT2			6
#define SENSOR_ADC0			7
#define SENSOR_ADC1			8
#define SENSOR_ADC2			9
#define SENSOR_ADC3			10
#define SENSOR_ADC4			11
#define SENSOR_ADC5			12
#define SENSOR_ADC6			13
#define SENSOR_NONE             	10000


void set_so_timer(struct so_timer * timer, uint32_t timeIntervalInMIPs);

void stop_so_timer(struct so_timer * timer);

void reset_so_timer(struct so_timer * timer);

int so_timer_expired(struct so_timer * timer);





void set_delayedStart_so_timer(uint32_t timeIntervalInMIPs);

int delayedStart_so_timer_expired();

int sensorTimer_so_timer_expired();

void reset_sensorTimer_so_timer();




uint16_t adquire(int sensorID);

#endif	/* SO_DRIVER_H */

