#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"


#include <stdio.h>
#include <string.h>
#include "dev/light-sensor.h"
#include "dev/sht11-sensor.h"
#include "drivers/so-driver.h"


static struct so_timer delayedStart_timer;
static struct so_timer sensorTimer;




void set_so_timer(struct so_timer * timer, uint32_t timeIntervalInMIPs) {
    etimer_set((struct etimer * ) timer, timeIntervalInMIPs);
}

void stop_so_timer(struct so_timer * timer) {
    etimer_stop((struct etimer * ) timer);
}

void reset_so_timer(struct so_timer * timer) {
    etimer_reset((struct etimer * ) timer);
}

int so_timer_expired(struct so_timer * timer) {
    return etimer_expired((struct etimer * ) timer);
}




struct etimer * delayedStart_timerp = &delayedStart_timer;
struct etimer * sensorTimerp = &sensorTimer;


void set_delayedStart_so_timer(uint32_t timeIntervalInMIPs) {
    etimer_set(delayedStart_timerp, timeIntervalInMIPs);
}

int delayedStart_so_timer_expired() {
    return etimer_expired(delayedStart_timerp);
}

int sensorTimer_so_timer_expired() {
    return etimer_expired(sensorTimerp);
}

void reset_sensorTimer_so_timer() {
    etimer_reset(sensorTimerp);
}



uint16_t adquire(int sensorID) {

    SENSORS_ACTIVATE(light_sensor);
    SENSORS_ACTIVATE(sht11_sensor);

    if (sensorID == SENSOR_TEMPERATURE)
        return sht11_sensor.value(SHT11_SENSOR_TEMP);
    if (sensorID == SENSOR_HUMIDITY)
        return sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
    if (sensorID == SENSOR_LIGHT)
        return light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC); //light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR);
    if (sensorID == SENSOR_NOTHING)
        return 0;
    if (sensorID == SENSOR_LIGHT2)
        return light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR); //light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC);
    
    SENSORS_DEACTIVATE(light_sensor);
    SENSORS_DEACTIVATE(sht11_sensor);
    return 0;
}
