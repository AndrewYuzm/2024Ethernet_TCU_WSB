/*
 * Author: 	Andrew Yu
 * Note: 	this contains some helper functions for TCP Ethernet and Internet communications,
 * 			defined ways to collect data, create TCP - socket connections to destination server
 * 			ip address, send data pakeages to server, and receive data from the server.
 * date:	20240604
 *
 * update:
 *
 */


#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_


//#include "main.h"

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



//#define MAX_DATA 10  // 最大数据项数

typedef struct {
    int id;
    char data[32];
    char str[32];  // 存储字符串形式
} DataItem;

//DataItem dataItems[MAX_DATA];


void init_dataItems();
void add_or_update_data(int id, char data[]);
char* get_all_data_str();
char* format_hex(int id, int data);


#endif
