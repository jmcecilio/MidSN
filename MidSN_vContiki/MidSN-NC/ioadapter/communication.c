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
#include "node-id.h"

#include "midsn-nc.h"
#include "drivers/so-driver.h"
#include "nc-ginapp-aa/aquisition.h"
#include "nc-ginapp-gp/events.h"
#include "nc-ginapp-gp/compute.h"
#include "ioadapter/midsn_packet.h"
#include "nc-kernel-am/agent-manager.h"
#include "nc-ginapp-cm/config-manager.h"
#include "ioadapter/communication.h"
#include "nc-ginapp-gp/clockMidSN.h"



/* Set to 1 to activate debug mode */
#define DEBUG 1
#define PRINTVALUES  1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


uint16_t streamID, sensorType, value;

void comm_sendConfigStatus() {

    //    struct midsn_packet pkt;
    //    pkt.msg_type = MSG_CONFIG_STATUS;

    //composeStatus(&cfg);


    //    copy_to_packet(&pkt, &cfg, sizeof (struct msg_config));
    //    send_pkt_to(client_conn, &pkt);

}

void comm_sendWakeUpInfo() {

    struct midsn_packet pkt;
    pkt.msg_type = MSG_HELLO;
    pkt.ctrl_type = MSG_HELLO;
    //    uint32_t timeStamp = currentTimeMilliseconds();

    //    copy_to_packet(&pkt, &timeStamp, sizeof (uint32_t));
    //    send_pkt_to(client_conn, &pkt);
}

void comm_receive_handler(struct midsn_packet * pkt) {


    if ((pkt->msg_type & 0x00FF) == MSG_CONTROL) {
        PRINTF("MSG_CONTROL\n");
        cm_node(pkt);
        return;
    }

    if ((pkt->msg_type & 0x00FF) == MSG_AGENT) {
        PRINTF("MSG_AGENT\n");
        cm_ginapp(pkt);
        return;
    }

    if ((pkt->msg_type & 0x00FF) == MSG_OTHER) {
        PRINTF("MSG_OTHER\n");

        // NOTHING, JUST ECHOES

        return;
    }

    if ((pkt->msg_type & 0x00FF) == MSG_DATA) {
        PRINTF("\nMSG_DATA: ");
        //        PRINTF("MidSN-NC got a packet: ");
        print_pkt_hex(pkt);
        PRINTF("\n");

        return;
    }

}


static uint16_t nodeid;

void set_nodeID(uint16_t addr) {
    nodeid = addr;
}

uint16_t get_nodeID() {
    //    printf("Node ID = %04x\n",nodeid);
    return nodeid;
}

void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer) {

    struct midsn_packet pkt;
    
    char ipNodeAddress [16];
    
    sprintf(&ipNodeAddress[0], "192.168.123.1%u", node_id);
    
    pkt.msg_type = MSG_DATA;
    pkt.ctrl_type = 0;

    sprintf(&pkt.source[0], "%s", ipNodeAddress);
    sprintf(&pkt.dest[0], "%s", streamp->ipTo);
    pkt.destPort = streamp->ipPort;


    uint32_t time = currentTimeMilliseconds();

    copy_to_packet(&pkt, &streamp->streamID, sizeof (streamp->streamID));
    add_to_packet(&pkt, &streamp->nSeq, sizeof (streamp->nSeq));
    add_to_packet(&pkt, &time, sizeof (time));


    add_to_packet(&pkt, valuesToSendArrayStructPointer->valuesPointer, valuesToSendArrayStructPointer->size * 2);


/*
#if PRINTVALUES
    printf("valuesToSend");
    int i = 0;
    for (i = 0; i < valuesToSendArrayStructPointer->size; i++)
        printf(" [%d] = %u;", i, valuesToSendArrayStructPointer->valuesPointer[i]);

    printf("\n");

#endif
*/


    send_pkt(streamp, &pkt);


};

