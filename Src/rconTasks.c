/*
 * rconTasks.c
 *
 *  Created on: 02.11.2019
 *      Author: Kurat
 */

#include "cmsis_os2.h"
#include "lwip.h"
#include "lwip/api.h"

#include "AuvRCON.h"

// memory pool for dynamic allocation of rcon_packet

#define RCON_MEMPOOL_OBJECTS 16

rcon_packet packets[RCON_MEMPOOL_OBJECTS];

typedef struct
{
	rcon_packet* packets;
	int capacity;

	int count;
	int head;
	int tail;
}rcon_cb;

rcon_cb cb;

void cb_init(rcon_cb* cb, rcon_packet* mem, int number_of_elements)
{
	cb->packets = mem;
	cb->capacity = number_of_elements;
	cb->count = 0;
	cb->head = 0;
	cb->tail = 0;
}

rcon_packet* cb_get(rcon_cb* cb)
{
	if(cb->count == cb->capacity)
		return NULL;

	rcon_packet* p = &(cb->packets[cb->head]);
	cb->head++;
	cb->head %= cb->capacity;

	cb->count++;
	return p;
}

rcon_packet* cb_peek(rcon_cb* cb)
{
	if(cb->count == 0)
		return NULL;

	return &(cb->packets[cb->tail]);
}

void cb_free(rcon_cb *cb)
{
	if(cb->count == 0)
		return;

	cb->tail++;
	cb->tail %= cb->capacity;

	cb->count--;
}

osEventFlagsId_t evt_id;



void RconServerTask(void *argument) {
	struct netconn *conn = 0, *newconn;

	err_t err, accept_err;

	struct netbuf *buf;
	uint8_t *data_rx;
	u16_t len;

	rcon_packet *packet;


	osDelay(5000);
	if ((conn = netconn_new(NETCONN_TCP))) {
		if ((err = netconn_bind(conn, NULL, 5005)) == ERR_OK) {
			netconn_listen(conn);

			while (1) {
				accept_err = netconn_accept(conn, &newconn);

				if (accept_err == ERR_OK) {
					while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
						packet = cb_get(&cb);
						do {
							netbuf_data(buf, (void**)&data_rx, &len);
							while (len--) {
								if (rcon_parse_byte(packet, *data_rx++)
										== RCON_PACKET_COMPLETE) {
									osEventFlagsSet(evt_id, 0x01);
								}
							}

						} while (netbuf_next(buf) >= 0);
						netbuf_delete(buf);
					}
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		} else {
			netconn_delete(newconn);
		}

	}

	for(;;)
	{
		osDelay(1000);
	}
}



void RconExecTask(void *argument) {
	rcon_packet *rcon_packet;

	while (1)
	{
		osEventFlagsWait(evt_id, 0x01, osFlagsWaitAny, osWaitForever);
		rcon_packet = cb_peek(&cb);

		if(!rcon_packet)
		{
			if (rcon_packet->type == '1')
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			if (rcon_packet->type == '2')
				HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			cb_free(&cb);
		}
	}
}
