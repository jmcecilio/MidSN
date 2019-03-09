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
#include <stdint.h>

#include "midsn-nc.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-aa/aquisition.h"
#include "nc-ginapp-gp/events.h"
#include "nc-ginapp-gp/compute.h"
#include "ioadapter/midsn_packet.h"
#include "ioadapter/communication.h"
#include "nc-ginapp-gp/clockMidSN.h"
#include "drivers/communication_protocol.h"

/* Set to 1 to activate debug mode */
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UINT16POSi(packet,  pos) ((packet)->payload.u16t_val)[pos]

uint16_t streamID, sensorType, value;

void cm_ginapp(struct midsn_packet * pkt) {

    if ((pkt->msg_type & 0x00FF) == MSG_AGENT) {

        //     printf("Control type %u" ,pkt.ctrl_type);
        switch (pkt->ctrl_type) {
            case CTRL_GP_NEW_STREAM:
                // <STRUCT STREAM>
                PRINTF("\nCTRL_NEW_STREAM: ");
                struct stream st;
                initialize_stream(&st);
                memcpy(&st, &(pkt->payload), pkt->size);
                PRINTF("%u\n", st.streamID);
                if (aq_StreamPosition(st.streamID) != -1)
                    ev_dropStreamAndEvent(aq_StreamPosition(st.streamID));
                ev_addStreamAndEvent(&st, st.send_rate);
                static struct field select_field;
                int i = 0;
                for (i = 0; i < MAXFORMATFIELDS; i++) {
                    select_field = st.select[i];
                    if (select_field.sensor == FFIELD_sensor_END)
                        break;
                    if (select_field.sensor < FFIELD_sensor_NODEID || select_field.sensor > FFIELD_sensor_LAST_SEND_INTERVAL) {                       
                        struct sensor sensor;
                        sensor.nsamples = 0;
                        sensor.type = select_field.sensor;
                        sensor.window_pos = 0;
                        ev_addSensorAndEvent(&sensor, st.sense_rate);
                    }
                }
                break;

            case CTRL_GP_DROP_STREAM:
                PRINTF("CTRL_GP_DROP_STREAM \n");
                if (pkt->size < 2) break;
                streamID = UINT16POSi(pkt, 0);
                ev_dropStreamAndEvent(aq_StreamPosition(streamID));
                break;

            case CTRL_GP_RESTART_STREAM:
                PRINTF("CTRL_GP_RESTART_STREAM \n");
                if (pkt->size < 2) break;
                streamID = UINT16POSi(pkt, 0);
                aq_resetNseqStreams(streamID);
                break;




            case CTRL_GP_SET_STREAM_SENDRATE:
                PRINTF("CTRL_GP_SET_STREAM_SENDRATE \n");
                if (pkt->size < 4) break;
                streamID = UINT16POSi(pkt, 0);
                value = UINT16POSi(pkt, 1);

                if (getAllEventsAreRunningSetRestartIfStarts()){
                    ev_stop_all_events();
                    ev_changeStreamSampleRate(streamID, value);
                    set_delayedStart_so_timer((uint32_t)WAIT_TIME);
                }

                break;

            case CTRL_GP_SET_STREAM_SENSERATE:
                PRINTF("CTRL_GP_SET_STREAM_SENSERATE\n");
                if (pkt->size < 4) break;
                sensorType = UINT16POSi(pkt, 0);
                value = UINT16POSi(pkt, 1);
                ev_changeSensorSampleRate(sensorType, value);
                break;


            case CTRL_GP_SET_WINDOW_SIZE:
                PRINTF("\nCTRL_GP_SET_WINDOW_SIZE\n");
                if (pkt->size < 4) break;
                streamID = UINT16POSi(pkt, 0);
                value = UINT16POSi(pkt, 1);
                aq_changeStreamWindowSize(streamID, value);
                break;


            case CTRL_GP_ONE_TIME_QUERY:
                PRINTF("CTRL_RESTART_SENSOR \n ");
           

/* está mal só envia lixo, foi só para experimentar o tdmr*/
                
                uint16_t data[5];
                data[0]= MSG_DATA;
                data[1]= currentTimeMilliseconds();
                data[3]= 1;
                data[4]= 1000;
        
                copy_to_packet(pkt, &data, sizeof(data));
                
/*                //------------------> POR AQUI O ENDEREÇO

                send_to(server_conn, &pkt);
*/
                
/* Fim */

                break;


            case CTRL_GP_SEND_TO:
                PRINTF("CTRL_GP_SEND_TO \n");
/*
                if (pkt->size < 4) break;

                streamID = UINT16POSi(pkt, 0);
                value = UINT16POSi(pkt, 1);
                ev_changeStreamWindowSize(streamID, value);
*/
                break;



            default:
                PRINTF("UNKNOWN CONTROL_TYPE\n");
                break;
        }

        return;
    }


}
