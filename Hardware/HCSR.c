#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int16_t Num;
void HCSR_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出，一个输出，多个输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//triger
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//echo
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM1);//72MHz
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//这种写的后面再配一个PWM波就不用写了
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 60000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
}
float HCSR_Distance(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);//triger发出一个高电平
	Delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==RESET);//如果高电平了，就跳出循环，高电平的时候就开始计时，cnt是系统时钟以72兆赫兹给你计时
	//  cnt/1000000，就得出计时了多少秒，1秒系统计1000000次
	TIM1->CNT=0;//CNT清零，CNT就是计数了多少次
	TIM_Cmd(TIM1,ENABLE);//如果echo15引脚收到高电平，那时钟开启
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==SET);//如果低电平了，就跳出循环，
	TIM_Cmd(TIM1,DISABLE);//TIM关闭
	Num=TIM_GetCounter(TIM1);
	float Distance=(Num*1.0*0.034)/2;//时间乘声音速度为距离
	Delay_ms(100);//每次重新工作就停一下
	return Distance;//cnt除于10的6次方，因为系统72MHz，预分频为72000000/72=10的6次方，即1秒测10的6次方次，
	//我们共测了CNT次，则cnt/1000000*0.034*1000000，因为340是秒要化为微秒，1秒为1000000微秒
}
