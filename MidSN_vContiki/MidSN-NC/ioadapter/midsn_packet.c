/**
 * José Cecílio
 *
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
#include "nc-ginapp-gp/clockMidSN.h"
#include "ioadapter/midsn_packet.h"
#include "ioadapter/communication.h"
#include "drivers/communication_protocol.h"

/* Set to 1 to activate debug mode */
#define DEBUG 1
#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void copy_to_packet(struct midsn_packet *packet, void * pointer, int length) {
    packet->size = 0;
    copy_to_packet_pos(packet, pointer, 0, length);
}

void copy_to_packet_pos(struct midsn_packet *packet, void * pointer, int pos, int length) {
    char *p = (char *) &packet->payload;
    p = p + pos;
    memcpy(p, pointer, length);
    packet->size = packet->size + length;
}

void add_to_packet(struct midsn_packet *packet, void * pointer, int length) {
    copy_to_packet_pos(packet, pointer, packet->size, length);
}

void print_pkt_hex(struct midsn_packet *packet) {


    int j;
    PRINTF("Pkt type=%u %04x ctrl=%u %04x nseq=%u size=%u: ", packet->msg_type, packet->msg_type, packet->ctrl_type, packet->ctrl_type, packet->nseq, packet->size);
    for (j = 0; j < packet->size; j++) {
        PRINTF("%02X ", packet->payload.char_val[j]);
        if ((j + 1) % 20 == 0) PRINTF("\n");
    }

    PRINTF("\n");

}

static int nseq = 1;

void send_pkt(struct stream * streamp, struct midsn_packet * packet) {
    int totalsize = (int) (packet->size + MIDSN_PKT_HEADER_SIZE);
    
    packet->nseq = nseq++;
    if (totalsize <= MIDSN_PKT_MAX_PAYLOAD_PER_PACKET) {
        send_to(streamp, packet);
        return;
    }

    // Else we need to fragment
    int freepayload = MIDSN_PKT_MAX_PAYLOAD_PER_PACKET - MIDSN_PKT_HEADER_SIZE;
    int nfullfragments = packet->size / freepayload;
    int nfragments = nfullfragments;
    int last = packet->size % freepayload;
    if (last > 0) nfragments++;

    int i = 0;
    for (i = 1; i <= nfullfragments; i++) {
        //        PRINTF("PKT_TYPE %u frag %d / %d ", packet->msg_type, i, nfragments);
        //        PRINTF("(nfragments & 0x000F) %d (i & 0x000F) %d", (nfragments & 0x000F), (i & 0x000F));
        packet->msg_type = (packet->msg_type & 0x00FF) | ((((i & 0x000F) << 4) | (nfragments & 0x000F)) << 8);
        //        PRINTF(" PKT_TYPE %u \n", packet->msg_type);
        packet->size = MIDSN_PKT_MAX_PAYLOAD_PER_PACKET;
        //        uip_udp_packet_send(client_conn, packet, MIDSN_PKT_MAX_PAYLOAD_PER_PACKET);
        send_to(streamp, packet);
        /*
                print_pkt_hex(packet);
         */
        char *p = (char*) & packet->payload;
        memcpy(packet->payload.char_val, (p + freepayload), freepayload);
    }
    if (last > 0) {
        packet->msg_type = (packet->msg_type & 0x00FF) | ((((nfragments & 0x000F) << 4) | (nfragments & 0x000F)) << 8);
        packet->size = last;
        //        uip_udp_packet_send(client_conn, packet, last);
        send_to(streamp, packet);
        /*
                print_pkt_hex(packet);
         */
    }

}

