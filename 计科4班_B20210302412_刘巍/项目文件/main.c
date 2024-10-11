#include "delay.h"
#include "usart.h"
#include "duoji.h"
#include "oled.h"
#include "led_key.h"
#include <stdio.h>
#include "list.h"
#include "list.h"
#include "dht11.h"
extern uint8_t RecvData;





//??????
u8 Caidan[4][16] = {
	"Study Mode",
	"Auto Play",
	"Time",
	"DHT11"
};


int CaidanFalg = 0;
int time_s = 0;
int time_mi = 0;
int time_h = 15;
int time_d = 26;
int time_mo = 9;
int time_y = 2024;


int main()
{
	delay_Init();
	LedInit();
	KeyInit();
	Usart1_Init(9600);
	Duoji_Init(115200);
	
	//???OLED
	OLED_Init();
	OLED_Clear();//??
	DHT11_Init();
	
	uint8_t temp,hum;
//	while(1)//????DHT11
//	{
//								OLED_Clear();
//									DHT11_ReadTempHum(&temp,&hum);
//									OLED_ShowNum(0,0,temp,4,16);
//									OLED_ShowNum(0,2,hum,4,16);
//									delay_ms(2000);
//	}
//}
	ListHeadNode *head =HeadList_Init();
	
	
	
	OLED_ReverseShowString(0,0,Caidan[0],16);
	OLED_ShowString(0,2,Caidan[1],16);
	OLED_ShowString(0,4,Caidan[2],16);
	OLED_ShowString(0,6,Caidan[3],16);
	#if 0
	while(1)
	{
		if(KeyControl(1) == SET)
		{
				LedControl(1,0);
		}
		else if(KeyControl(2) == SET)
		{
				LedControl(2,0);
		}
		else if(KeyControl(3) == SET)
		{
				LedControl(1,0);
		}
		else if(KeyControl(4) == SET)
		{
				LedControl(2,0);
		}
		else
		{
			LedControl(0,1);
		}
	
	}
	#endif
	//RecvData = 'T';
	while(1)
	{
		
		LedControl(1,1);
		delay_ms(100);
		LedControl(1,0);
		duoji_control();
		if(KeyControl(1) == SET || RecvData == 'A')
		{
										
				if(CaidanFalg < 3)
				{
							CaidanFalg++;
					    RecvData ='Z';
								//???????? ????CaidanFalg??
							OLED_ShowString(0,(CaidanFalg - 1) * 2,Caidan[CaidanFalg - 1],16);
							OLED_ReverseShowString(0,CaidanFalg * 2,Caidan[CaidanFalg],16);
				}
							//??????
				while(KeyControl(1) == SET || RecvData == 'A');
		}
		
		
		else if(KeyControl(2) == SET || RecvData == 'B')
		{
				if(CaidanFalg > 0)
				{
					 RecvData ='Z';
						CaidanFalg--;
						//???????? ????CaidanFalg??
						OLED_ShowString(0,(CaidanFalg + 1) * 2,Caidan[CaidanFalg + 1],16);
						OLED_ReverseShowString(0,CaidanFalg * 2,Caidan[CaidanFalg],16);
				}
						//??????
						while(KeyControl(2) == SET || RecvData == 'B');
		}
		
		else if(KeyControl(3) == SET || RecvData == 'C')
		{
					RecvData ='Z';
					if(CaidanFalg==0)
					{
						if(head->first!=NULL)
						{
							HeadList_Destroy(head);
						}	
						
						OLED_Clear();
						OLED_ShowString(0,1 * 2,(u8*)" Studing in 10s ",16);
					  record_list(head);
						OLED_Clear();
						OLED_ShowString(0,1 * 2,(u8*)"   studing OK",16);
					 }
										 
						else if(CaidanFalg==1)
						{
							
							OLED_Clear();
						
								
							OLED_ShowString(0,1 * 2,(u8*)"    Working!",16);
																																									
							
							play_record(head);	
									
						}
						
							else if(CaidanFalg==2)
						{
								
							OLED_Clear();	
							int s = 0;
							int mi = 0;
							int h = 15;
							int d = 29;
							int y = 2024;
							
							u8 TIME[3][3]={h,mi,s};
					   
								while(KeyControl(4) !=	SET)	
								{									
									
										s+=1;
									if(s>=59)
									{
										s=0;
										mi++;
									}
									if(mi>=59)
									{
										mi=0;
										h++;
									}
									if(h>=23)
									{
										h=0;
										d++;
									}
								
								
								
								char hour_str[3], minute_str[3], second_str[3];
								sprintf(hour_str, "%02d", h);
								sprintf(minute_str, "%02d", mi);
								sprintf(second_str, "%02d", s);
								
					   
								
								char time_str[9] = {0};
								strcat(time_str, hour_str);
								strcat(time_str, ":");
								strcat(time_str, minute_str);
								strcat(time_str, ":");
								strcat(time_str, second_str);
								
								u8 *time_str_u8 = (u8 *)time_str;
								
								OLED_ShowString(25, 3, time_str_u8, 16);									
									
																					
									delay_ms(1000);		
									if( RecvData == 'D')
								{
									break;
									}
															
								}
									
							OLED_Clear();
						}
							else if(CaidanFalg==3)
						{
								OLED_Clear();
								delay_ms(100);
							while(KeyControl(4) !=	SET)
							{
								
									
									DHT11_ReadTempHum(&temp,&hum);
									OLED_ShowNum(0,0,temp,4,16);
									OLED_ShowNum(0,2,hum,4,16);

									delay_ms(100);
								if( RecvData == 'D')
								{
									break;
									}
							}
						}
				while(KeyControl(3) == SET || RecvData == 'C');
			}

		else if(KeyControl(4) == SET || RecvData == 'D')
		{
			{
			RecvData ='Z';
			OLED_Clear();
			CaidanFalg = 0;
			OLED_ReverseShowString(0,0,Caidan[0],16);
			OLED_ShowString(0,2,Caidan[1],16);
			OLED_ShowString(0,4,Caidan[2],16);
			OLED_ShowString(0,6,Caidan[3],16);
		}
			
			//??????
	 	while(KeyControl(4) == SET || RecvData == 'D');
		}
	}
}



