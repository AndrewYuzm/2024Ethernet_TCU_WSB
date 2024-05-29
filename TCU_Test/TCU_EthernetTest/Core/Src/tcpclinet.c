#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "tcpclient.h"

#include "lwip/err.h"
#include "lwip/sockets.h"

#include "cmsis_os.h"

#define SERVER_IP "208.68.36.87" // 服务器IP地址
//#define SERVER_IP "192.168.2.19" // 服务器IP地址
//#define SERVER_IP "192.168.2.89" // 服务器IP地址
//#define SERVER_IP "192.168.56.1" // 服务器IP地址
#define SERVER_PORT 2333         // 服务器端口号

int create_tcp_client(void) {
    int sock;
    struct sockaddr_in server_address;

    // 创建socket
    sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
    	printf("soc create error...\n\r");
        return -1; // Socket创建失败
    }

    // 设置服务器地址
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP, &server_address.sin_addr.s_addr);

    // 连接服务器
    if (lwip_connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        lwip_close(sock);
    	printf("connect error...\n\r");
        return -2; // 连接失败
    }

    return sock; // 返回socket描述符
}
void tcp_client_send(int sock, const char *data) {
	err_t err;
    err = lwip_send(sock, data, strlen(data), 0);
	if (err == ERR_OK) {
//		tcp_output(tpcb);
		// 实际发送数据
	} else {
//		printf("memerr..\n\r");
		// 处理错误
		// ERR_MEM 如果发送缓冲区太小不能放下全部数据
		// 其他错误码表示其他问题
	}
}

void tcp_client_recv(int sock) {
    char buffer[512];
    int len = lwip_recv(sock, buffer, sizeof(buffer), 0);
    if (len > 0) {
        // 处理接收到的数据
    }
}


extern struct tcp_pcb *client_pcb;


static void client_err(void *arg, err_t err)       //出现错误时调用这个函数，打印错误信息，并尝试重新连接
{
//  printf("连接错误!!\n\r");
//	printf("尝试重连!!\n\r");

  //连接失败的时候释放TCP控制块的内存
	printf("tcp closed...\n\r");
  tcp_close(client_pcb);


  //重新连接
	printf("try to reconnect...\n\r");
	TCP_Client_Init();

}


static err_t client_send(void *arg, struct tcp_pcb *tpcb)   //发送函数，调用了tcp_write函数
{
  uint8_t send_buf[]= "Hi pc, this is STM32, How u doin????\n\r";

  //发送数据到服务器
  tcp_write(tpcb, send_buf, sizeof(send_buf), 1);

  return ERR_OK;
}

static err_t client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  if (p != NULL)
  {
    /* 接收数据*/
    tcp_recved(tpcb, p->tot_len);

    /* 返回接收到的数据*/
    tcp_write(tpcb, p->payload, p->tot_len, 1);

    memset(p->payload, 0 , p->tot_len);
    pbuf_free(p);
  }
  else if (err == ERR_OK)
  {
    //服务器断开连接
    printf("Server disconnected!\n\r");
    tcp_close(tpcb);

    //重新连接
    TCP_Client_Init();
  }
  return ERR_OK;
}

static err_t client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
  printf("Server connected!\n\r");

  //注册一个周期性回调函数
//  tcp_poll(pcb,client_send,2);

  //注册一个接收函数
//  tcp_recv(pcb,client_recv);

  return ERR_OK;
}

