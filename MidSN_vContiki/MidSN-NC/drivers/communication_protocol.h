/**
 * José Cecílio
 *
 * jcecilio@dei.uc.pt
 *
 */
void init_protocol();
void send_to(struct stream * streamp, struct midsn_packet * packet);
struct midsn_packet * midsn_get_receive_packet();

