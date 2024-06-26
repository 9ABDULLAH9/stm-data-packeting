#include "data_packet.h"
#include "main.h"
#include "crc.h"
#include "spi.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"


void floatToUint8(float value, uint8_t *buffer, uint8_t i)
{
    FloatUInt8Union unionVal;
    unionVal.f = value;

    buffer[i] = unionVal.u[0];
    buffer[i+1] = unionVal.u[1];
	buffer[i+2] = unionVal.u[2];
    buffer[i+3] = unionVal.u[3];
}

float uInt8ToFloat(float* value, uint8_t *buffer, uint8_t i)
{
    FloatUInt8Union unionVal;

    unionVal.u[0] = buffer[i];
    unionVal.u[1] = buffer[i+1];
	unionVal.u[2] = buffer[i+2];
    unionVal.u[3] = buffer[i+3];

	value = &unionVal.f;
	return(unionVal.f);
}

void transmit_data_packet(Data_packet_transmit* transmit_data, uint8_t *buffer)
{
    
	uint16_t altitude;
	uint8_t bat_level;

    buffer[0] = transmit_data->c_stat;

    altitude = (uint16_t)transmit_data->altitude;
    buffer[1] = (uint8_t)convert16bittoMSB(altitude); 		 // Yüksek byte
    buffer[2] = (uint8_t)convert16bittoLSB(altitude);        // Düşük byte

	floatToUint8(transmit_data->pressure, buffer, 3);

	bat_level = (uint8_t)transmit_data->bat_level;
    buffer[7] = bat_level;

	floatToUint8(transmit_data->latitude, buffer, 8);

	floatToUint8(transmit_data->longtitude, buffer, 12);


	int16_t crc = RF95_ComputeCRC((uint8_t*)buffer, 16, CRC_TYPE_IBM);

    buffer[16] = (uint8_t)convert16bittoMSB(crc); 
    buffer[17] = (uint8_t)convert16bittoLSB(crc);

    transmit_data->crc = crc; 
}



uint8_t recive_data_packet(Data_packet_recieve* recieve_data, uint8_t *buffer)
{
	uint16_t crc = RF95_ComputeCRC((uint8_t*)buffer, 16, CRC_TYPE_IBM);
	uint16_t crc_control = convert8bitto16bit(buffer[17], buffer[16]);

	if (crc == crc_control)
	{
		recieve_data->c_stat = buffer[0];

		recieve_data->altitude = convert8bitto16bit(buffer[1], buffer[2]);

		uInt8ToFloat(&(recieve_data->pressure), buffer, 3);

		recieve_data->bat_level = (uint8_t)buffer[7];

		uInt8ToFloat(&(recieve_data->latitude), buffer, 8);

		uInt8ToFloat(&(recieve_data->longtitude), buffer, 12);

		return((uint8_t)0);
	}
	else
    	return ((uint8_t)1);
}

uint16_t ComputeCRC(uint16_t crc, uint8_t data, uint16_t polynomial)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if((((crc & 0x8000) >> 8) | (data & 0x80)) != 0)
		{
			crc <<= 1;
			crc |= polynomial;
		}
		else
			crc <<= 1;
		data <<= 1;
	}
	return (crc);
}


uint16_t RF95_ComputeCRC(uint8_t *buffer, uint8_t bufferLength, uint8_t crcType)
{
	uint8_t i;
	uint16_t crc;
	uint16_t polynomial;

	polynomial = (crcType == CRC_TYPE_IBM) ? POLYNOMIAL_IBM : POLYNOMIAL_CCITT;
	crc = (crcType == CRC_TYPE_IBM) ? CRC_IBM_SEED : CRC_CCITT_SEED;

	for(i = 0; i < bufferLength; i++)
		crc = ComputeCRC(crc, buffer[i], polynomial);

	if(crcType == CRC_TYPE_IBM)
		return (crc);
	else
		return ((uint16_t)(~crc));
}
