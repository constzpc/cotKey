/**
  ******************************************************************************
  * @file    main.c
  * @author  const-zpc
  * @date    30-August-2019
  * @brief   1������ʵ���÷�
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
  * @brief      ���������¼�����ص�����(���� KEY_CTRL_LED1 �� KEY_CTRL_LED2).
  * @param[in]  eKey    ����.
  * @param[in]  eAction ��������.
  * @retval     None.
  */
void OnKeyActionProess(IoKeyType_e eKey, KeyAction_e eAction)
{
	/* ����: �������(ÿ�ε�����ʱ��С��100ms)����3�������LED1, �����������2����Ϩ��LED1, �������1����תLED1 */
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
	
	/* �����ɿ�3�������ٰ��������LED2, ����Ϩ��LED2 */
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
  * @brief      ���� LED3 ���������¼�����ص�����
  * @param[in]  eKey    ����.
  * @param[in]  eAction ��������.
  * @retval     None.
  */
void OnLed3KeyActionProess(IoKeyType_e eKey, KeyAction_e eAction)
{
	if (eKey != KEY_CTRL_LED3)
	{
		return;
	}

	/* ����: ����3�����������LED3������ת */
    if (KEY_ACTION_PRESS == eAction)
    {
		KEY_RefreshNotify(eKey, 3000); // 3�����Ҫ�ٴδ�������֪ͨ�ص�
		
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


