/**
  ******************************************************************************
  * @file    main.c
  * @author  const-zpc
  * @date    30-August-2019
  * @brief   1、按键实例用法
  *
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "key_user.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_CTRL_LED1  KEY_2
#define KEY_CTRL_LED2  KEY_5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool g_10msFlag;
extern bool g_20msFlag;
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

void AnalyzeKeyAction(void)
{
	for (i = 0; i < KEY_MAX_NUM; i++)
	{
		keyVal[i] = KEY_ReadAction(i);
	}
	
	if (KEY_CONFIRM_ACTION(keyVal[KEY_CTRL_LED1], KEY_SHORT_LOOSEN))
	{
		LED1_CRTL = !LED1_CRTL;
	}
	
	if (KEY_CONFIRM_ACTION(keyVal[KEY_CTRL_LED2], KEY_SHORT_HOLD_PRESS))
	{
		LED2_CRTL = 1;
	}
	else
	{
		LED2_CRTL = 0;
	}
}

int mian(void)
{
	uint8_t i;
	uint8_t keyVal[KEY_MAX_NUM];
	
	KEY_UserInit();
	
	while(1)
	{
		if (g_10msFlag)
		{
			KEY_ScanTask();
			
			g_10msFlag = 0;
		}
		
		/* 支持按键异步处理 */
		if (g_20msFlag)
		{
			AnalyzeKeyAction();
			
			g_20msFlag = 0;
		}
	}
}


