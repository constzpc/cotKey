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
#define KEY_CTRL_LED3  KEY_8
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool g_10msFlag;
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**
  * @brief      处理控制led1按键动作回调函数.
  * @param[in]  emKeyName   按键.
  * @param[in]  emKeyAction 按键动作.
  * @retval     None.
  */
static void OnProcessCtrlLed1(KEY_NameTypeDef emKeyName, KEY_ActionTypeDef emKeyAction)
{
	if (emKeyAction == KEY_IS_LOOSEN)
	{
		LED1_CRTL = !LED1_CRTL;
	}
}

/**
  * @brief      处理按键动作回调函数.
  * @param[in]  emKeyName   按键.
  * @param[in]  emKeyAction 按键动作.
  * @retval     None.
  */
static void OnProcessKeyAction(KEY_NameTypeDef emKeyName, KEY_ActionTypeDef emKeyAction)
{
    switch (emKeyName)
    {
    case KEY_CTRL_LED2:
		if (emKeyAction == KEY_IS_LOOSEN)
		{
			LED2_CRTL = !LED2_CRTL;
		}
        break;
    case KEY_CTRL_LED3:
		if (emKeyAction == KEY_IS_LOOSEN)
		{
			LED3_CRTL = 1;
		}
		else
		{
			LED3_CRTL = 0;
		}
        break;
	default:
	
		break;
	}
}

int mian(void)
{
	uint8_t i;
	uint8_t keyVal[KEY_MAX_NUM];
	
	FML_KEY_ManageInit();
	
	FML_KEY_Register(KEY_CTRL_LED1, OnProcessCtrlLed1);
	FML_KEY_Register(KEY_CTRL_LED2, OnProcessKeyAction);
	FML_KEY_Register(KEY_CTRL_LED3, OnProcessKeyAction);
	
	while(1)
	{
		if (g_10msFlag)
		{
			FML_KEY_ScanTask();
			
			g_10msFlag = 0;
		}
	}
	
	FML_KEY_UnRegister(KEY_CTRL_LED1);
	FML_KEY_UnRegister(KEY_CTRL_LED2);
	FML_KEY_UnRegister(KEY_CTRL_LED3);
}


