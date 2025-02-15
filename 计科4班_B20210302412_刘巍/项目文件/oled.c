/*********************************************
*	文件名称：oled.c
*	作    者：李明轩
*	文件描述：测试STM32机械臂开发板的OLED
*	引脚定义：
*              GND   电源地
*              VCC   接5V电源
*              SCL   接PB6（SCL）
*              SDA   接PB7（SDA）      
*   修改时间：2021-09-09      
* 	注意事项：采用IO口模拟IIC总线控制OLED
*********************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

//设置OLED屏的SDA引脚为输出模式
void OLED_IIC_SDAOutputMode(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
	//1. 开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	//2. 配置OLED引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;				//推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;		                //引脚定义
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//输出速率

	//3. 引脚初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
}

//设置OLED屏的SDA引脚为输入模式
void OLED_IIC_SDAInputMode(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
	//1. 开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	//2. 配置OLED引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;				    //上拉输入
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;		                //引脚定义
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//输出速率

	//3. 引脚初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);			
}


void OELD_IIC_Start(void)
{
    //1.设置SDA引脚为输出模式
	OLED_IIC_SDAOutputMode();
	
	//2.确保SDA和SCL为高电平
	OLED_IIC_SCL_WRITE(1);
	OLED_IIC_SDA_WRITE(1);
    delay_us(5);            //提高程序可靠性
    
	//3.把SDA引脚拉低
	OLED_IIC_SDA_WRITE(0);
	delay_us(5);   // IIC总线的通信速率为100KHZ  1000000us = 100000HZ  10us = 1HZ 
	
	//4.把SCL引脚拉低，表示准备通信
	OLED_IIC_SCL_WRITE(0);
}


void OLED_IIC_Stop(void)
{
    //1.设置SDA引脚为输出模式
	OLED_IIC_SDAOutputMode();

	//2.确保SDA和SCL为低电平
	OLED_IIC_SCL_WRITE(0);
	OLED_IIC_SDA_WRITE(0);
	
	//4.把SCL引脚拉高
	OLED_IIC_SCL_WRITE(1);
	delay_us(5);
	
	//5.把SDA引脚拉高
	OLED_IIC_SDA_WRITE(1);
	delay_us(5);  //确保SDA的电平状态可以被其他从器件检测到
	
}

uint8_t OLED_IIC_WaitAck(void)
{
    uint8_t ack;
	
	//1.设置SDA引脚为输入模式
	OLED_IIC_SDAInputMode();

	//2.SCL为低电平      从机准备数据
	OLED_IIC_SCL_WRITE(0);
	delay_us(5);
	
	//3.SCL为高电平      从机发送数据
	OLED_IIC_SCL_WRITE(1);
	delay_us(5);
	
	//主机判断从机的数据
	if( OLED_IIC_SDA_READ == 1)
	{
		ack=1; //说明无应答
	}
	else
		ack=0; //说明已应答
	
	//4.SCL为低电平       主机忽略数据
	OLED_IIC_SCL_WRITE(0);
	delay_us(5);
	
	return ack;

}
/**********************************************
// IIC Write byte
**********************************************/
void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
{
	uint8_t i= 0;
	
	//1.设置SDA引脚为输出模式
	OLED_IIC_SDAOutputMode();
	
	//2.确保SDA和SCL为低电平
	OLED_IIC_SCL_WRITE(0);
	OLED_IIC_SDA_WRITE(0);
	
	//3.循环发送bit
	for(i=0;i<8;i++)
	{
		//MSB 高位先出   主机准备数据
		if ( IIC_Byte & 1<<(7-i) ) 
		{
			OLED_IIC_SDA_WRITE(1);
		}
		else
			OLED_IIC_SDA_WRITE(0);
		
		delay_us(5);
		
		//SCL为高电平    主机发送数据
		OLED_IIC_SCL_WRITE(1);
		delay_us(5);
		
		//SCL为低电平    主机准备数据
		OLED_IIC_SCL_WRITE(0);
		delay_us(5);
	}
}
/**********************************************
// IIC Write Command
**********************************************/
void OLED_Write_IIC_Command(unsigned char IIC_Command)
{
    //1.主机发送开始信号
    OELD_IIC_Start();

    //2.主机发送器件地址
    OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0

    //3.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);

    //4.主机发送
    OLED_Write_IIC_Byte(0x00);			//write command
    
    //5.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);
    
    //6.主机发送命令
    OLED_Write_IIC_Byte(IIC_Command); 
    
    //7.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);
    
    //8.主机发送停止信号
    OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLED_Write_IIC_Data(unsigned char IIC_Data)
{
    //1.主机发送开始信号
    OELD_IIC_Start();

    //2.主机发送器件地址
    OLED_Write_IIC_Byte(0x78);            //Slave address,SA0=0

    //3.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);

    //4.主机发送
    OLED_Write_IIC_Byte(0x40);			//write data
    
    //5.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);
    
    //6.主机发送命令
    OLED_Write_IIC_Byte(IIC_Data); 
    
    //7.主机等待从机应答    
    while( OLED_IIC_WaitAck() == 1);
    
    //8.主机发送停止信号
    OLED_IIC_Stop();
}


void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
    {
        OLED_Write_IIC_Data(dat);
    }
    else 
        OLED_Write_IIC_Command(dat);
}


/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_WR_Byte(0xb0+m,0);		//page0-page1
		OLED_WR_Byte(0x00,0);		//low column start address
		OLED_WR_Byte(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_WR_Byte(fill_Data,1);
			}
	}
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}

//反显一个字符  y以8倍进行计算
void OLED_ReverseShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);
		}
		else {	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(~F6x8[c][i],OLED_DATA);
				
		}
}


//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,int32_t num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;
  if(num<0)
	{
			num=-num;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//反显一个数字 参数与显示一个数字的参数一样
void OLED_ReverseShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{
	u8 t,temp;
	u8 enshow=0;						    
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ReverseShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ReverseShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
}




//显示一个字符串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//反显示一个字符串
void OLED_ReverseShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ReverseShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字  x = 0~127  y = 0~7    汉字在数组中的下标
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

//反显示汉字  x = 0~127  y = 0~7    汉字在数组中的下标
void OLED_ReverseShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(~Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(~Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}


/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
    GPIO_InitTypeDef GPIO_InitStructure;
	
	//1. 开启GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	//2. 配置OLED引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;				//推挽输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;		//引脚定义
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//输出速率

	//3. 引脚初始化
	GPIO_Init(GPIOB, &GPIO_InitStructure);							
    

    delay_ms(800);
    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  





























