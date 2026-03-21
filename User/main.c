#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "PWM.h"
#include "Car.h"
#include "Servo.h"
#include "Serial.h"
#include "HCSR.h"
#include "Track.h"
uint8_t RxData;
int main(void)
{
	
	CAR_Init();
	Serial_Init();
    Servo_Init();
	Track_Init();
	while(1)
	{ 
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0&&
			GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
		    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
		    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Go_Ahead();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
		{
			Car_Stop();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
		{
			Self_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Self_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
		{
			Self_Right();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Turn_Left();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Self_Left();
		}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0&&
		     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0)
		{
			Turn_Left();
		}	

    }
}

