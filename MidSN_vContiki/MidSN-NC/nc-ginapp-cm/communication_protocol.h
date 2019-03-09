/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */
//#define PRINT6HOSTPORT(((conn)->ripaddr)) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x %5d", ((u8_t *)((conn)->ripaddr))[0], ((u8_t *)((conn)->ripaddr))[1], ((u8_t *)((conn)->ripaddr))[2], ((u8_t *)((conn)->ripaddr))[3], ((u8_t *)((conn)->ripaddr))[4], ((u8_t *)((conn)->ripaddr))[5], ((u8_t *)((conn)->ripaddr))[6], ((u8_t *)((conn)->ripaddr))[7], ((u8_t *)((conn)->ripaddr))[8], ((u8_t *)((conn)->ripaddr))[9], ((u8_t *)((conn)->ripaddr))[10], ((u8_t *)((conn)->ripaddr))[11], ((u8_t *)((conn)->ripaddr))[12], ((u8_t *)((conn)->ripaddr))[13], ((u8_t *)((conn)->ripaddr))[14], ((u8_t *)((conn)->ripaddr))[15], ((u16_t *)((conn)->rport)))

extern uint16_t nodeid;
extern uint16_t nodeidh;

/*---------------------------------------------------------------------------*/
void comm_sendConfigStatus();
void comm_sendWakeUpInfo();
void comm_tcpip_handler(void);
//void comm_tcpip_handler(struct uip_udp_conn *server_conn,struct uip_udp_conn *client_conn) ;

void comm_timeout_handler(void);

void comm_print_local_addresses(void);

void comm_set_global_address(void);

void comm_set_connection_address(uip_ipaddr_t *ipaddr);

void comm_config_connection(struct uip_udp_conn *server_conn,struct uip_udp_conn *client_conn,uip_ipaddr_t *ipaddr, int portToSend, int portToReceive);

uint16_t getNodeID();
uint16_t getNodeIDHigh();
//void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);

//void comm_config_connection(uip_ipaddr_t *ipaddr, int portToSend, int portToReceive);

//void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);
//void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);



// void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);

