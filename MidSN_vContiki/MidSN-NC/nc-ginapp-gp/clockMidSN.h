/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 * TelosB UDP Client
 */
 

extern uint32_t clockCounter;
extern uint32_t clockLastTicks;

uint32_t currentTimeMilliseconds(void);
uint32_t currentTimeSeconds(void);

void clockMidSNInit();
PROCESS_NAME(clockMidSN_process);
