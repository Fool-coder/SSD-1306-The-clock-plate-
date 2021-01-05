#include "spi.h"
// �ⲿ����
extern SPI_HandleTypeDef hspi1;
/*
	ʵ���ĸ�����:
	// 1��GPIO ģ�� SPI �˿ڳ�ʼ��
	2����Ĵ�����ַ��д��һ�� Byte ����
	3��д����
	4��д����
*/

// SPI_HandleTypeDef hspi;

// ���ų�ʼ��(����Ϊ���)
void OLED_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = CS_Pin|DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RES_GPIO_Port, &GPIO_InitStruct);
	OLED_RES(1);
}

/*
	data ��ʾҪд�������/����
	cmd  ����/�����־��0 ��ʾ���1 ��ʾ����
*/
void SPI_WriteByte(unsigned char data, unsigned char cmd)
{
	OLED_DC(cmd);
	OLED_CS(0);
	// Ӳ�� SPI
	HAL_SPI_Transmit(&hspi1, &data, 1, 10);
	OLED_CS(1);
	OLED_DC(1);
}

/* д���� */
void WriteCmd(unsigned char cmd)
{
	SPI_WriteByte(cmd, OLED_CMD);
}

/* д���� */
void WriteDat(unsigned char data)
{
	SPI_WriteByte(data, OLED_DATA);
}