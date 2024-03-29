/* Includes ------------------------------------------------------------------*/
#include "includes.h"

RTC_DateTypeDef RTC_DateStruct_get;
RTC_TimeTypeDef RTC_TimeStruct_get;

/*--------xQueueHandle queue------------------*/
xQueueHandle WifiSendQueue;
xQueueHandle WifiReceQueue;
xQueueHandle DCSendQueue;
xQueueHandle DCReceQueue;
xQueueHandle ACSendQueue;
xQueueHandle ACReceQueue;
xQueueHandle TouchSendQueue;
xQueueHandle TouchReceQueue;

extern uint16_t Block[4096];
extern uint16_t BlockSize;
struct air_condition air_condition_current={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int main(void)
{
	Init();
 /* Start tasks */ 
	xTaskCreate(Main_task, (int8_t *) "Main_task", STACK_MAIN_TASK,    NULL, MAIN_TASK,  NULL);
	xTaskCreate(Wifi_task, (int8_t *) "Wifi_task", STACK_WIFI_TASK,    NULL, WIFI_TASK,  NULL);
	xTaskCreate(Touch_task, (int8_t *) "Touch_task", STACK_TOUCH_TASK, NULL, TOUCH_TASK, NULL);
	xTaskCreate(DC_task, (int8_t *) "DC_task",     STACK_DC_TASK,      NULL, DC_TASK,    NULL);
	xTaskCreate(AC_task, (int8_t *) "AC_task",     STACK_AC_TASK,      NULL, AC_TASK,    NULL);
 /* Start scheduler */
  vTaskStartScheduler();
}
/**
	* @brief Init function 
	* @param None
	* @retval None
	*/
void Init(void)
{
	Blu_Led_Pwm();
	uart_init();
	rtc_init();
	Flash_Init();
	Tem_Rh_Gpio_Init();
	//Flash_Test();
	queue_init();
}
/**
  * @brief  Main task run in the background all the time
  * @param  None
  * @retval None
  */
void Main_task(void * pvParameters)
{
	struct air_condition air_condition_main;
	while(1)
	{
		//vTaskDelay(2000);
		uart_putstr("Main_task start.");
		if(xQueueReceive(DCSendQueue,&air_condition_main, 100/portTICK_RATE_MS )==pdPASS)
		{
			if(air_condition_main.air_fault==1)
			{
				//��������
			}
			else
			{
				air_condition_main.month=RTC_DateStruct_get.RTC_Month;
				air_condition_main.day=RTC_DateStruct_get.RTC_Date;
				air_condition_main.week=RTC_DateStruct_get.RTC_WeekDay;
				air_condition_main.hour=RTC_TimeStruct_get.RTC_Hours;
				air_condition_main.minute=RTC_TimeStruct_get.RTC_Minutes;
				
				Flash_Write_UserData(air_condition_main);
			}
		}
		else if(xQueueReceive(ACSendQueue,&air_condition_main, 100/portTICK_RATE_MS )==pdPASS)
		{
			if(air_condition_main.air_fault==1)
			{
				//��������
			}
			else
			{
				air_condition_main.month=RTC_DateStruct_get.RTC_Month;
				air_condition_main.day=RTC_DateStruct_get.RTC_Date;
				air_condition_main.week=RTC_DateStruct_get.RTC_WeekDay;
				air_condition_main.hour=RTC_TimeStruct_get.RTC_Hours;
				air_condition_main.minute=RTC_TimeStruct_get.RTC_Minutes;
				
				Flash_Write_UserData(air_condition_main);
			}
		}
		else if(xQueueReceive(WifiSendQueue,&air_condition_main, 100/portTICK_RATE_MS )==pdPASS)
		{
			
		}
		else if(xQueueReceive(TouchSendQueue,&air_condition_main, 100/portTICK_RATE_MS )==pdPASS)
		{
			
		}
		else if(RTC_TimeStruct_get.RTC_Minutes==30||RTC_TimeStruct_get.RTC_Minutes==0)
		{
			
		}
	}
} 
/**
	* @brief Wifi task to communicate to the app
	* @param None
	* @retval None
	*/
void Wifi_task(void * pvParameters)
{
	while(1)
	{
		vTaskDelay(2000);
		uart_putstr("Wifi_task start.");
	}
}
/**
	* @brief Touch task to display the UI and communicate with the users
	* @param None
	* @retval None
	*/
void Touch_task(void * pvParameters)
{
	while(1)
	{
		vTaskDelay(2000);
		uart_putstr("Touch_task start.");
		
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStruct_get);
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStruct_get);
		uart_putchar(RTC_TimeStruct_get.RTC_Hours);
		uart_putchar(RTC_TimeStruct_get.RTC_Minutes);
		uart_putchar(RTC_TimeStruct_get.RTC_Seconds);
	}
}
/**
	* @brief DC task to communicate to air-condition by direct current
	* @param None
	* @retval None
	*/
void DC_task(void * pvParameters)
{
	while(1)
	{
		vTaskDelay(1500);
		uart_putstr("DC_task start.");
		xQueueSend(DCSendQueue, (void*)&air_condition_current, 0 );
	}
}
/**
	* @brief AC task to communicate to air-condition by alternating current
	* @param None
	* @retval None
	*/
void AC_task(void * pvParameters)
{
	while(1)
	{
		vTaskDelay(2000);
		uart_putstr("AC_task start.");
	}
}
//init the queue
void queue_init()
{
	WifiSendQueue = xQueueCreate(1,sizeof(air_condition_record));
	WifiReceQueue = xQueueCreate(1,sizeof(air_condition_record));
	DCSendQueue = xQueueCreate(1,sizeof(air_condition_record));
	DCReceQueue = xQueueCreate(1,sizeof(air_condition_record));
	ACSendQueue = xQueueCreate(1,sizeof(air_condition_record));
	ACReceQueue = xQueueCreate(1,sizeof(air_condition_record));
	TouchSendQueue = xQueueCreate(1,sizeof(air_condition_record));
	TouchReceQueue = xQueueCreate(1,sizeof(air_condition_record));
}
//1ms delay, not exectly 
void ms_Delay(uint32_t time)
{
	uint32_t i = 0, j = 0;
	for(i = 0; i < time; i++)
	{
		for(j = 0; j < 0x835E; j++)
		{;}
	}
}

//1us delay
void us_Delay(uint32_t time)
{
	uint32_t i = 0, j = 0;
	for(i = 0; i < time; i++)
	{
		for(j = 0; j < 0x25; j++)
		{;}
	}
}
