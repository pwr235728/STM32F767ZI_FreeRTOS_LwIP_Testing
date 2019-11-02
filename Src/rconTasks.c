/*
 * rconTasks.c
 *
 *  Created on: 02.11.2019
 *      Author: Kurat
 */

#include "cmsis_os.h"
#include "lwip.h"
#include "lwip/api.h"

#include "AuvRCON.h"

extern osMessageQueueId_t rconCmdQueueHandle;

void RconServerTask(void *argument) {

	struct netconn *conn = 0, *newconn;

	err_t err, accept_err;

	struct netbuf *buf;
	uint8_t *data_rx;
	u16_t len;

	rcon_packet packet;
	rcon_packet_reset(&packet);

	osDelay(5000);
	if ((conn = netconn_new(NETCONN_TCP))) {
		if ((err = netconn_bind(conn, NULL, 5005)) == ERR_OK) {
			netconn_listen(conn);

			while (1) {
				accept_err = netconn_accept(conn, &newconn);

				if (accept_err == ERR_OK) {
					while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
						do {
							netbuf_data(buf, &data_rx, &len);
							while (len--) {
								if (rcon_parse_byte(&packet, *data_rx++)
										== RCON_PACKET_COMPLETE) {
									osMessageQueuePut(rconCmdQueueHandle,  &packet, 0, 0);
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



extern void RconServerExec(void *argument)
{
	uint32_t msg_count;
	uint8_t msg_prio;
	rcon_packet rcon_packet;

	while(1)
	{
		if((msg_count = osMessageQueueGetCount(rconCmdQueueHandle)))
		{
			if(osMessageQueueGet(rconCmdQueueHandle, &rcon_packet, &msg_prio, osWaitForever) == osOK)
			{
				if(rcon_packet.type == '1')
						HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				if(rcon_packet.type == '2')
						HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			}
		}
		osDelay(1);
	}
}
