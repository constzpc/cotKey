/**
  ******************************************************************************
  * @file    keyDrive.c
  * @author  const_zpc
  * @date    2020-6-20
  * @brief   该文件提供功能模块层管理功能，以管理 KEY 输入驱动处理的以下功能：
  *           + KEY输入的硬件设置
  *           + 按键信息初始化函数
  *           + 按键信息处理函数
  *           + 读取按键动作函数
  *           + 设置/复位按键相关操作函数
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "keyDrive.h"
#include "MPC5604B_Extend.h"
#include "MPC5604B.h"
#include <string.h>

/** @addtogroup FunctionalModuleLayer
  * @brief      功能模块层
  * @{
  */

/** @addtogroup FML_KEY
  * @brief      KEY 输入模块
  * @{
  */

/** @addtogroup KEY_DRIVE
  * @brief      KEY 驱动
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/**
  * @brief 按键配置信息结构体定义
  */
typedef struct{
    KEY_ModeTypeDef emMode;                     /*!< 指定所选按键长按模式的禁用/使能
                                                     This parameter can be any value of @ref KEY_ModeTypeDef */

    KEY_StateTypeDef emKeyState;                /*!< 数字量开关的电平状态 */
    
    uint8_t uiAction;                           /*!< 指定所选按键的动作信息 */

    uint16_t uiPressTic;                        /*!< 指定所选按键的按下累计时间, 该参数单位为1ms,该值初值为0 */

    uint16_t uiShortPressTime;                  /*!< 指定所选按键的短按有效时间, 该参数单位为1ms */

    uint16_t uiLongPressTime;                   /*!< 指定所选按键的长按有效时间(长按模式禁止时无效), 该参数单位为1ms */
} KEY_ConfigInfoType;

/**
  * @brief 按键句柄信息结构体定义
  */
typedef struct{
    KEY_ConfigInfoType tKeysConfigInfo[KEY_MAX_NUM];        /*!< 所有按键配置信息指针 */
} KEY_DriveHandleType;

/* Private define ------------------------------------------------------------*/

/** 按键通用配置定义 */
#define KEY_SHORT_TIME     20
#define KEY_LONG_TIME      1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static KEY_DriveHandleType sg_tKeyDrvHandle = {0};

/* Private function prototypes -----------------------------------------------*/
static void SetOutputPin(PORT_PadTypeDef port);
static void SacnKeyDriveState(void);
static void OneKeyShortModeHandle(KEY_ConfigInfoType * const ptKeyInfo, uint8_t callTime);
static void OneKeyLongModeHandle(KEY_ConfigInfoType * const ptKeyInfo, uint8_t callTime);

/* Private function ----------------------------------------------------------*/

/** @defgroup KEY 输入的驱动组1 KEY 输入的硬件设置
 *  @brief    KEY 输入的硬件设置
 *
 @verbatim
 ===============================================================================
                 ##### KEY 输入的硬件设置  #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      KEY 硬件驱动的初始化.
  * @retval     -1,错误; 0,成功.
  */
void FML_KEY_DriveInit(void)
{
    SIU.PCR[PORT_C5].R = 0x0102;
    SIU.PCR[PORT_C2].R = 0x0102;
    SIU.PCR[PORT_C3].R = 0x0102;
    SIU.PCR[PORT_C4].R = 0x0102;
    SIU.PCR[PORT_A1].R = 0x0203;
    SIU.PCR[PORT_A2].R = 0x0203;
    SIU.PCR[PORT_A3].R = 0x0203;
    SIU.PCR[PORT_A4].R = 0x0203;
}

/**
  * @brief      矩阵按键中指定IO口拉高.
  * @param[in]  port 需要拉高的 IO 口.
  * @retval     None.
  */
static void SetOutputPin(PORT_PadTypeDef port)
{
    SIU.GPDO[PORT_A1].B.PDO = 0;
    SIU.GPDO[PORT_A2].B.PDO = 0;
    SIU.GPDO[PORT_A3].B.PDO = 0;
    SIU.GPDO[PORT_A4].B.PDO = 0;
    
    SIU.GPDO[port].B.PDO = 1;
}

/**
  * @brief      扫描所有按键的开关状态.
  * @retval     None.
  */
