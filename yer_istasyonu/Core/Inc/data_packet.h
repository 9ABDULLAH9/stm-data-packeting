#include <stdint.h>

#ifndef DATA_PACKET_H
#define DATA_PACKET_H

// Dosya Bilgileri
#include "main.h"
#include "crc.h"
#include "spi.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

// Dosya Adı ve Versiyon Bilgisi
#define DOSYA_ADI "data_packet.h"
#define VERSIYON "1.0"

// Yazar Bilgisi
#define YAZAR "Mert"

//
#define convert8bitto16bit(x, y)  (((x) << 8) | (y))
#define convert16bittoLSB(x) ((x) & 255)
#define convert16bittoMSB(x) ((x) >> 8)

//typedef

typedef struct data_packet
{
	uint8_t c_stat;
	float altitude;
	float pressure;
	float bat_level;
	float latitude;
	float longtitude;
	uint16_t crc;
}Data_packet;

typedef union {
    float f;
    uint8_t u[4];
} FloatUInt8Union;


// Fonksiyon Prototipleri
uint8_t *transmit_data_packet(Data_packet* transmit_data);
void floatToUint8(float value, uint8_t *buffer, uint8_t i,
						uint8_t ii, uint8_t iii, uint8_t iv);

#endif // FUNCTION_H
