#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX引脚，发送模式，输出数据，一根线可以有一个输出，但是可以有多个输入，接蓝牙模块的RX引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RX引脚，接收模式，输入数据,接蓝牙模块的TX引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//串口模式
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位1位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长，不需要校验，那字长就选择8位
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启RXNE标志位到NVIC的输出
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//优先位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)//发送字节
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//置1，置1说明上一个字节发送完了，可以进行下一个了 
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)//发送数组，*Array是指向数组的首地址
{//因为数组无法判断数组是否结束，所以再传了一个Length，是数组的长度
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)//发送字符串，因为字符串自带一个结束标志位，所以就不用Length了
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//当字符串不等于\0标志位时，就一直传到结束
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)//所以我们需要写一个次方函数
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;//X的Y次方，X乘Y次，比如X=10，Y=3，的10的3次方
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)//发送字符形式的数字，需要将10位100位，个位的数拆分后发送出去
{//如12345，12345/10000%10=1，对10取余，1除于10=0，余1。12345/1000%10=2
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');//因为以字符的形式表示数据，所以要再加个0，偏移
	}
}

int fputc(int ch, FILE *f)//有了这个printf的函数，	就可以直接在main函数中直接用Print
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)//这位置1了，就是收到数据了
{
	if (Serial_RxFlag == 1)//读完标志位后清0
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART1_IRQHandler(void)//中断函数
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除标志位
	}
}
