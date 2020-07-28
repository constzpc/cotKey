/**
  ******************************************************************************
  * @file    key_core.c
  * @author  周鹏程
  * @version V2.0.0
  * @date    18-October-2019
  * @brief   该文件提供功能模块层管理功能，以管理KEY输入相关数据处理的以下功能：
  *           + 按键信息初始化函数
  *           + 按键信息处理函数
  *           + 读取按键动作函数
  *           + 设置/复位按键相关操作函数
  *
  ******************************************************************************
  * @note 版本更新说明
  * v2.0.0      18-October-2019
  * 	1.新增对指定按键扫描识别功能使能/禁止
  * 	2.新增检测指定按键是否存在动作
  *
  * v1.1.0		30-August-2019
  * 	1.优化文件模块架构, 优化底层驱动接口函数和核心管理代码模块
  * 	2.优化模块 API 接口函数
  * 	3.部分函数名及变量重命名
  * 	4.其他优化内容
  *
  * v1.0.0		22-January-2019
  * 	首版发布
  *
  ******************************************************************************
  * @attention
  * 无特殊要求或无其他BUG时该文件函数无需改动
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "key_core.h"
#include "key_cfg.h"

/** @addtogroup FunctionalModuleLayer
  * @brief      功能模块层
  * @{
  */

/** @addtogroup FML_KEY
  * @brief      KEY输入模块
  * @{
  */

/** @addtogroup KEY_Core
  * @brief      KEY输入的相关数据处理
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** 按键处理时间(单位: 1ms) */
#define KEY_HANDLE_TIME     10
      
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OneKeyShortModeHandle(KEY_ConfigInfoType * const ptKeyInfo);
static void OneKeyLongModeHandle(KEY_ConfigInfoType * const ptKeyInfo);

/* Private function ----------------------------------------------------------*/

/** @defgroup KEY_Core_Private_Functions
  * @{
  */

/** @defgroup KEY输入的核心驱动组1 KEY输入的信息初始化
 *  @brief    KEY输入的信息初始化
 *
 @verbatim
 ===============================================================================
                 ##### KEY输入的信息初始化 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      按键配置信息进行初始化.
  * @retval     None.
  */
void KEY_ConfigInfoInit(void)
{
    uint8_t i;

    for (i = 0; i < KEY_MAX_NUM; i++)
    {
        g_arrtKeyConfigInfo[i].pressTic = 0;
        g_arrtKeyConfigInfo[i].actionInfo = KEY_NOT_PRESS;
    }
}

/**
  * @}
  */

/** @defgroup KEY输入的核心驱动组2 KEY信息处函数
 *  @brief    KEY信息处函数
 *
 @verbatim
 ===============================================================================
                         ##### KEY信息处函数 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      对指定短按模式的按键信息进行处理.
  * @param[in]  ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针.
  * @param[out] ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针
  * @retval     None.
  */
static void OneKeyShortModeHandle(KEY_ConfigInfoType * const ptKeyInfo)
{
    KEY_StateTypeDef emKeyState = KEY_GetButtonInState(ptKeyInfo->emKeyName);

    if (ptKeyInfo->pressTic < ptKeyInfo->shortPressMaxTic)
    {
        if (KEY_IS_ON == emKeyState)
        {
            ptKeyInfo->pressTic += KEY_HANDLE_TIME;
        }
        else
        {
            ptKeyInfo->pressTic = 0;                        /* 未达到短按时间 */
        }
    }
    else
    {
        if (KEY_NOT_PRESS == ptKeyInfo->actionInfo)
        {
            ptKeyInfo->actionInfo |= KEY_FRIST_PRESS;
            ptKeyInfo->actionInfo |= KEY_SHORT_HOLD_PRESS;
        }
        else
        {
            if (KEY_IS_ON == emKeyState)
            {
                ptKeyInfo->actionInfo |= KEY_SHORT_HOLD_PRESS;
            }
            else
            {
                ptKeyInfo->pressTic = 0;
                ptKeyInfo->actionInfo |= KEY_SHORT_LOOSEN;
            }
        }
    }
}

/**
  * @brief      对指定长按模式的按键信息进行处理.
  * @param[in]  ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针.
  * @param[out] ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针
  * @retval     None.
  */
