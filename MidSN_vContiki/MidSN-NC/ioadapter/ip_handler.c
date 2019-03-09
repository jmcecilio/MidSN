
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

/* Temperature and Humidity*/
//#include "dev/sht11.h"
/* Light */
//#include "dev/light.h"
/* Battery */
//#include "dev/battery-sensor.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "isnap_packet.h"


/* Set to 1 to activate debug mode */
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINT6ADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((u8_t *)addr)[0], ((u8_t *)addr)[1], ((u8_t *)addr)[2], ((u8_t *)addr)[3], ((u8_t *)addr)[4], ((u8_t *)addr)[5], ((u8_t *)addr)[6], ((u8_t *)addr)[7], ((u8_t *)addr)[8], ((u8_t *)addr)[9], ((u8_t *)addr)[10], ((u8_t *)addr)[11], ((u8_t *)addr)[12], ((u8_t *)addr)[13], ((u8_t *)addr)[14], ((u8_t *)addr)[15])
#define PRINTLLADDR(lladdr) PRINTF(" %02x:%02x:%02x:%02x:%02x:%02x ",(lladdr)->addr[0], (lladdr)->addr[1], (lladdr)->addr[2], (lladdr)->addr[3],(lladdr)->addr[4], (lladdr)->addr[5])
#else
#define PRINTF(...)
#define PRINT6ADDR(addr)
#define PRINTLLADDR(addr)
#endif


void tcpip_handler(struct isnap_packet *packet) {
    //   static int seq_id;
    PRINTF("TCPIP HANDLER\n");

//   struct isnap_packet pkt;

    // Debugging
    /*
        //    printf("uip_len()=%d\n",uip_len());

        printf("uip_initialmss()=%d\n",uip_initialmss());

        printf("uip_mss()=%d\n",uip_mss());

        printf("SICSLOWPAN_CONF_FRAG=%d, SICSLOWPAN_REASS_MAXAGE = %d\n" , SICSLOWPAN_CONF_FRAG, SICSLOWPAN_REASS_MAXAGE);
     */

    if (!uip_newdata()) return;
    get_packet(packet);
    PRINTF("Rcv %d: '%s' from ", uip_datalen(), (char *) uip_appdata);
//    PRINT6ADDR(&UDP_IP_BUF->srcipaddr);
    PRINTF("\n");
    //    PRINTF("Pkt %d: %d '%s\n'" , pkt.msg_type, pkt.ctrl_type, pkt.payload);
    print_pkt_hex(packet);

    // if MSG control
    if (packet->msg_type == MSG_CONTROL) {
  //      PRINTF("MSG_CONTROL\n");

        switch (packet->ctrl_type) {

                // if a start control
            case CTRL_START:
                PRINTF("MSG_CONTROL_START ");
                if (packet->size > 0) {
                    time = get_uint16_t(packet, 0);
                    setRuntime(time);
                    etimer_set(&samplingtimer, sampling_rate);
//                    PRINTF(" TIME = %u\n", time);
                } else
                    etimer_restart(&samplingtimer);
                isrunning = 1;
                break;


            case CTRL_STOP:
                PRINTF("MSG_CONTROL_STOP \n");
                isrunning = 0;
                break;

            case CTRL_SET_SAMPLING_RATE:
 //               PRINTF("MSG_CONTROL_SET_SAMPLING_RATE ");
                if (packet->size > 0) {
                    //                sampling_rate = UINT16(uip_appdata, 4);
                    sampling_rate = get_uint16_t(packet, 0);
                    PRINTF(" sampling_rate=%u \n", sampling_rate);
                    etimer_set(&samplingtimer, sampling_rate);
                }
                break;

            case CTRL_SET_WINDOW_SIZE:
   //             PRINTF("MSG_CONTROL_SET_WINDOW SIZE ");
                if (packet->size > 0) {
                    window_size = get_uint16_t(packet, 0);
     //               PRINTF(" Window_size=%u \n", window_size);

                }
                break;

            case CTRL_SEND_TO:
                PRINTF("MSG_SEND_TO ");
                uint16_t sendPort;
                /* New connection with remote host */
                if (packet->size > 0) {
                    sendPort = get_uint16_t(packet, 16);
  //                  PRINTF(" Window_size=%u \n", window_size);

                }
       //         client_conn = udp_new(ipaddr, HTONS(sendPort), NULL);
                break;

        }
//        PRINTF("\nAFTER SWITCH ");
        packet->msg_type = MSG_ACK;
  //      PRINTF("\nAFTER set type = ack ");
        print_pkt_hex(packet);
    //    PRINTF("\nAFTER PRINT");
        send_pkt_to(client_conn, packet);
      //  PRINTF("\nAFTER SEND");
        //        reply_pkt_to(server_conn, pkt);
        //        reset_server_conn();
        memset(&server_conn->ripaddr, 0, sizeof (server_conn->ripaddr));
        server_conn->rport = 0;
    //    reset_server_conn(server_conn);
/*
        memset(&server_conn->ripaddr, 0, sizeof (server_conn->ripaddr));
        server_conn->rport = 0;
*/
  //      PRINTF("\nAFTER RESET");

        return;
    }
    if (packet->msg_type == MSG_OTHER) {
             PRINTF("\nAMSG_OTHER\n");
        memset(&server_conn->ripaddr, 0, sizeof (server_conn->ripaddr));
        server_conn->rport = 0;

        //   struct isnap_packet pkt1;
        int g = 0;
        packet->size = 0;
        for (g = 1; g < ISNAP_PKT_MAX_PAYLOAD_LEN-10; g++) {
            packet->payload[g] = g;
            packet->size++;
        }
        print_pkt_hex(packet);
        send_pkt_to(client_conn, packet);
        //        send_pkt_to(client_conn, pkt1);
        PRINTF("\nAFTER RESET\n");
        return;
    }

    if (packet->msg_type == MSG_CONFIG_STATUS) {
        packet->msg_type = MSG_ACK;
        /*
                struct msg_config cfg;
                cfg = composeStatus();
                memcpy(&pkt.payload, &cfg, sizeof (cfg));
                send_pkt_to(client_conn, pkt);
                reply_pkt_to(server_conn, pkt);
                reset_server_conn();
         */
        return;
    }


    if (packet->msg_type == MSG_REQUEST_STREAMDATA) {
  /*
        struct stream stream;

        printf("Expected=%d  got%d \n", sizeof (stream), pkt.size);
        if (sizeof (struct stream) <= pkt.size) {
            memcpy(&stream, &(pkt.payload), sizeof (struct stream));
            //        stream = (struct stream *) &(pkt.payload);

            printf("Stream ID = %d\n", stream.streamID);
        } else
            printf("Too Short: expected=%d  got%d \n", sizeof (stream), pkt.size);
*/


    }

    uip_ipaddr_copy(&server_conn->ripaddr, &UDP_IP_BUF->srcipaddr);
    server_conn->rport = UDP_IP_BUF->srcport;
    PRINTF("Responding with message: ");
    /*
        sprintf(buff, "Hello from the server! (%d)", ++seq_id);
        PRINTF("%s\n", buff);

        uip_udp_packet_send(server_conn, buff, strlen(buff));
     */

 //   reset_server_conn(server_conn);

}

