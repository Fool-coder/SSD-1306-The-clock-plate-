#ifndef _SPI_H
#define _SPI_H

#include "stdint.h"
#include "main.h"
#include "stm32f1xx.h"

#define u8 uint8_t
#define OLED_CMD 0  // ��������
#define OLED_DATA 1 // ��������

#define OLED_RES(x)     x ? HAL_GPIO_WritePin(GPIOA, RES_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, RES_Pin, GPIO_PIN_RESET) // RES��λ
#define OLED_DC(x)      x ? HAL_GPIO_WritePin(GPIOA, DC_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, DC_Pin, GPIO_PIN_RESET)   // 0 ��ʾ�������1 ��ʾ��������
#define OLED_CS(x)      x ? HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_RESET)   // CS Ƭѡ�źţ��͵�ƽ��Ч


void OLED_SPI_Init(void);                       // ���� MCU ��SPI
void SPI_WriteByte(unsigned char data, unsigned char cmd); // ��Ĵ�����ַдһ�� Byte ������
void WriteCmd(unsigned char cmd);               // д����     
void WriteDat(unsigned char date);              // д����

#endif