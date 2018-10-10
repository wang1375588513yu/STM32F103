#include "Run_task.h"
#include "Data_Structure.h"

#include "Led_driver.h"
#include "beep_driver.h"
#include "TIM_driver.h"
#include "usart_driver.h"




static void Run_Task(void)
{
	uint8_t msg = NO_MSG;
	
	#ifdef USART1_DEBUG
	printf("Run_Task\r\n");
	#endif
	
	while(1)
	{
		msg = get_msg_Fifo();
		
		switch(msg)
		{
			case MSG_IOKEY_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_IOKEY_SHORT\r\n");
				#endif
				Led_Ctl(LED_BLUE,ON);
				
				break;
			case MSG_ADKEY_1_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_1_SHORT\r\n");
				#endif
			
				Led_Ctl(LED_BLUE,OFF);
				
				break;
			case MSG_ADKEY_2_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_2_SHORT\r\n");
				#endif
				Led_Ctl(LED_RED,ON);
				break;
			case MSG_ADKEY_3_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_3_SHORT\r\n");
				#endif
				Led_Ctl(LED_RED,OFF);
				break;
			case MSG_ADKEY_4_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_4_SHORT\r\n");
				#endif
				Led_Ctl(LED_GREEN,ON);
				break;
			case MSG_ADKEY_5_SHORT:
				#ifdef USART1_DEBUG
				printf("MSG_ADKEY_5_SHORT\r\n");
				#endif
				Led_Ctl(LED_GREEN,OFF);
				break;
			case MSG_100MS:
				break;
			case MSG_200MS:
				#ifdef USART1_DEBUG
				printf("MSG_200MS\r\n");
				#endif
				break;
			case MSG_HALF_SECOND:
				break;
			case MSG_800MS:
				break;
			default:
				break;
		}
	}
}


void Run_Start(void)
{
	#ifdef USART1_DEBUG
	printf("Run_Start\r\n");
	#endif
	
	SeqQueue_Init();
	Clear_msg_Fifo();
	
	Run_Task();
}

