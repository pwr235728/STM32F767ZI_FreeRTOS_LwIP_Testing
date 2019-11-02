/*
 * AuvRCON.h
 *
 *  Created on: 02.11.2019
 *      Author: Kurat
 */

#ifndef AUVRCON_H_
#define AUVRCON_H_

#include "stdint.h"

#define RCON_HEADER_SIZE 5		// ID 2 bytes, Type 1 byte, Size 2 bytes
#define RCON_PAYLOAD_SIZE 513	// BODY 512 bytes, END 1 byte
#define RCON_PACKET_SIZE 518

typedef enum
{
	RCON_PACKET_INVALID,
	RCON_PACKET_COMPLETE,
	RCON_PACKET_INCOMPLETE
}rcon_state;

typedef enum
{
	_RCON_DATA_RESET,
	_RCON_DATA_INVALID,
	_RCON_DATA_HEADER_RX,
	_RCON_DATA_GOT_HEADER,
	_RCON_DATA_PAYLOAD_RX,
	_RCON_DATA_GOT_PAYLOAD
}_rcon_data_state;

typedef struct
{
	uint8_t data[RCON_PACKET_SIZE];
	uint32_t index;
	_rcon_data_state state;
}_rcon_data;

typedef struct
{
	int16_t id;
	int8_t type;
	uint16_t size;
	uint8_t* body;

	_rcon_data _rcon_data;
}rcon_packet;

void rcon_packet_reset(rcon_packet* packet);

rcon_state rcon_parse_byte(rcon_packet* packet, uint8_t data);


#endif /* AUVRCON_H_ */
