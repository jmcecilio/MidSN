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

/* Set to 1 to activate debug mode */
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define UINT16POSi(packet,  pos) ((packet)->payload.u16t_val)[pos]
#define PKT_UINT16T  pkt.payload.u16t_val

uint16_t streamID, sensorType, value;


void cm_node(struct midsn_packet * pkt) {
    
    if ((pkt->msg_type & 0x00FF) == MSG_CONTROL) {

        //     printf("Control type %u" ,pkt.ctrl_type);
        switch (pkt->ctrl_type) {
            case CTRL_NODE_START_AGENT:
                value = UINT16POSi(pkt, 0);
                PRINTF("CTRL_START AGENT: agent number %u\n ", value);
                break;
                
                case CTRL_NODE_STOP_AGENT:
                value = UINT16POSi(pkt, 0);
                PRINTF("CTRL_NODE_STOP_AGENT: agent number %u\n ", value);
                break;
                
                case CTRL_NODE_LOAD_AGENT:
                value = UINT16POSi(pkt, 0);
                PRINTF("CTRL_NODE_LOAD_AGENT: agent number %u\n ", value);
                break;
                
                case CTRL_NODE_UNLOAD_AGENT:
                value = UINT16POSi(pkt, 0);
                PRINTF("CTRL_NODE_UNLOAD_AGENT: agent number %u\n ", value);
                break;
                
                case CTRL_NODE_DELETE_AGENT:
                value = UINT16POSi(pkt, 0);
                PRINTF("CTRL_NODE_DELETE_AGENT: agent number %u\n ", value);
                break;
                
                case CTRL_NODE_NEW_AGENT:
                PRINTF("NEW_AGENT NODE\n ");
                break;
                
                case CTRL_NODE_PING:
                PRINTF("CTRL_PING NODE\n ");
                break;
                
                case CTRL_NODE_REBOOT:
                PRINTF("CTRL_REBOOT NODE\n ");
                break;
                
        }
    }

    
}


