#include "dht11.h"
#include "delay.h"

#define DHT11_READ GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define DHT11_HIGH GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define DHT11_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define DHT11_ERRORVALUE 0xff
		
		
void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}
		
		
static void DHT11_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
static void DHT11_In(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
		
static uint8_t DHT11_Start(void)
{
	DHT11_Out();
	DHT11_HIGH;
	delay_ms(20);
	DHT11_LOW;
	delay_ms(20);
	DHT11_HIGH;
	delay_us(30);
	DHT11_In();
	delay_us(40);
	if(DHT11_READ == SET)
	{
		return 0;
	}
	int counter = 0;
	while(DHT11_READ == RESET && ++counter < 1000)
	{
		delay_us(1);
	}
	if(counter >= 1000)
	{
		return 0;
	}
	return 1;
}


static uint8_t DHT11_ReadByte(void)
{
	int counter = 0;
	uint8_t data = 0;
	for(int i = 7;i >= 0;i--)
	{
		while(DHT11_READ == SET && ++counter < 1000)
		{
			delay_us(1);
		}
		if(counter >= 1000)
		{
			return DHT11_ERRORVALUE;
		}
		while(DHT11_READ == RESET && ++counter < 1000)
		{
			delay_us(1);
		}
		if(counter >= 1000)
		{
			return DHT11_ERRORVALUE;
		}
		delay_us(35);
		if(DHT11_READ == SET)
		{
			data |= 1 << i;
		}	
	}
	return data;
}

static void DHT11_Stop(void)
{
	DHT11_Out();
	DHT11_HIGH;
}

void DHT11_ReadTempHum(uint8_t * temp,uint8_t * hum)
{
	uint8_t buf[5] = {0};
	*temp = DHT11_ERRORVALUE;
	*hum = DHT11_ERRORVALUE;
	if(DHT11_Start() == 1)
	{
		for(int i = 0;i < 5;i++)
		{
			buf[i] = DHT11_ReadByte();
		}
		uint8_t check = buf[0] +  buf[1] +  buf[2] +  buf[3];
		if(check == buf[4])
		{
			*temp = buf[2];
			*hum = buf[0];
		}
	}
}