static void SacnKeyDriveState(void)
{
    uint8_t i;
    KEY_ConfigInfoType *ptKeyInfo = sg_tKeyDrvHandle.tKeysConfigInfo;
    PORT_PadTypeDef emPort[] = {PORT_A1, PORT_A2, PORT_A3, PORT_A4};
    
    for (i = 0; i < KEY_MAX_NUM; i++)
    {
        ptKeyInfo[i].emKeyState = KEY_IS_OFF;
    }
    
    for (i = 0; i < 4; i++)
    {
        SetOutputPin(emPort[i]);
        
        if (SIU.GPDI[PORT_C5].B.PDI)
        {
            ptKeyInfo[SCH_KEY_1 + i * 4].emKeyState = KEY_IS_ON;
        }
        if (SIU.GPDI[PORT_C2].B.PDI)
        {
            ptKeyInfo[SCH_KEY_2 + i * 4].emKeyState = KEY_IS_ON;
        }
        if (SIU.GPDI[PORT_C3].B.PDI)
        {
            ptKeyInfo[SCH_KEY_3 + i * 4].emKeyState = KEY_IS_ON;
        }
        if (SIU.GPDI[PORT_C4].B.PDI)
        {
            ptKeyInfo[SCH_KEY_4 + i * 4].emKeyState = KEY_IS_ON;
        }
    }
}

/**
  * @}
  */

/** @defgroup KEY 输入的驱动组2 KEY 配置信息处理
 *  @brief    KEY 配置信息处理
 *
 @verbatim
 ===============================================================================
                 ##### KEY 配置信息处理 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      按键配置信息进行初始化.
  * @param[in]  pInfo - 所有按键的配置信息, 指向"KEY_ConfigInfoType"结构体类型的指针.
  * @param[in]  num -   配置后的按键数目.
  * @retval     None.
  */
void FML_KEY_ConfigInit(void)
{
    uint8_t i;
    
    memset(&sg_tKeyDrvHandle, 0, sizeof(sg_tKeyDrvHandle));
    
    for (i = 0; i < KEY_MAX_NUM; i++)
    {
        sg_tKeyDrvHandle.tKeysConfigInfo[i].uiLongPressTime = KEY_LONG_TIME;
        sg_tKeyDrvHandle.tKeysConfigInfo[i].uiShortPressTime = KEY_SHORT_TIME;
        sg_tKeyDrvHandle.tKeysConfigInfo[i].emMode = KEY_NORMAL_MODE;
        sg_tKeyDrvHandle.tKeysConfigInfo[i].uiPressTic = 0;
        sg_tKeyDrvHandle.tKeysConfigInfo[i].uiAction = KEY_NOT_PRESS;
        sg_tKeyDrvHandle.tKeysConfigInfo[i].emKeyState = KEY_IS_OFF;
    }
}

/**
  * @brief      对指定短按模式的按键信息进行处理.
  * @param[in][out]  ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针.
  * @param[in]  callTime - 该函数调用周期时间, 单位ms.
  * @retval     None.
  */
static void OneKeyShortModeHandle(KEY_ConfigInfoType * const ptKeyInfo, uint8_t callTime)
{
    if (ptKeyInfo->uiPressTic < ptKeyInfo->uiShortPressTime)
    {
        if (KEY_IS_ON == ptKeyInfo->emKeyState)
        {
            ptKeyInfo->uiPressTic += callTime;
        }
        else
        {
            ptKeyInfo->uiPressTic = 0;                        /* 未达到短按时间 */
        }
    }
    else
    {
        if (KEY_NOT_PRESS == ptKeyInfo->uiAction)
        {
            ptKeyInfo->uiAction |= KEY_FRIST_PRESS;
            ptKeyInfo->uiAction |= KEY_SHORT_HOLD_PRESS;
        }
        else
        {
            if (KEY_IS_ON == ptKeyInfo->emKeyState)
            {
                ptKeyInfo->uiAction |= KEY_SHORT_HOLD_PRESS;
            }
            else
            {
                ptKeyInfo->uiPressTic = 0;
                ptKeyInfo->uiAction |= KEY_SHORT_LOOSEN;
            }
        }
    }
}

