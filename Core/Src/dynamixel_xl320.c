//***********************************************
// STM32_servo_test
// Copyright (c) 2022 Ricardo SILVESTRE.
// All rights reserved.
//***********************************************

#include "../Inc/dynamixel_xl320.h"

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max){
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DMA_receive_delay(){  //verify a good value for the delay
	HAL_Delay(1000);
}

/*	red		1
	green	2
	blue	4
	yellow	3
	cyan	6
	purple	5
	white	7*/
void XL320_set_led_ON(UART_HandleTypeDef *m_huart, uint8_t color){
	uint8_t CRC_L = 0;
	uint8_t CRC_H = 0;
	uint8_t TxPacket[13] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_BROADCAST_ID,
							0x06,
							0x00,
							DXL_INST_WRITE,
							0x19,
							0x00,
							color,
							CRC_L,
							CRC_H};
	uint16_t CRC_2 = update_crc(0, TxPacket, 11);
	CRC_L = (CRC_2 & 0x00FF);
	CRC_H = ((CRC_2 >> 8) & 0x00FF);
	TxPacket[11] = CRC_L;
	TxPacket[12] = CRC_H;

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 13, HAL_MAX_DELAY);
}

void XL320_set_led_OFF(UART_HandleTypeDef *m_huart){
	uint8_t CRC_L = 0;
	uint8_t CRC_H = 0;
	uint8_t TxPacket[13] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_BROADCAST_ID,
							0x06,
							0x00,
							DXL_INST_WRITE,
							DXL_LOBYTE(XL_LED),
							DXL_HIBYTE(XL_LED),
							0x00,
							CRC_L,
							CRC_H};
	uint16_t CRC_2 = update_crc(0, TxPacket, 11);
	CRC_L = (CRC_2 & 0x00FF);
	CRC_H = ((CRC_2 >> 8) & 0x00FF);
	TxPacket[11] = CRC_L;
	TxPacket[12] = CRC_H;

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 13, HAL_MAX_DELAY);
}

void XL320_set_pos(UART_HandleTypeDef *m_huart, uint8_t pos){
	uint8_t CRC_L = 0;
	uint8_t CRC_H = 0;
	map(pos, 0, 300, 0, 1023);
	uint8_t pos_L = (pos & 0x00FF);
	uint8_t pos_H = ((pos >> 8) & 0x00FF);
	uint8_t TxPacket2[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							 DXL2_0_PACKET_IDX_HEADER_2,
							 DXL2_0_PACKET_IDX_HEADER_3,
							 DXL2_0_PACKET_IDX_RESERVED,
							 DXL_BROADCAST_ID,
							 0x07,
							 0x00,
							 DXL_INST_WRITE,
							 0x1E,
							 0x00,
							 pos_L,
							 pos_H,
							 CRC_L,
							 CRC_H};
	uint16_t CRC_2 = update_crc(0, TxPacket2, 12);
	CRC_L = (CRC_2 & 0x00FF);
	CRC_H = ((CRC_2 >> 8) & 0x00FF);
	TxPacket2[12] = CRC_L;
	TxPacket2[13] = CRC_H;

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket2, 14, HAL_MAX_DELAY);
}

void XL320_read_moving(UART_HandleTypeDef *m_huart){
	uint8_t CRC_L = 0;
	uint8_t CRC_H = 0;
	uint8_t RxPacket[13] = {0};

	uint8_t TxPacket[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_DEFAULT_ID,
							0x07,
							0x00,
							DXL_INST_READ,
							0x2E,
							0x00,
							0x01,
							0x00,
							CRC_L,
							CRC_H};
	uint16_t CRC_2 = update_crc(0, TxPacket, 12);
	CRC_L = (CRC_2 & 0X00FF);
	CRC_H = ((CRC_2 >> 8) & 0x00FF);
	TxPacket[12] = CRC_L;
	TxPacket[13] = CRC_H;

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 14, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 13);
	DMA_receive_delay();
}

uint16_t XL320_read_load(UART_HandleTypeDef *m_huart){
	uint8_t CRC_L = 0;
	uint8_t CRC_H = 0;
	uint8_t RxPacket[14] = {0};

	uint8_t TxPacket[14] = {DXL2_0_PACKET_IDX_HEADER_1,
							DXL2_0_PACKET_IDX_HEADER_2,
							DXL2_0_PACKET_IDX_HEADER_3,
							DXL2_0_PACKET_IDX_RESERVED,
							DXL_DEFAULT_ID,
							0x07,
							0x00,
							DXL_INST_READ,
							0x29,
							0x00,
							0x02,
							0x00,
							CRC_L,
							CRC_H};
	uint16_t CRC_2 = update_crc(0, TxPacket, 12);
	CRC_L = (CRC_2 & 0X00FF);
	CRC_H = ((CRC_2 >> 8) & 0x00FF);
	TxPacket[12] = CRC_L;
	TxPacket[13] = CRC_H;

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket, 14, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 14);
	DMA_receive_delay();

	uint16_t present_load = (uint16_t) (RxPacket[11] << 8) | (RxPacket[10]);
	uint16_t load_ratio = present_load & 0b0000001111111111;

	return load_ratio;
}

void XL320_ping(UART_HandleTypeDef *m_huart){
	uint8_t RxPacket[100] = {0};

	uint8_t TxPacket3[10] = {DXL2_0_PACKET_IDX_HEADER_1,
							 DXL2_0_PACKET_IDX_HEADER_2,
							 DXL2_0_PACKET_IDX_HEADER_3,
							 DXL2_0_PACKET_IDX_RESERVED,
							 DXL_BROADCAST_ID,
							 0x03,
							 0x00,
							 DXL_INST_PING,
							 0x31,
							 0x42};

	HAL_UART_Transmit(m_huart, (uint8_t *) &TxPacket3, 10, HAL_MAX_DELAY);
	HAL_UART_Receive_DMA(m_huart, (uint8_t *) &RxPacket, 100);
	DMA_receive_delay();
}