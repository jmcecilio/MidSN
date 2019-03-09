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

#include "communication.h"
#include "sensor_code.h"
#include "isnap_packet.h"
#include "config_status.h"
#include "aquisition.h"
#include "compute.h"
#include "clockIsnap.h"
#include "events.h"

/*---------------------------------------------------------------------------*/
//#define PRINT6HOSTPORT(conn) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x %5d", ((u8_t *)((conn)->ripaddr))[0], ((u8_t *)((conn)->ripaddr))[1], ((u8_t *)((conn)->ripaddr))[2], ((u8_t *)((conn)->ripaddr))[3], ((u8_t *)((conn)->ripaddr))[4], ((u8_t *)((conn)->ripaddr))[5], ((u8_t *)((conn)->ripaddr))[6], ((u8_t *)((conn)->ripaddr))[7], ((u8_t *)((conn)->ripaddr))[8], ((u8_t *)((conn)->ripaddr))[9], ((u8_t *)((conn)->ripaddr))[10], ((u8_t *)((conn)->ripaddr))[11], ((u8_t *)((conn)->ripaddr))[12], ((u8_t *)((conn)->ripaddr))[13], ((u8_t *)((conn)->ripaddr))[14], ((u8_t *)((conn)->ripaddr))[15], ((u16_t *)((conn)->rport)))

/* Set to 1 to activate debug mode */
#define DEBUG 0
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif



static struct msg_config cfg;

uint16_t streamID, sensorType, value;

void comm_sendConfigStatus() {

    struct isnap_packet pkt;
    pkt.msg_type = MSG_CONFIG_STATUS;

    composeStatus(&cfg);


//    copy_to_packet(&pkt, &cfg, sizeof (struct msg_config));
//    send_pkt_to(client_conn, &pkt);

}

void comm_sendWakeUpInfo() {

    struct isnap_packet pkt;
    pkt.msg_type = MSG_HELLO;
    pkt.ctrl_type = MSG_HELLO;
    uint32_t timeStamp = currentTimeMilliseconds();

//    copy_to_packet(&pkt, &timeStamp, sizeof (uint32_t));
//    send_pkt_to(client_conn, &pkt);
}