/**
  * @brief      对指定长按模式的按键信息进行处理.
  * @param[in][out]  ptKeyInfo - 指向"KEY_OperaInfoType"结构体类型的指针.
  * @param[in]  callTime - 该函数调用周期时间, 单位ms.
  * @retval     None.
  */
static void OneKeyLongModeHandle(KEY_ConfigInfoType * const ptKeyInfo, uint8_t callTime)
{
    if (ptKeyInfo->uiPressTic < ptKeyInfo->uiShortPressTime)
    {
        if (KEY_IS_ON == ptKeyInfo->emKeyState)
        {
            ptKeyInfo->uiPressTic += callTime;
        }
        else
        {
            ptKeyInfo->uiPressTic = 0;                        /* 未满足短按时间 */
        }
    }
    else
    {
        if (KEY_NOT_PRESS == ptKeyInfo->uiAction)
        {
            ptKeyInfo->uiAction |= KEY_FRIST_PRESS;
            ptKeyInfo->uiAction |= KEY_SHORT_HOLD_PRESS;
        }
        else
        {
            if (ptKeyInfo->uiPressTic < ptKeyInfo->uiLongPressTime)
            {
                if (KEY_IS_ON == ptKeyInfo->emKeyState)
                {
                    ptKeyInfo->uiPressTic += callTime;
                }
                else
                {
                    ptKeyInfo->uiPressTic = 0;                /* 未满足长按时间 */
                    ptKeyInfo->uiAction |= KEY_SHORT_LOOSEN;
                }
            }
            else
            {
                if (KEY_IS_ON == ptKeyInfo->emKeyState)
                {
                    ptKeyInfo->uiAction |= KEY_LONG_HOLD_PRESS;
                }
                else
                {
                    ptKeyInfo->uiPressTic = 0;
                    ptKeyInfo->uiAction |= KEY_LONG_LOOSEN;
                }
            }
        }
    }
}

/**
  * @brief      对一个或多个指定的按键信息进行处理.
  * @param[in]  callTime - 该函数调用周期时间, 单位ms.
  * @retval     None.
  */
void FML_KEY_ConfigHandle(uint8_t callTime)
{
    uint8_t i;
    KEY_ConfigInfoType *ptKeyInfo = sg_tKeyDrvHandle.tKeysConfigInfo;
    void (*pKeyModeHandle[2])(KEY_ConfigInfoType*, uint8_t) = {OneKeyShortModeHandle, OneKeyLongModeHandle};

    SacnKeyDriveState();
    
    for (i = 0; i < KEY_MAX_NUM; i++)
    {
        pKeyModeHandle[ptKeyInfo[i].emMode](&ptKeyInfo[i], callTime);
    }
}

/**
  * @}
  */

/** @defgroup KEY 输入的驱动组3 KEY 功能设置/获取
 *  @brief    KEY 功能设置/获取
 *
 @verbatim
 ===============================================================================
                         ##### KEY 功能设置/获取 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      检测指定按键是否存在动作.
  * @param[in]  emKeyName 指定按键
  * @retval     按键动作, 返回的值含义如下:
  * 			@arg -1,错误
  *             @arg 0,无动作
  *             @arg 1,有动作
  */
int FML_KEY_CheckIsAction(KEY_NameTypeDef emKeyName)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        if (sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].uiAction != 0)
        {
            return 1;
        }

        return 0;
    }
    
    return -1;
}

/**
  * @brief      读取指定按键的动作信息.
  * @note       1.若短按模式下按键完成一次动作, 即按下后松开, 则根据以下顺序每次调用该函数返回之前的动作信息：
  *                 首次按下 -> 短按持续按下 -> 短按松开(也适合长按模式下完成一次短按动作).
  *             2.若长按模式下按键完成一次动作, 即按下达到长按时间后松开, 则根据以下顺序每次调用该函数返回之前的动作信息：
  *                 首次按下 -> 短按持续按下 -> 长按持续按下 -> 长按松开.
  *             3.其中"持续按下"状态若调用过程中没有松开,则一直保持
  *             4.每次调用该函数读取后，需使用宏定义命令"KEY_CONFIRM_ACTION(keyAction, Action)"检测是否符合动作信息
  * @param[in]  emKeyName 指定按键
  * @retval     按键动作, 返回的值含义如下:
  *             @arg -1,错误
  *             @arg KEY_NOT_PRESS,没有按下
  *             @arg KEY_FRIST_PRESS,首次按下
  *             @arg KEY_SHORT_HOLD_PRESS,短按持续按下
  *             @arg KEY_SHORT_LOOSEN,短按松开
  *             @arg KEY_LONG_HOLD_PRESS,长按持续按下(仅长按模式有效)
  *             @arg KEY_LONG_LOOSEN,长按松开(仅长按模式有效)
  */
