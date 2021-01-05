#include "oled.h"
#include "font.h"
#include "spi.h"

#define u8 uint8_t
#define u32 uint32_t
// [0] 1 2 3 ... 127
// [1] 1 2 3 ... 127
// [2] 1 2 3 ... 127
// [3] 1 2 3 ... 127
// [4] 1 2 3 ... 127
// [5] 1 2 3 ... 127
// [6] 1 2 3 ... 127
// [7] 1 2 3 ... 127
u8 OLED_GRAM[128][8];     // 这里以八位二进制为一个单位（高位先行）

/*
	初始化 OLED 模块
*/
void OLED_Init(void)
{
	OLED_SPI_Init();
	OLED_RES(0);
	HAL_Delay(200);
	OLED_RES(1);
	//从上电到复位开始初始化要有足够的时间，即等待RC复位完毕
	WriteCmd(0xAE);         // 关闭显示
	WriteCmd(0xD5);         // 设置时钟分频因子 
	WriteCmd(0x80);         
	WriteCmd(0xA8);         // 设置驱动路数
	WriteCmd(0x3F);         // 路数默认 0x3F 
	WriteCmd(0xD3);         // 设置显示偏移
	WriteCmd(0x00);         // 偏移默认 0
	WriteCmd(0x40 | 0x00);  // 设置显示开始行 [5:0]
	WriteCmd(0x8D);         // 电荷汞设置
	WriteCmd(0x10 | 0x04);  // 开启 / 关闭
	WriteCmd(0x20);         // 设置内存地址模式
	WriteCmd(0x02);         // 列地址模式 01 行地址模式 10 页地址模式 10
	WriteCmd(0xA0 | 0x01);  // 段重定义设置    
	WriteCmd(0xC8);         // 驱动  
	WriteCmd(0xDA);         // 设置 COM 硬件引脚设置
	WriteCmd(0x02 | 0x10);  // [5:4] 配置
	WriteCmd(0x81);         // 对比度设置
	WriteCmd(0xEF);         // 默认 0x7F
	WriteCmd(0xD9);         // 设置预充电周期
	WriteCmd(0xF1);         // 
	WriteCmd(0xDB);         //
	WriteCmd(0x30);         // 
	WriteCmd(0xA4 | 0x00);  // 全局显示开启 1 开启，0 关闭
	WriteCmd(0xA6 | 0x00);  // 设置显示方式 1 反相显示，0 正常显示
	WriteCmd(0xAF);         // 开始显示
	OLED_Clear();           // 初始清屏   
}

void OLED_Refresh_Gram(void)
{
	for (int i = 0; i < 8; ++i) {
		WriteCmd(0xb0 + i);   // 设置页地址
		WriteCmd(0x00);       // 列地址 低四位
    WriteCmd(0x10);       // 列地址 高四位
		for (int j = 0; j < 128; ++j) {
			WriteDat(OLED_GRAM[j][i]); // GRAM 信息写入图像
		}
	}
}


void OLED_Clear(void)
{
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 128; ++j) {
			OLED_GRAM[j][i] = 0x00;
		}
	}
	OLED_Refresh_Gram();
}

void OLED_DrawPoint(u8 x, u8 y)
{
	u8 i, m, n;             // x 表示列
	if (x > 127 || y > 63) return ;
	i = y / 8;              // i 表示对应的页（总共八页（每页128列））
	m = y % 8;              
	n = 1 << m;             // n 所求的为在该页下对应的二进制表示（像素点的具体位置） 
	OLED_GRAM[x][i] |= n;
}

void OLED_ClearPoint(u8 x, u8 y)
{
	u8 i, m, n;
	if (x > 127 || y > 63) return ;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

/* 
	Bresenham 画圆算法
*/
void OLED_DrawCircle(u8 x, u8 y, u8 r) // (x, y) 为圆心，r 为半径
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r) {
		OLED_DrawPoint(x + a, y - b);
		OLED_DrawPoint(x - a, y - b);
		OLED_DrawPoint(x - a, y + b);
		OLED_DrawPoint(x + a, y + b);
		
		OLED_DrawPoint(x + b, y - a);
		OLED_DrawPoint(x - b, y - a);
		OLED_DrawPoint(x - b, y + a);
		OLED_DrawPoint(x + b, y + a);
		a++;
		// 计算画的点离圆心的距离 
		num = (a * a + b * b) - r * r;
		if (num > 0) {
			b--;
			a--;
		}
	}
}

/*
	Bresenham 直线算法
*/
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode) 
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1; // x 的增量方向，取或 -1
	int uy = ((dy > 0) << 1) - 1; // y 的增量方向，取或 -1
	int x = x1, y = y1, eps;      // eps 为累加误差
	eps = 0;
	dx < 0 ? (dx = -dx) : (dx = dx);
	dy < 0 ? (dy = -dy) : (dy = dy);
	if (dx > dy) {
		for (x = x1; x != x2; x += ux) {
			if (mode) OLED_DrawPoint(x, y);
			else OLED_ClearPoint(x, y);
			eps += dy;
			if ((eps << 1) >= dx) {
				y += uy, eps -= dx;
			}
		}			
	}
	else {
		for (y = y1; y != y2; y += uy) {
			if (mode) OLED_DrawPoint(x, y);
			else OLED_ClearPoint(x, y);
			eps += dx;
			if ((eps << 1) >= dy) {
				x += ux, eps -= dy;
			}
		}
	}
}

/*
	在指定位置显示 1 个字符
*/
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size)
{
	u8 temp;
	u8 y0 = y;
	// 得到该字符所占用的字节数
	u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
	chr = chr - ' ';
	for (int i = 0; i < csize; ++i) {
		if(size == 12) temp = asc2_1206[chr][i];      // 调用1206字体
    else if(size == 16) temp = asc2_1608[chr][i]; // 调用1608字体
    else if(size == 24) temp = asc2_2412[chr][i]; // 调用2412字体	
		else return ;                                 // 没有字库
		for (int j = 0; j < 8; ++j) {
			if (temp & 0x80) OLED_DrawPoint(x, y);
			else OLED_ClearPoint(x, y);
			temp <<= 1;
			y++;
			if ((y - y0) == size) {
				y = y0;
				x++;
				break;
			}
		}
	}
}

/*
	在指定位置显示字符串
*/
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size)
{
	while ((*chr >= ' ') && (*chr <= '~')) { // 判断是否为非法字符
		OLED_ShowChar(x, y, *chr, size);
		x += size / 2;
		if (x > 128 - size) { // 换行
			x = 0;
			y += 2;
		}
		++chr;
	}
}

u32 OLED_Pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--) result *= m;
	return result;
}

/*
	指定位置显示 多个 数字
*/
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size)
{
	u8 temp;
	for (int i = 0; i < len; ++i) {
		temp = (num / OLED_Pow(10, len - i - 1)) % 10;
		if (temp == 0) { // 按照字符显示，特判一下 0 
			OLED_ShowChar(x + (size / 2) * i, y, '0', size); // 对 x 进行偏移	
		}
		else {
			OLED_ShowChar(x + (size / 2) * i, y, temp + '0', size);
		}
	}
}

/*
	判闰年
*/
int isLeapYear(int y)
{
	return y % 4 == 0 && y % 100 != 0 || y % 400 == 0;
}