void comm_receive_handler(void) {
    
    struct isnap_packet pkt;


    if (!uip_newdata()) return;
    get_packet(&pkt);
    //    PRINTF("Rcv %d: '%s' from ", uip_datalen(), (char *) uip_appdata);
    //    PRINT6ADDR(&UDP_IP_BUF->srcipaddr);
    PRINTF("MSGTYPE (pkt.msg_type & 0x00FF) %u %04x \n", (pkt.msg_type & 0x00FF), (pkt.msg_type & 0x00FF));
    //    PRINT6HOSTPORT(client_conn);
    //    PRINTF("\n");
    //    PRINTF("Pkt %d: %d '%s\n'" , pkt.msg_type, pkt.ctrl_type, pkt.payload);
    //    print_pkt_hex(&pkt);

    if ((pkt.msg_type & 0x00FF) == MSG_CONTROL) {

        //     printf("Control type %u" ,pkt.ctrl_type);
        switch (pkt.ctrl_type) {
            case CTRL_START:
                PRINTF("CTRL_START \n ");
                // <VALUE>
                if (pkt.size >= 2) {
                    // SET <TIMER>
                    // <TIMER> = get_uint16_t(&pkt, 0);
                }
                send_ack_to_sender(server_conn, &pkt);
                send_ack_to(client_conn, &pkt);
                break;

            case CTRL_STOP:
                PRINTF("CTRL_STOP \n");
                //                 isrunning = 0;
                send_ack_to_sender(server_conn, &pkt);
                send_ack_to(client_conn, &pkt);

                ev_initialize_events();

                watchdog_reboot();
                break;

            case CTRL_REBOOT:
                PRINTF("CTRL_REBOOT \n");
                //                 isrunning = 0;
                send_ack_to_sender(server_conn, &pkt);
                send_ack_to(client_conn, &pkt);
                watchdog_reboot();
                break;

            case CTRL_SEND_CONFIG_STATUS:
                PRINTF("CTRL_SEND_CONFIG_STATUS\n");
                comm_sendConfigStatus();
                break;

            case CTRL_SET_CONFIG_STATUS:
                PRINTF("CTRL_SET_CONFIG_STATUS\n");
                if (pkt.size < 4) break;
                uint32_t timeInterval = ((uint32_t *) pkt.payload.u8t_val)[0];

                set_config_status_etimer(timeInterval);
                send_ack_to_sender(server_conn, &pkt);
                break;

            case CTRL_PING:
                PRINTF("CTRL_PING\n");
                send_ack_to_sender(server_conn, &pkt);
                break;

            case CTRL_ONE_TIME_QUERY:
                PRINTF("CTRL_ONE_TIME_QUERY\n");




                

/* está mal só envia lixo, foi só para experimentar o tdmr*/
                
                uint16_t data[5];
                data[0]= MSG_DATA;
                data[1]= currentTimeMilliseconds();
                data[3]= 1;
                data[4]= 1000;
        
                copy_to_packet(&pkt, &data, sizeof(data));
                reply_pkt_to(server_conn, &pkt);
/* Fim */
                break;


                // SENSOR CONTROL MESSAGES
            case CTRL_NEW_SENSOR:
                PRINTF("\nCTRL_NEW_SENSOR\n");
                struct sensor sr;

                uint16_t sampling_rate = 1000;

                sr.type = SENSOR_NOTHING;
                sr.window_pos = 0;
                sr.nsamples = 0;

                memcpy(&sr, &(pkt.payload), pkt.size);

                ev_dropSensorAndEvent(sr.type);
                ev_addSensorAndEvent(&sr, sampling_rate);

                send_ack_to_sender(server_conn, &pkt);
                break;


            case CTRL_RESTART_SENSOR:
                PRINTF("CTRL_RESTART_SENSOR \n ");

                if (pkt.size < 2) break;
                sensorType = PKT_UINT16T[0];

                aq_resetCountSamples(sensorType);

                send_ack_to_sender(server_conn, &pkt);

                break;


            case CTRL_SET_SENSOR_SRATE:
                PRINTF("CTRL_SET_SENSOR_SRATE \n");
                if (pkt.size < 4) break;

                sensorType = PKT_UINT16T[0];
                value = PKT_UINT16T[1];

                ev_changeSensorSampleRate(sensorType, value);

//                set_delayedStart_etimer(WAIT_TIME);

                send_ack_to_sender(server_conn, &pkt);
                break;

                // STREAM CONTROL MESSAGES


            case CTRL_NEW_STREAM:
                // <STRUCT STREAM>
                PRINTF("\nCTRL_NEW_STREAM\n");
                struct stream st;
                initialize_stream(&st);
                memcpy(&st, &(pkt.payload), pkt.size);

                if (aq_StreamPosition(st.streamID) != -1)
                    ev_dropStreamAndEvent(aq_StreamPosition(st.streamID));
                
                ev_addStreamAndEvent(&st, st.send_rate);

                send_ack_to_sender(server_conn, &pkt);
                break;
            case CTRL_RESTART_STREAM:
                PRINTF("CTRL_RESTART_STREAM \n ");

                if (pkt.size < 4) break; //perg ao João Pedro pq 4
                streamID = PKT_UINT16T[0];

                aq_resetNseqStreams(streamID);

                send_ack_to_sender(server_conn, &pkt);

                break;

            case CTRL_SET_STREAM_SRATE:
                PRINTF("CTRL_SET_STREAM_SRATE \n");
                if (pkt.size < 4) break;
                streamID = PKT_UINT16T[0];
                value = PKT_UINT16T[1];

                if (getAllEventsAreRunningSetRestartIfStarts()){
                    ev_stop_all_events();
                    ev_changeStreamSampleRate(streamID, value);
                    send_ack_to_sender(server_conn, &pkt);
                    set_delayedStart_etimer((uint32_t)WAIT_TIME);
                }
           
                break;

            case CTRL_SET_WINDOW_SIZE:
                PRINTF("CTRL_SET_WINDOW_SIZE \n");
                if (pkt.size < 4) break;
                streamID = PKT_UINT16T[0];
                value = PKT_UINT16T[1];

                aq_changeStreamWindowSize(streamID, value);

                send_ack_to_sender(server_conn, &pkt);
                break;

                // SENDTO
            case CTRL_SEND_CONN:
                PRINTF("MSG_CONTROL_SEND_CONN \n");
                // <NONE>
                PRINT6ADDR(&client_conn->ripaddr);
                PRINTF(" Port =%u \n", HTONS(client_conn->rport));
                copy_to_packet(&pkt, &client_conn->ripaddr, sizeof (client_conn->ripaddr));
                add_to_packet(&pkt, &client_conn->rport, sizeof (client_conn->rport));

                send_ack_to_sender(server_conn, &pkt);
                send_ack_to(client_conn, &pkt);

                //                send_pkt_to(server_conn, &pkt);
                //                send_pkt_to(client_conn, &pkt);
                break;

            case CTRL_SEND_TO:
                PRINTF("CTRL_SEND_TO \n");
                // <IPv6ADDRESS><PORT>
                uint16_t sendPort;
                //                PRINTF("pkt.size %u >= sizeof (uip_ipaddr_t) %d\n", pkt.size, sizeof (uip_ipaddr_t));
                /* New connection with remote host */
                if (pkt.size >= sizeof (uip_ipaddr_t)) {
                    uip_ipaddr_t ipaddr;
                    //                    PRINTF("REDIRECT TO ?");
                    memcpy(&ipaddr, &(pkt.payload), sizeof (ipaddr));
                    PRINT6ADDR(&ipaddr);
                    if (pkt.size >= sizeof (uip_ipaddr_t)) {
                        //                        sendPort = get_uint16_t(&pkt, 8);
                        sendPort = PKT_UINT16T[8];
                        //                        sendPort = UINT16POSi(&pkt, 8);
                        //                        PRINTF(" Port =%u \n", sendPort);
                        client_conn = udp_new(&ipaddr, HTONS(sendPort), NULL);
                        //                        PRINTF("REDIRECTING TO ");
                        //                        memcpy(&client_conn->ripaddr, &(pkt.payload), sizeof (ipaddr));
                        //                        memcpy(&client_conn->ripaddr, &ipaddr, sizeof (ipaddr));
                        //                        PRINT6ADDR(&client_conn->ripaddr);
                        client_conn = udp_new(&ipaddr, HTONS(sendPort), NULL);
                        send_pkt_to(server_conn, &pkt);
                    }
                }//
                break;

            default:
                PRINTF("UNKNOWN CONTROL_TYPE\n");
                send_ack_to_sender(server_conn, &pkt);
                break;


        }
        reset_server_conn(server_conn);
        return;
    }

    if (pkt.msg_type == MSG_OTHER) {
        PRINTF("MSG_OTHER\n");

        // NOTHING, JUST ECHOES

        reset_server_conn(server_conn);
        return;
    }

    if (pkt.msg_type == MSG_CONFIG_STATUS) {
        PRINTF("\nAMSG_CONFIG_STATUS\n");
        // REDIRECT to SINK
        send_pkt_to(client_conn, &pkt);
        reset_server_conn(server_conn);
        return;
    }
    if (pkt.msg_type == MSG_DATA) {
        PRINTF("\nAMSG_DATA\n");
        // REDIRECT to SINK ?
        send_pkt_to(client_conn, &pkt);
        // ADD to the aggregation Window
        // add_to_window();
        reset_server_conn(server_conn);
        return;
    }


    //    PRINTF("\nOTHER UNKNOWN PACKET. Sending 4to client\n");
    send_pkt_to(client_conn, &pkt);

    reset_server_conn(server_conn);


}


