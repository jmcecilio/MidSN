


/* Data max length */
#define MIDSN_PKT_MAX_PAYLOAD_LEN		100
#define MIDSN_PKT_MAX_U8T_PAYLOAD_LEN		MIDSN_PKT_MAX_PAYLOAD_LEN
#define MIDSN_PKT_MAX_U16T_PAYLOAD_LEN		MIDSN_PKT_MAX_PAYLOAD_LEN / 2

#define MIDSN_PKT_HEADER_SIZE                   44  //(5 fields * 2 bytes each)

#define MIDSN_PKT_MAX_PAYLOAD_PER_PACKET        100


 union values {
    char        char_val[MIDSN_PKT_MAX_PAYLOAD_LEN];
    uint8_t     u8t_val [MIDSN_PKT_MAX_U8T_PAYLOAD_LEN];
    uint16_t    u16t_val[MIDSN_PKT_MAX_U16T_PAYLOAD_LEN];
    };


struct midsn_packet {
    char source[16];
    char dest[16];
    uint16_t destPort;
    uint16_t msg_type;
    uint16_t ctrl_type;
    uint16_t agent;
    uint16_t nseq;
    int size;
    union values payload;
};


// Message Type

#define MSG_CONTROL             0x0001 // Bytes that indicate the
#define MSG_ACK                 0x0002 // Bytes that indicate type of message
#define MSG_DONE                0x0003 // Bytes that indicate type of message
#define MSG_AGENT               0x0004 // Bytes that indicate type of message
#define MSG_OTHER               0x0005 // Bytes that indicate type of message
#define MSG_DATA                0x0006 // Bytes that indicate type of message
#define MSG_HELLO               0x0007 // Bytes that indicate reboot


// CTRL FOR MSG_CONTROL
// Control Start Format
#define CTRL_NODE_START_AGENT           0x0011 // Starts executions // 0x00 0x01 STREAMID SAMPLING DURATION
#define CTRL_NODE_STOP_AGENT            0x0012 // Starts executions
#define CTRL_NODE_LOAD_AGENT            0x0013
#define CTRL_NODE_UNLOAD_AGENT          0x0014
#define CTRL_NODE_DELETE_AGENT          0x0015
#define CTRL_NODE_NEW_AGENT             0x0016
#define CTRL_NODE_REBOOT                0x0017 // Reboot
#define CTRL_NODE_PING                  0x0018 


// STREAM CONTROL
#define CTRL_GP_NEW_STREAM              0x0021 // Bytes that indicate type of message
#define CTRL_GP_DROP_STREAM             0x0022 // Bytes that indicate type of message
#define CTRL_GP_RESTART_STREAM          0x0023
#define CTRL_GP_SET_STREAM_SENDRATE     0x0024 // Change Sampling rate // 0x0001 0x0003 SamplingRate
#define CTRL_GP_SET_STREAM_SENSERATE    0x0025 // Change Sampling rate // 0x0001 0x0003 SamplingRate
#define CTRL_GP_SET_WINDOW_SIZE         0x0026 // Change Window Size // 0x0001 0x0004 WindowSize
#define CTRL_GP_ONE_TIME_QUERY          0x0027
#define CTRL_GP_SEND_TO                 0x0028 // Change Window Size // 0x0001 0x0004 WindowSize




void copy_to_packet(struct midsn_packet *packet, void * pointer, int length);
void copy_to_packet_pos(struct midsn_packet *packet, void * pointer, int pos, int length);

void add_to_packet(struct midsn_packet *packet, void * pointer, int length);


void print_pkt_hex(struct midsn_packet *packet);

void send_pkt(struct stream * streamp, struct midsn_packet * packet);

