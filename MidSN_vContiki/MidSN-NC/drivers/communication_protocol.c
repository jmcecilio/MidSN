/**
 * José Cecílio
 *
 *
 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include "dev/leds.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../midsn-nc.h"
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


static u16_t msg_source;
static u16_t msg_dest;
static u16_t msg_hops = 0;
static u16_t msg_len;

struct midsn_packet packet;

static void packet_recv(struct abc_conn *c) {
    if (msgWaiting == 0) {
        rimeaddr_copy((rimeaddr_t *) & msg_dest, packetbuf_addr(PACKETBUF_ADDR_RECEIVER));
        rimeaddr_copy((rimeaddr_t *) & msg_source, packetbuf_addr(PACKETBUF_ADDR_SENDER));
        msg_hops = packetbuf_attr(PACKETBUF_ATTR_HOPS);
        msg_len = packetbuf_copyto(&packet);

        msgWaiting = 1;

    } else
        printf("MidSN-NC: Packet Dropped\n");

    process_poll(&midsn);
}

//Set up ABC
static const struct abc_callbacks abc_call = {packet_recv};
static struct abc_conn abc;

void init_protocol() {
    abc_open(&abc, 128, &abc_call);
}

void send_to(struct stream * streamp, struct midsn_packet * packet) {

/*
    printf("Packet to be sent contains: ");
    print_pkt_hex(packet);
*/

    packetbuf_clear();
    packetbuf_copyfrom((u8_t *) packet, (packet->size + MIDSN_PKT_HEADER_SIZE));

    uint16_t addr = (uint16_t) streamp->rimeTo;

    packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &addr);

    leds_on(LEDS_GREEN);
    abc_send(&abc);
    leds_off(LEDS_GREEN);

    /*
        if (abc_send(&abc))
            printf(" %u bytes sent\n",packet->size + MIDSN_PKT_HEADER_SIZE);
        else
            printf("Msg sending fail!\n");
     */


}

struct midsn_packet * midsn_get_receive_packet() {
    return &packet;
}
