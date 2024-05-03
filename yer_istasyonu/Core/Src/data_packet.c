#include "data_packet.h"
#include "main.h"
#include "crc.h"
#include "spi.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

uint8_t transmit_data_packet(Data_packet* transmit_data) {
    uint8_t buffer[20]; 
	uint16_t altitude;
	uint8_t bat_level;

    buffer[0] = transmit_data->c_stat;

    altitude = (uint16_t)transmit_data->altitude;
	bat_level = (uint8_t)transmit_data->bat_level;
    buffer[1] = (uint8_t)(altitude >> 8); // Yüksek byte
    buffer[2] = (uint8_t)altitude;       // Düşük byte


    buffer[7] = transmit_data->bat_level;


    
    uint16_t crc = HAL_CRC_Calculate(&hcrc, buffer, 20); 

    buffer[18] = (uint8_t)(crc >> 8); 
    buffer[19] = (uint8_t)crc;        

    transmit_data->crc = crc; 

    return buffer; 
