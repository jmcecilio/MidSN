/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */

extern uint16_t nodeid;

/*---------------------------------------------------------------------------*/
void comm_sendConfigStatus();
void comm_sendWakeUpInfo();

void comm_receive_handler(void);

void comm_timeout_handler(void);

void comm_print_local_addresses(void);

void comm_set_global_address(void);

uint16_t getNodeID();

void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);



/*


void comm_set_connection_address(uip_ipaddr_t *ipaddr);

void comm_config_connection(struct uip_udp_conn *server_conn,struct uip_udp_conn *client_conn,uip_ipaddr_t *ipaddr, int portToSend, int portToReceive);


//void comm_config_connection(uip_ipaddr_t *ipaddr, int portToSend, int portToReceive);

//void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);
//void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);



// void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);

*/