static void OneKeyLongModeHandle(KEY_ConfigInfoType * const ptKeyInfo)
{
    KEY_StateTypeDef emKeyState = KEY_GetButtonInState(ptKeyInfo->emKeyName);

    if (ptKeyInfo->pressTic < ptKeyInfo->shortPressMaxTic)
    {
        if (KEY_IS_ON == emKeyState)
        {
            ptKeyInfo->pressTic += KEY_HANDLE_TIME;
        }
        else
        {
            ptKeyInfo->pressTic = 0;                        /* 未满足短按时间 */
        }
    }
    else
    {
        if (KEY_NOT_PRESS == ptKeyInfo->actionInfo)
        {
            ptKeyInfo->actionInfo |= KEY_FRIST_PRESS;
            ptKeyInfo->actionInfo |= KEY_SHORT_HOLD_PRESS;
        }
        else
        {
            if (ptKeyInfo->pressTic < ptKeyInfo->longPressMaxTic)
            {
                if (KEY_IS_ON == emKeyState)
                {
                    ptKeyInfo->pressTic += KEY_HANDLE_TIME;
                }
                else
                {
                    ptKeyInfo->pressTic = 0;                /* 未满足长按时间 */
                    ptKeyInfo->actionInfo |= KEY_SHORT_LOOSEN;
                }
            }
            else
            {
                if (KEY_IS_ON == emKeyState)
                {
                    ptKeyInfo->actionInfo |= KEY_LONG_HOLD_PRESS;
                }
                else
                {
                    ptKeyInfo->pressTic = 0;
                    ptKeyInfo->actionInfo |= KEY_LONG_LOOSEN;
                }
            }
        }
    }
}

/**
  * @brief      对一个或多个指定的按键信息进行处理.
  * @warning    根据该函数的调用周期修改宏定义 KEY_HANDLE_TIME.
  * @retval     None.
  */
void KEY_ConfigInfoHandle(void)
{
    uint8_t i;
    KEY_ConfigInfoType *ptKeyInfo = g_arrtKeyConfigInfo;
    void (*pKeyModeHandle[2])(KEY_ConfigInfoType*) = {OneKeyShortModeHandle, OneKeyLongModeHandle};

    for (i = 0; i < KEY_MAX_NUM; i++)
    {
    	if (ptKeyInfo[i].funEnable)
    	{
            /* 根据按键长按模式禁止/使能选择不同判断依据 */
            pKeyModeHandle[ptKeyInfo[i].emLongMode](&ptKeyInfo[i]);
    	}
    	else
    	{
    		ptKeyInfo[i].actionInfo = KEY_NOT_PRESS;
    	}
    }
}

/**
  * @}
  */

/** @defgroup KEY输入的核心驱动组3 KEY读取按键动作函数
 *  @brief    KEY读取按键动作函数
 *
 @verbatim
 ===============================================================================
                         ##### KEY读取按键动作函数 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      检测指定按键是否存在动作.
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @retval     按键动作, 返回的值含义如下:
  *             @arg 0,无动作
  *             @arg 1,有动作
  */
uint8_t KEY_CheckIsAction(uint8_t index)
{
	if (g_arrtKeyConfigInfo[index].actionInfo != 0)
	{
		return 1;
	}

	return 0;
}

/**
  * @brief      读取指定按键的动作信息.
  * @note       1.若短按模式下按键完成一次动作, 即按下后松开, 则根据以下顺序每次调用该函数返回之前的动作信息：
  *                 首次按下 -> 短按持续按下 -> 短按松开(也适合长按模式下完成一次短按动作).
  *             2.若长按模式下按键完成一次动作, 即按下达到长按时间后松开, 则根据以下顺序每次调用该函数返回之前的动作信息：
  *                 首次按下 -> 短按持续按下 -> 长按持续按下 -> 长按松开.
  *             3.其中"持续按下"状态若调用过程中没有松开,则一直保持
  *             4.每次调用该函数读取后，需使用宏定义命令"KEY_CONFIRM_ACTION(keyAction, Action)"检测是否符合动作信息
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @retval     按键动作, 返回的值含义如下:
  *             @arg KEY_NOT_PRESS: 没有按下
  *             @arg KEY_FRIST_PRESS: 首次按下
  *             @arg KEY_SHORT_HOLD_PRESS: 短按持续按下
  *             @arg KEY_SHORT_LOOSEN: 短按松开
  *             @arg KEY_LONG_HOLD_PRESS: 长按持续按下(仅长按模式有效)
  *             @arg KEY_LONG_LOOSEN: 长按松开(仅长按模式有效)
  */
