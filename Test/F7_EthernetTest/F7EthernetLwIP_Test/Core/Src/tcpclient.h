#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#define TCP_CLIENT_PORT 2333


#define DEST_IP_ADDR0	208
#define DEST_IP_ADDR1	68
#define DEST_IP_ADDR2	36
#define DEST_IP_ADDR3	87

//#define DEST_IP_ADDR0	192
//#define DEST_IP_ADDR1	168
//#define DEST_IP_ADDR2	56
//#define DEST_IP_ADDR3	1
//208.68.36.87
//192.168.56.1
#define DEST_PORT		2333
#define LOCAL_PORT		2333

int create_tcp_client(void);
void tcp_client_send(int sock, const char *data);
void tcp_client_recv(int sock);

void TCP_Client_Init(void);
void send_json_data(struct tcp_pcb *tpcb, const char *json_data);
#endif
