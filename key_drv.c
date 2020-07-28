/**
  ******************************************************************************
  * @file    key_drv.c
  * @author  周鹏程
  * @date    30-August-2019
  * @brief   该文件提供功能模块层管理功能，以管理KEY输入硬件接口定义的以下功能：
  *           + (模拟)输入所有按键的开关状态
  *           + 获取指定(模拟)输入按键的开关状态
  *
  ******************************************************************************
  * @attention
  * 当改变硬件电路时需修改该文件中的函数 KEY_AllButtonInScan
  * 不提供单片机内相关硬件资源的初始化
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "key_drv.h"

// HAL
#include "pal.h"

/** @addtogroup FunctionalModuleLayer
  * @brief      功能模块层
  * @{
  */

/** @addtogroup FML_KEY
  * @brief      KEY输入模块
  * @{
  */

/** @addtogroup KEY_Drv
  * @brief      KEY输入的硬件接口定义
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* 所有数字量开关的电平信息 */
static KEY_StateTypeDef sg_arrKeyState[KEY_MAX_NUM];

/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/** @defgroup KEY_Drv_Private_Functions
  * @{
  */

/** @defgroup KEY输入的硬件驱动组1 (模拟)输入所有按键的开关状态
 *  @brief    (模拟)输入所有按键的开关状态
 *
 @verbatim
 ===============================================================================
                 ##### (模拟)输入所有按键的开关状态 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      KEY驱动的初始化.
  * @retval     0, 成功; 1,失败.
  */
uint8_t KEY_Initialize(void)
{
    uint8_t res = 0;
    

    return res;
}

/**
  * @brief      (模拟)输入所有按键的开关状态.
  * @note       模拟: 不是IO直接输入,而是其他方式输入,可通过输入的信号间接决定该按键的开关状态.
  * @param      None.
  * @retval     None.
  */
void KEY_AllButtonInScan(void)
{
    /* 开关按下为低电平, 因此取反 */
    sg_arrKeyState[KEY_1] = (KEY_StateTypeDef)!pal_readpad(PORT_A, 7);
    sg_arrKeyState[KEY_2] = (KEY_StateTypeDef)!pal_readpad(PORT_A, 3);
    sg_arrKeyState[KEY_3] = (KEY_StateTypeDef)!pal_readpad(PORT_A, 8);
    sg_arrKeyState[KEY_4] = (KEY_StateTypeDef)!pal_readpad(PORT_D, 14);
    sg_arrKeyState[KEY_5] = (KEY_StateTypeDef)!pal_readpad(PORT_D, 9);
    sg_arrKeyState[KEY_6] = (KEY_StateTypeDef)!pal_readpad(PORT_B, 15);
    sg_arrKeyState[KEY_7] = (KEY_StateTypeDef)!pal_readpad(PORT_E, 14);
    sg_arrKeyState[KEY_8] = (KEY_StateTypeDef)!pal_readpad(PORT_A, 14);
    sg_arrKeyState[KEY_9] = (KEY_StateTypeDef)!pal_readpad(PORT_B, 6);
}

/**
  * @}
  */

/** @defgroup KEY输入的硬件驱动组2 获取指定输入/模拟按键的原始状态
 *  @brief    获取指定输入/模拟按键的原始状态
 *
 @verbatim
 ===============================================================================
                 ##### 获取指定输入/模拟按键的原始状态 #####
 ===============================================================================

 @endverbatim
  * @{
  */

/**
  * @brief      获取指定(模拟)输入按键的开关状态.
  * @note       该状态没有经过滤波等处理
  * @param[in]  key - 该值为"KEY_NameTypeDef"定义的枚举值.
  * @retval     输入/模拟按键的原始状态(无动作, 有动作).
  */
KEY_StateTypeDef KEY_GetButtonInState(KEY_NameTypeDef emKeyName)
{
    return sg_arrKeyState[emKeyName];
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