static uint16_t nodeid;
static uint16_t nodeidh;

void comm_print_local_addresses(void) {
    int i;
    uip_netif_state state;

    /* Print IPv6 addresses */
    PRINTF("Client IPv6 addresses: ");
    for (i = 0; i < UIP_CONF_NETIF_MAX_ADDRESSES; i++) {
        state = uip_netif_physical_if.addresses[i].state;
        if (state == TENTATIVE || state == PREFERRED) {
            PRINT6ADDR(&uip_netif_physical_if.addresses[i].ipaddr);
            PRINTF("\n");
        }
    }

}

void set_nodeId(uip_ipaddr_t * addr) {
    //    printf("\n\n set nodeID = %02x%02x\n\n", ((uint8_t *)addr)[14],((uint8_t *)addr)[15]);
    nodeid = (((uint8_t *) addr)[15] & 0x00ff) | ((((uint8_t *) addr)[14] << 8) & 0xff00);

    nodeidh = (((uint8_t *) addr)[13] & 0x00ff) | ((((uint8_t *) addr)[12] << 8) & 0xff00);

}

void set_local_nodeid() {
    int i;
    uip_netif_state state;



    // Print IPv6 addresses
    for (i = 0; i < UIP_CONF_NETIF_MAX_ADDRESSES; i++) {
        state = uip_netif_physical_if.addresses[i].state;
        if (state == TENTATIVE || state == PREFERRED) {
            set_nodeId(&uip_netif_physical_if.addresses[i].ipaddr);
        }
    }

}

