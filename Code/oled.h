#ifndef _OLED_H
#define _OLED_H

#include "stdint.h"
#define u8 uint8_t
#define u32 uint32_t

void OLED_Init(void);                                       // ��ʼ��OLED
void OLED_Refresh_Gram(void);                               // �����Դ���OLED��ͼ��ˢ�£�
void OLED_Clear(void);                                      // ����
void OLED_DrawPoint(u8 x, u8 y);                            // ����
void OLED_ClearPoint(u8 x, u8 y);                           // ���
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size);            // ��ʾ�ַ�
void OLED_DrawCircle(u8 x, u8 y, u8 r);                     // ��Բ����(x, y)ΪԲ�ģ�r Ϊ�뾶�� 
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size);         // ��ʾ�ַ���
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);    // ��ʾ�������
u32 OLED_Pow(u8 m, u8 n);                                   // m^n   

#endif