/*
 * AuvRCON.c
 *
 *  Created on: 02.11.2019
 *      Author: Kurat
 */


#include "AuvRCON.h"


void rcon_packet_reset(rcon_packet* packet)
{
	packet->_rcon_data.index = 0;
	packet->_rcon_data.state = _RCON_DATA_RESET;
	packet->id = 0;
	packet->type = 0;
	packet->size = 0;
	packet->body = &(packet->_rcon_data.data[RCON_HEADER_SIZE]);
}

rcon_state rcon_parse_byte(rcon_packet* packet, uint8_t data)
{
	_rcon_data* rd = &(packet->_rcon_data);

	switch (rd->state) {
	case _RCON_DATA_RESET:
	{	}
	/* no break */
	case _RCON_DATA_INVALID:
	{
		rd->index = 0;
		rd->state = _RCON_DATA_HEADER_RX;
	}
	/* no break */

	case _RCON_DATA_HEADER_RX:
	{
		if (rd->index < RCON_HEADER_SIZE) {
			rd->data[rd->index++] = data;
		}
		if (rd->index == RCON_HEADER_SIZE) {
			rd->state = _RCON_DATA_GOT_HEADER;
		}
	}
		break;

	case _RCON_DATA_GOT_HEADER:
	{
		packet->id = rd->data[0] + (rd->data[1] << 8);
		packet->type = rd->data[2];
		packet->size = rd->data[3] + (rd->data[4] << 8);
		packet->body = &(rd->data[RCON_HEADER_SIZE]);
		rd->index = 0;
		rd->state = _RCON_DATA_PAYLOAD_RX;
	}
	/* no break */
	case _RCON_DATA_PAYLOAD_RX:
	{
		if (rd->index < packet->size + 1) {
			rd->data[RCON_HEADER_SIZE + rd->index++] = data;
		}
		if (rd->index == packet->size+1) {
			rd->state = _RCON_DATA_GOT_PAYLOAD;
		}
	}
		break;

	default: {
		/* some bananas happend */
	}
		break;
	}

	if (rd->state == _RCON_DATA_GOT_PAYLOAD) {
		if (packet->body[packet->size - 1] == '\0'
				&& packet->body[packet->size] == '\0') {
			rd->state = _RCON_DATA_RESET;
			return RCON_PACKET_COMPLETE;
		} else {
			rd->state = _RCON_DATA_INVALID;
			return RCON_PACKET_INVALID;

		}
	}

	return RCON_PACKET_INCOMPLETE;
}
