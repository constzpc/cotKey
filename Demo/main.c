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
#include "key.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_CTRL_LED1  IO_KEY_0
#define KEY_CTRL_LED2  IO_KEY_1
#define KEY_CTRL_LED3  IO_KEY_WK_UP
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern bool g_10msFlag;
/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/


/**
  * @brief      按键动作事件处理回调函数(处理 KEY_CTRL_LED1 和 KEY_CTRL_LED2).
  * @param[in]  eKey    按键.
  * @param[in]  eAction 按键动作.
  * @retval     None.
  */
void OnKeyActionProess(IoKeyType_e eKey, KeyAction_e eAction)
{
	/* 作用: 连续点击(每次点击间隔时间小于100ms)按键3次则点亮LED1, 连续点击按键2次则熄灭LED1, 点击按键1次则翻转LED1 */
	if (eKey == KEY_CTRL_LED1)
	{
		uint8_t cnt = KEY_GetClickCnt(eKey, 100);
		
		if (cnt == 3)
		{
			LED1_CRTL = 1;
		}
		else if (cnt == 2)
		{
			LED1_CRTL = 0;
		}
		else if (cnt == 1)
		{
			LED1_CRTL = !LED1_CRTL;
		}
	}
	
	/* 按键松开3秒以上再按下则点亮LED2, 否则熄灭LED2 */
	if (eKey == KEY_CTRL_LED2)
	{
		if (KEY_ACTION_PRESS == eAction)
		{
			if (KEY_GetLossenTime(eKey) > 3000)
			{
				LED2_CRTL = 1;
			}
			else
			{
				LED2_CRTL = 0;
			}
			
		}
	}
}

/**
  * @brief      控制 LED3 按键动作事件处理回调函数
  * @param[in]  eKey    按键.
  * @param[in]  eAction 按键动作.
  * @retval     None.
  */
void OnLed3KeyActionProess(IoKeyType_e eKey, KeyAction_e eAction)
{
	if (eKey != KEY_CTRL_LED3)
	{
		return;
	}

	/* 作用: 长按3秒以上则点亮LED3，否则翻转 */
    if (KEY_ACTION_PRESS == eAction)
    {
		KEY_RefreshNotify(eKey, 3000); // 3秒后需要再次触发按键通知回调
		
        if (KEY_GetPressTime(eKey) >= 3000)
        {
            LED3_CRTL = 1;
        }
    }
    else if (KEY_ACTION_LOSSEN == eAction)
    {
        if (KEY_GetPressTime(eKey) < 3000)
        {
            LED3_CRTL = !LED3_CRTL;
        }
    }
}


int mian(void)
{	
    KEY_Init();

    KEY_Register(KEY_CTRL_LED1, OnKeyActionProess);
    KEY_Register(KEY_CTRL_LED2, OnKeyActionProess);
    KEY_Register(KEY_CTRL_LED3, OnLed3KeyActionProess);
	
	while(1)
	{
		if (g_10msFlag)
		{
			KEY_Scan(10);
			
			g_10msFlag = 0;
		}
	}
}