uint8_t KEY_ReadAction(uint8_t index)
{
    uint8_t state;
    KEY_ConfigInfoType *ptKeyInfo = &g_arrtKeyConfigInfo[index];

    if (ptKeyInfo->actionInfo & KEY_FRIST_PRESS)
    {
        state = KEY_FRIST_PRESS;
        ptKeyInfo->actionInfo &= ~KEY_FRIST_PRESS;
    }
    else if (ptKeyInfo->actionInfo & KEY_SHORT_HOLD_PRESS)
    {
        state = KEY_SHORT_HOLD_PRESS;

        if ((ptKeyInfo->actionInfo & KEY_SHORT_LOOSEN) || (ptKeyInfo->actionInfo & KEY_LONG_LOOSEN) ||
                (ptKeyInfo->actionInfo & KEY_LONG_HOLD_PRESS))
        {
            ptKeyInfo->actionInfo &= ~KEY_SHORT_HOLD_PRESS;
        }
    }
    else if (ptKeyInfo->actionInfo & KEY_LONG_HOLD_PRESS)
    {
        state = KEY_LONG_HOLD_PRESS;

        if ((ptKeyInfo->actionInfo & KEY_SHORT_LOOSEN) || (ptKeyInfo->actionInfo & KEY_LONG_LOOSEN))
        {
            ptKeyInfo->actionInfo &= ~KEY_LONG_HOLD_PRESS;
        }
    }
    else if ((ptKeyInfo->actionInfo & KEY_SHORT_LOOSEN) || (ptKeyInfo->actionInfo & KEY_LONG_LOOSEN))
    {
        state = (ptKeyInfo->actionInfo & KEY_SHORT_LOOSEN) ? KEY_SHORT_LOOSEN : KEY_LONG_LOOSEN;
        ptKeyInfo->actionInfo = KEY_NOT_PRESS;
    }
    else
    {
        state = KEY_NOT_PRESS;
    }

    return state;
}

/**
  * @}
  */

/** @defgroup KEY输入的核心驱动组4 KEY 设置/复位按键相关操作函数
 *  @brief    设置/复位按键相关操作函数
 *
 @verbatim
 ===============================================================================
                         ##### KEY 设置/复位按键相关操作函数 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      改变指定按键的短按有效时间.
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @param[in]  emMode - 按键模式.
  * @retval     None.
  */
void KEY_ChangeMode(uint8_t index, KEY_ModeTypeDef  emMode)
{
    g_arrtKeyConfigInfo[index].emLongMode = emMode;
}

/**
  * @brief      改变指定按键的短按有效时间.
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @param[in]  time - 按下有效时间, 单位: 1ms.
  * @retval     None.
  */
void KEY_ChangeShortTime(uint8_t index, uint16_t time)
{
    g_arrtKeyConfigInfo[index].shortPressMaxTic = time;
}

/**
  * @brief      改变指定按键的长按有效时间.
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @param[in]  time - 按下有效时间, 单位: 1ms.
  * @retval     None.
  */
void KEY_ChangeLongTime(uint8_t index, uint16_t time)
{
    g_arrtKeyConfigInfo[index].longPressMaxTic = time;
}

/**
  * @brief      复位指定按键的动作.
  * @note       复位后, 按键的动作信息会被清除
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @retval     None.
  */
void KEY_ResetAction(uint8_t index)
{
    g_arrtKeyConfigInfo[index].actionInfo = KEY_NOT_PRESS;
    g_arrtKeyConfigInfo[index].pressTic   = 0;
}

/**
  * @brief      使能/禁止所有按键的动作识别.
  * @param[in]  index - g_arrtKeyConfigInfo 的索引值.
  * @param[in]  enable - 使能(1)/禁止(0).
  * @retval     None.
  */
void KEY_EnableScan(uint8_t index, uint8_t enable)
{
	g_arrtKeyConfigInfo[index].funEnable = enable;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

