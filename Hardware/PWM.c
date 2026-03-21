#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	//1.启动时钟源
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);     //给4个电机的PWM,TIM2有两个通道，GPIOA口，通道3和通道2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//给舵机模块的PWM，GPIOB口，TIM3第4通道
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//2.启动GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;//GPIOA1是通道2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //初始化舵机的B1口
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//3.初始化时基单元
	//TIM2是给四个电机的
	TIM_InternalClockConfig(TIM2);
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//这种写的后面再配一个PWM波就不用写了
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC   //因为舵机的周期是20ms，1000ms是1s
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;         //f=50=72M%(ARR+1)%(PSC+1),72000000
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	//4，初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;//这种写的后面再配一个PWM波就不用写了
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	//5.使能TIM
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	//TIM3给舵机模块的

}
void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}
void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);    //自设定3个CCR的值，以调占空比，compare就是CCR的值
}
void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM3, Compare);
}
