#ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
    #define bool _Bool
    #define true 1
    #define TRUE true
    #define false 0
    #define FALSE false
#else
    #ifdef __GNUC__
    /* GNU C++ supports direct inclusion of stdbool.h to provide C99
        compatibility by defining _Bool */
        #define _Bool bool
    #endif
#endif

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "led.h"
//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
u8 ukey;
u8 umove;
u8 ckey=0;
bool wait_flag=true;
unsigned char c[16];
int num=0;
extern u8 USART_RX_BUF[USART_REC_LEN];     				
extern u16 USART_RX_STA;				
				
				
				
void key(void);
void Usart1_Send(char *str);
void Usart1_Config(void);
void Nvic_Usart1_Config(void);
void move(void);


 int main(void)
 {
	delay_init();
	LED_Init();
  KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart2_init(115200);	 //串口初始化为115200
	Usart1_Config();
	Nvic_Usart1_Config();
	printf("AT+CWMODE=2\r\n");
	delay_ms(1000);
	printf("AT+RST\r\n");
	delay_ms(1000);
	printf("AT+CWSAP=\"ATK-ESP8266\",\"12345678\",1,4\r\n");
	delay_ms(1000);
	printf("AT+CIPMUX=0\r\n");
	delay_ms(1000);

	
 
	while(1)
	{
		//key();
		Usart1_Send("G2F1\r\n");
		   wait_flag=true;
       while(wait_flag);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		Usart1_Send("G3F1\r\n");
		   wait_flag=true;
       while(wait_flag);
		Usart1_Send("G4F1\r\n");
		   wait_flag=true;
       while(wait_flag);
		Usart1_Send("G5F1\r\n");
		   wait_flag=true;
       while(wait_flag);
		
			
	}	 
}
 
void key(void)
{
	ukey=KEY_Scan(1);
	switch(ukey)
	{
		case WKUP_PRES:
		{
			if(ckey==0)
			{
				GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			  printf("AT+CIPSTART=\"TCP\",\"192.168.4.2\",8086\r\n");
	      delay_ms(1000);
	      printf("AT+CIPMODE=1\r\n");
	      delay_ms(1000);
	      printf("AT+CIPSEND\r\n");
	      delay_ms(1000);
			  printf("GAME START\r\n");
			  GPIO_ResetBits(GPIOA,GPIO_Pin_8);
				ckey=1;
				Usart1_Send("G1F1\r\n");
		   wait_flag=true;
       while(wait_flag);
			}
			else
			{
			}
		}
		case KEY0_PRES:
		{
		}
    case KEY1_PRES:
    {
    }			
	}
}
void move(void)
{
	if(USART_RX_STA&0x8000)
	{
		umove=USART_RX_BUF[0];
		USART_RX_STA=0;
	}
	switch(umove)
	{
		case 'w':   //前进
		{  
			 USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
			 Usart1_Send("G1F3\r\n");
		   wait_flag=true;
       while(wait_flag);
			 umove=0;
			 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		}
		case 's':    //后退
		{
			
			umove=0;
		}
		case 'a':    //左转
		{
			
			umove=0;
		}
		case 'd':     //右转
		{
			
			umove=0;
		}
		case 'j':    //左平移
		{
			
			umove=0;
		}
		case 'k':    //右平移
		{
			
			umove=0;
		}
		case 'y':    //飞行模式
	  {
			umove=0;
		}
	}
}

void Usart1_Send(char *str)
{
	while(*str != '\0')
	{		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
		USART_SendData(USART1 , *str);
		str++;
	}
}
void Usart1_Config(void)
{		
	GPIO_InitTypeDef GPIO_Uart;
	USART_InitTypeDef USART_Uart;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);
	
 	GPIO_Uart.GPIO_Pin = GPIO_Pin_9;//tx
  	GPIO_Uart.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Uart.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_Uart);
	
	GPIO_Uart.GPIO_Pin = GPIO_Pin_10;//rx
	GPIO_Uart.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_Uart);
	
	USART_Uart.USART_BaudRate = 9600;
	USART_Uart.USART_WordLength = USART_WordLength_8b;
	USART_Uart.USART_StopBits = USART_StopBits_1;
	USART_Uart.USART_Parity = USART_Parity_No ;
	USART_Uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_Uart);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);																	
	USART_Cmd(USART1, ENABLE);
}
void Nvic_Usart1_Config(void)
{
	NVIC_InitTypeDef NVIC_Uart1;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_Uart1.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Uart1.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelSubPriority = 1;
	NVIC_Uart1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Uart1);
}
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        c[num]=USART1->DR;
        if(c[num-1]=='O'&&c[num]=='K')
        {
            int i;
            wait_flag=false;
            for(i=0;i<15;i++)
                c[i]='\0';
			num=0;
        }
        num++;
        if(num>=15)
            num=0;
    }
}