void comm_set_global_address(void) {
    /* Router solicitation and IP configuration */
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0, 0, 0, 0, 0, 0, 0, 0);
    uip_netif_addr_autoconf_set(&ipaddr, &uip_lladdr);
    uip_netif_addr_add(&ipaddr, 16, 0, TENTATIVE);

    //    PRINT6ADDR(&ipaddr);
}

uint16_t getNodeID() {
    //    printf("Node ID = %04x\n",nodeid);
    return nodeid;
}

uint16_t getNodeIDHigh() {
    //    printf("Node ID = %04x\n",nodeidh);
    return nodeidh;
}

void comm_set_connection_address(uip_ipaddr_t *ipaddr) {
    /* Destination IP */
    uip_ip6addr(ipaddr, 0x3ffe, 0x502, 0xffff, 0x0100, 0x7600, 0x13ff, 0xfe7c, 0x48);
    //	uip_ip6addr(ipaddr,0xfe80, 0x0000, 0x0000, 0x0000, 0x020c, 0x29ff, 0xfe1b, 0xb291);
}

void comm_config_connection(struct uip_udp_conn *server_con, struct uip_udp_conn *client_con, uip_ipaddr_t *ipaddr, int portToSend, int portToReceive) {

    //void comm_config_connection(uip_ipaddr_t *ipaddr, int portToSend, int portToReceive) {

    /* Obtain IPv6 using router solicitation */
#if UIP_CONF_ROUTER
    comm_set_global_address();
#endif

    /* Print addresses for debug purposes */
    comm_print_local_addresses();

    /* set nodeid */
    set_local_nodeid();

    /* Set destination address */
    comm_set_connection_address(ipaddr);

    /* New connection with remote host */
    client_con = udp_new(ipaddr, HTONS(portToSend), NULL);
//tcp_connect(&ip6addr, HTONS(7), NULL);

    server_con = udp_new(NULL, HTONS(0), NULL);
    udp_bind(server_con, HTONS(portToReceive));

    client_conn = client_con;
    server_conn = server_con;
    //    PRINT6ADDR(&server_conn->ripaddr);
    //    PRINTF(" %u\n", HTONS(server_conn->rport));

}

void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer) {

    struct isnap_packet pkt;
    pkt.msg_type = MSG_DATA;

    uint32_t time = currentTimeMilliseconds();

    copy_to_packet(&pkt, &streamp->streamID, sizeof (streamp->streamID));
    add_to_packet(&pkt, &streamp->nSeq, sizeof (streamp->nSeq));
    add_to_packet(&pkt, &time, sizeof (time));
    //    printf("\nTIME %lu (size=%u)" , time, sizeof (time));

    add_to_packet(&pkt, valuesToSendArrayStructPointer->valuesPointer, valuesToSendArrayStructPointer->size * 2);
    //        add_to_packet(&pkt,valuesToSendArrayStructPointer->valuesPointer,8*2);

    send_pkt_to(client_conn, &pkt);
    print_pkt_hex(&pkt);




};

