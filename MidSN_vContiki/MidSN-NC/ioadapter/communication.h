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

void comm_receive_handler(struct midsn_packet * pkt);
void set_nodeID(uint16_t  addr);
uint16_t get_nodeID();

void comm_send_values(struct stream * streamp, struct valuesToSendArrayStruct * valuesToSendArrayStructPointer);

