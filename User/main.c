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
    // ---------------- 初始化 ----------------
    CAR_Init();
    Serial_Init();
    Servo_Init();
    Track_Init();

    while(1)
    {
        // -------- 蓝牙控制 --------
        if(Serial_GetRxFlag() == 1)
        {
            RxData = Serial_GetRxData();

            switch(RxData)
            {
                case 0x40: Go_Ahead(); break;
                case 0x41: Go_Back(); break;
                case 0x42: Turn_Left(); break;
                case 0x43: Turn_Right(); break;
                case 0x44: Car_Stop(); break;
                case 0x45: Self_Right(); break;
                case 0x46: Self_Left(); break;
            }
        }

        // -------- 循迹模块 --------
        uint8_t S8 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);
        uint8_t S6 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
        uint8_t S7 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
        uint8_t S15 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);

        if(S8==0 && S6==0 && S7==0 && S15==0)      Go_Ahead();
        else if(S8==1 && S6==1 && S7==1 && S15==1) Car_Stop();
        else if(S8==0 && S6==0 && S7==1 && S15==1) Self_Right();
        else if(S8==0 && S6==0 && S7==1 && S15==0) Turn_Right();
        else if(S8==0 && S6==0 && S7==0 && S15==1) Turn_Right();
        else if(S8==0 && S6==1 && S7==0 && S15==0) Turn_Left();
        else if(S8==1 && S6==1 && S7==0 && S15==0) Self_Left();
        else if(S8==1 && S6==0 && S7==0 && S15==0) Turn_Left();

        // -------- 超声避障 --------
        if(S8==0 && S6==0 && S7==0 && S15==0) // 只有前进时才避障
        {
            uint16_t distance = HCSR_Distance(); 
            Serial_SendNumber(distance, 3);

            if(distance < 15) // 距离太近
            {
                Car_Stop();
                Servo_SetAngle(0);
                Delay_ms(1000);

                uint16_t left_dist = HCSR_Distance();
                Serial_SendNumber(left_dist,3);
                if(left_dist > 15)
                {
                    Servo_SetAngle(90);
                    Delay_ms(1000);
                    Self_Right();
                    Delay_ms(1000);
                    Go_Ahead();
                }
                else
                {
                    Servo_SetAngle(180);
                    Delay_ms(1000);
                    uint16_t right_dist = HCSR_Distance();
                    Serial_SendNumber(right_dist,3);
                    if(right_dist > 15)
                    {
                        Servo_SetAngle(90);
                        Delay_ms(1000);
                        Self_Left();
                        Delay_ms(1000);
                        Go_Ahead();
                    }
                    else
                    {
                        Servo_SetAngle(90);
                        Delay_ms(1000);
                        Go_Back();
                        Delay_ms(1000);
                        Car_Stop();
                    }
                }
            }
        }

        Delay_ms(10); // 循环延时，避免占用CPU
    }
}