void TCP_Client_Init(void)
{
	//	struct tcp_pcb *client_pcb = NULL;   //这一句一定要放在里面，否则会没用
	client_pcb = NULL;   //这一句一定要放在里面，否则会没用
	ip4_addr_t server_ip;     //因为客户端要主动去连接服务器，所以要知道服务器的IP地址
	err_t err;
	/* 创建一个TCP控制块  */
	client_pcb = tcp_new();

	IP4_ADDR(&server_ip, DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);//合并IP地址

	//  printf("客户端开始连接!\n\r");
	printf("StartConn...\n\r");
	//开始连接
	err=tcp_connect(client_pcb, &server_ip, TCP_CLIENT_PORT, client_connected);
	if(err!=ERR_OK)
		printf("connErr!err=%d\n\r",err);
	err=ip_set_option(client_pcb, SOF_KEEPALIVE);
	printf("tcpconnCalled...\n\r");
	//	printf("已经调用了tcp_connect函数\n\r");

	//注册异常处理
	tcp_err(client_pcb, client_err);
	tcp_nagle_disable(client_pcb);
	//	printf("已经注册异常处理函数\n\r");
	printf("errFunRegisted.\n\r");
}
// 调用此函数来发送JSON数据
int err_times=0;
void send_json_data(struct tcp_pcb *tpcb, const char *json_data) {
    err_t err;

    // 将数据放入发送缓冲区
    err = tcp_write(tpcb, json_data, strlen(json_data), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    if (err == ERR_OK) {
    	err_times=0;
        // 实际发送数据
    } else {
    	printf("%d merr,err=%d..\n\r",err_times,err);
    	err_times++;
        // 处理错误
        // ERR_MEM 如果发送缓冲区太小不能放下全部数据
        // 其他错误码表示其他问题
    }
}

////collect data

#define MAX_DATA 30  // 最大数据项数
//typedef struct {
//    int id;
//    int data;
//    char str[12];  // 存储字符串形式
//} DataItem;
//
DataItem dataItems[MAX_DATA];
SemaphoreHandle_t dataMutex;

//初始化存储空间
void init_dataItems() {
//    dataMutex = xSemaphoreCreateMutex();
    for (int i = 0; i < MAX_DATA; i++) {
        dataItems[i].id = -1;  // 使用-1表示该位置为空
        dataItems[i].data = 0;
        memset(dataItems[i].str, 0, sizeof(dataItems[i].str));
    }
//    xSemaphoreGive(dataMutex);
}

//产生新数据
void add_or_update_data(int id, int data) {

//    xSemaphoreTake(dataMutex, portMAX_DELAY);
    int emptyIndex = -1;
//printf("running..\n");
    for (int i = 0; i < MAX_DATA; i++) {
        if (dataItems[i].id == id) {  // 找到相同ID，更新数据
            dataItems[i].data = data;
            sprintf(dataItems[i].str, ",0x%08X%016X\n", id, data);
//            xSemaphoreGive(dataMutex);
            return;
        }
        if (dataItems[i].id == -1 && emptyIndex == -1) {  // 记录第一个空位
            emptyIndex = i;
        }
    }

    if (emptyIndex != -1) {  // 有空位，添加新数据
        dataItems[emptyIndex].id = id;
        dataItems[emptyIndex].data = data;
        sprintf(dataItems[emptyIndex].str, ",0x%08X%016X\n", id, data);
    }

//    xSemaphoreGive(dataMutex);
}

char* get_all_data_str() {
    static char allDataStr[MAX_DATA * 29];  // 需要足够大的空间来存储所有字符串
//    xSemaphoreTake(dataMutex, portMAX_DELAY);
    strcpy(allDataStr, "");  // 初始化字符串

    for (int i = 0; i < MAX_DATA; i++) {
        if (dataItems[i].id != -1) {
            strcat(allDataStr, dataItems[i].str);
        }
    }
//    strcat(allDataStr, '\n');
//    printf(allDataStr);
//    xSemaphoreGive(dataMutex);
    init_dataItems();
    return allDataStr;
}

// update all CAN signals state to server
void update_CanLib_To_Server(){

}


// Function to convert id and data into a formatted hex string
// General C
char* format_hex(int id, int data) {
    // Allocate memory for the resulting string
    static char result[29]; // Enough for ",0xAAAABBBB\0"
    // Format the id and data into the result string
    sprintf(result, ",0x%08X%016X", id, data);
    return result;
}
//example:
//char* formatted_string = format_hex(id, data);
//printf("%s\n", formatted_string);
