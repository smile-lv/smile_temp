/*********************************************
*	�ļ����ƣ�oled.h
*	��    �ߣ�������
*	�ļ�����������STM32��е�ۿ������OLED
*	���Ŷ��壺
*              GND   ��Դ��
*              VCC   ��5V��Դ
*              SCL   ��PB6��SCL��
*              SDA   ��PB7��SDA��      
*   �޸�ʱ�䣺2021-09-09      
* 	ע���������IO��ģ��IIC���߿���OLED
*********************************************/
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stm32f10x.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC�˿ڶ���----------------				   

#define  OLED_IIC_SDA_WRITE(n)   (n)?GPIO_SetBits(GPIOB,GPIO_Pin_7):GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define  OLED_IIC_SCL_WRITE(n)   (n)?GPIO_SetBits(GPIOB,GPIO_Pin_6):GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define  OLED_IIC_SDA_READ       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED���ƺ���
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ReverseShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);

void OLED_ShowNum(u8 x,u8 y,int32_t num,u8 len,u8 size);
void OLED_ReverseShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_ReverseShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_ReverseShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);

//OLED����IIC����ͨѶ����
void OELD_IIC_Start(void);
void OLED_IIC_Stop(void);
void OLED_IIC_SDAOutputMode(void);
void OLED_IIC_SDAInputMode(void);
void OLED_Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Write_IIC_Command(unsigned char IIC_Command);
void OLED_Write_IIC_Data(unsigned char IIC_Data);
uint8_t OLED_IIC_WaitAck(void);
#endif  
	 