int FML_KEY_ReadAction(KEY_NameTypeDef emKeyName)
{
    uint8_t state;
    KEY_ConfigInfoType *ptKeyInfo = &sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName];

    if (emKeyName < KEY_MAX_NUM)
    {
        if (ptKeyInfo->uiAction & KEY_FRIST_PRESS)
        {
            state = KEY_FRIST_PRESS;
            ptKeyInfo->uiAction &= ~KEY_FRIST_PRESS;
        }
        else if (ptKeyInfo->uiAction & KEY_SHORT_HOLD_PRESS)
        {
            state = KEY_SHORT_HOLD_PRESS;

            if ((ptKeyInfo->uiAction & KEY_SHORT_LOOSEN) || (ptKeyInfo->uiAction & KEY_LONG_LOOSEN) ||
                            (ptKeyInfo->uiAction & KEY_LONG_HOLD_PRESS))
            {
                    ptKeyInfo->uiAction &= ~KEY_SHORT_HOLD_PRESS;
            }
        }
        else if (ptKeyInfo->uiAction & KEY_LONG_HOLD_PRESS)
        {
            state = KEY_LONG_HOLD_PRESS;

            if ((ptKeyInfo->uiAction & KEY_SHORT_LOOSEN) || (ptKeyInfo->uiAction & KEY_LONG_LOOSEN))
            {
                    ptKeyInfo->uiAction &= ~KEY_LONG_HOLD_PRESS;
            }
        }
        else if ((ptKeyInfo->uiAction & KEY_SHORT_LOOSEN) || (ptKeyInfo->uiAction & KEY_LONG_LOOSEN))
        {
            state = (ptKeyInfo->uiAction & KEY_SHORT_LOOSEN) ? KEY_SHORT_LOOSEN : KEY_LONG_LOOSEN;
            ptKeyInfo->uiAction = KEY_NOT_PRESS;
        }
        else
        {
            state = KEY_NOT_PRESS;
        }
        
        return state;
    }

    return -1;
}

/**
  * @brief      改变指定按键模式.
  * @param[in]  emKeyName 指定按键
  * @param[in]  emMode - 按键模式.
  * @retval     -1,错误; 0,成功.
  */
int FML_KEY_ChangeMode(KEY_NameTypeDef emKeyName, KEY_ModeTypeDef  emMode)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].emMode = emMode;
        return 0;
    }
    
    return -1;
}

/**
  * @brief      改变指定按键的有效时间.
  *             longTime 仅在支持长按模式下有效
  * @param[in]  emKeyName 指定按键
  * @param[in]  shortTime 短按有效时间, 单位: 1ms.
  * @param[in]  longTime  长按有效时间, 单位: 1ms.
  * @retval     -1,错误; 0,成功.
  */
int FML_KEY_ChangeTime(KEY_NameTypeDef emKeyName, uint16_t shortTime, uint16_t longTime)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].uiShortPressTime = shortTime;
        sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].uiLongPressTime = longTime;
        return 0;
    }
    
    return -1;
}

/**
  * @brief      复位指定按键的动作.
  *         	复位后, 按键的动作信息会被清除
  * @param[in]  emKeyName 指定按键.
  * @retval     -1,错误; 0,成功.
  */
int FML_KEY_ResetAction(KEY_NameTypeDef emKeyName)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].uiAction = KEY_NOT_PRESS;
        sg_tKeyDrvHandle.tKeysConfigInfo[emKeyName].uiPressTic   = 0;
        return 0;
    }
    
    return -1;
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
