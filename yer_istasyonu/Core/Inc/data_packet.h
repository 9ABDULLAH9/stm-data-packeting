


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

//typedef


// Fonksiyon Prototipleri


#endif // FUNCTION_H
//18byte