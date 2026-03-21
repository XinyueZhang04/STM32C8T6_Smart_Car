#include "stm32f10x.h"                  // Device header
#include "PWM.h"
void Servo_Init(void)
{
	PWM_Init();
}
void Servo_SetAngle(float Angle)
{
	PWM_SetCompare4(Angle/180*2000+500);
}
//20ms,其中高电平的宽度为0.5ms到2.5ms，0.5ms的高电平除于20ms就是占空比，即对应CCR/ARR，500/20000
//同理，2.5ms的高电平除于20ms就是占空比，即对应CCR/ARR，2500/20000
//0到180度对应500到2500，所以有公式