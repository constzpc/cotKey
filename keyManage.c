/**
  ******************************************************************************
  * @file    keyManage.c
  * @author  const_zpc
  * @date    2020-6-20
  * @brief   该文件提供功能模块层管理功能，以管理 KEY 驱动管理的以下功能：
  *           + 初始化函数
  *           + 扫描任务函数
  *
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "keyManage.h"
#include <string.h>

/** @addtogroup FunctionalModuleLayer
  * @brief      功能模块层
  * @{
  */

/** @addtogroup FML_KEY
  * @brief      KEY输入模块
  * @{
  */

/** @addtogroup KEY_MANAGE
  * @brief      KEY管理
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/**
  * @brief TIME 句柄信息结构体定义
  */
typedef struct{
    uint16_t ucSupportCnt;              /*!< 注册的定时回调函数的数目 */
    
    KeyFun tCallFun[KEY_MAX_NUM];       /*!< 注册的回调函数 */
} KEY_ManageHandleType;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static KEY_ManageHandleType sg_tKeyManageHandle = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**
  * @brief      KEY输入功能初始化函数.
  * @retval     None.
  */
void FML_KEY_ManageInit(void)
{
    FML_KEY_DriveInit();
    FML_KEY_ConfigInit();
    memset(&sg_tKeyManageHandle, 0, sizeof(sg_tKeyManageHandle));
}

/**
  * @brief      注册按键操作回调函数.
  * @note       注册的回调函数执行时间不能太长
  * @param[in]  emKeyName 指定按键
  * @param[in]  pKeyFun 回调函数
  * @retval     0,成功; -1,失败
  */
int FML_KEY_Register(KEY_NameTypeDef emKeyName, KeyFun pKeyFun)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        sg_tKeyManageHandle.tCallFun[emKeyName] = pKeyFun;
        return 0;
    }

    return -1;    
}

/**
  * @brief      注销按键操作回调函数.
  * @param[in]  emKeyName 指定按键
  * @retval     None
  */
void FML_KEY_UnRegister(KEY_NameTypeDef emKeyName)
{
    if (emKeyName < KEY_MAX_NUM)
    {
        sg_tKeyManageHandle.tCallFun[emKeyName] = NULL;
    }
}

/**
  * @brief      KEY输入扫描任务.
  * @note       10ms任务
  * @retval     None.
  */
void FML_KEY_ScanTask(void)
{
    int i;
    KEY_NameTypeDef keyName = SCH_KEY_1;
    int keyState;
    KEY_ActionTypeDef keyAction;
    
    FML_KEY_ConfigHandle(10);
    
    for (i = 0; i < KEY_MAX_NUM; i++)
    {
        keyState = FML_KEY_ReadAction(keyName);
        
        if (FML_KEY_CONFIRM_ACTION(keyState, KEY_FRIST_PRESS))
        {
            keyAction = KEY_IS_FRIST_PRESS;
        }
        else if (FML_KEY_CONFIRM_ACTION(keyState, KEY_SHORT_HOLD_PRESS))
        {
            keyAction = KEY_IS_HOLD_PRESS;
        }
        else if (FML_KEY_CONFIRM_ACTION(keyState, KEY_SHORT_LOOSEN))
        {
            keyAction = KEY_IS_LOOSEN;
        }
        else if (FML_KEY_CONFIRM_ACTION(keyState, KEY_LONG_HOLD_PRESS))
        {
            keyAction = KEY_IS_LONG_HOLD_PRESS;
        }
        else if (FML_KEY_CONFIRM_ACTION(keyState, KEY_LONG_LOOSEN))
        {
            keyAction = KEY_IS_LONG_LOOSEN;
        }
        else
        {
            keyAction = KEY_IS_NOT_PRESS;
        }
        
        if (sg_tKeyManageHandle.tCallFun[i] != NULL)
        {
            sg_tKeyManageHandle.tCallFun[i](keyName, keyAction);
        }
        
        keyName++;
    }
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
