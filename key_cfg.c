/**
  ******************************************************************************
  * @file    key_cfg.c
  * @author  周鹏程
  * @date    30-August-2019
  * @brief   该文件提供功能模块层管理功能，以管理KEY输入相关数据配置的以下功能：
  *           + 开关按键相关信息配置
  *
  *
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "key_cfg.h"

/** @addtogroup FunctionalModuleLayer
  * @brief      功能模块层
  * @{
  */

/** @addtogroup FML_KEY
  * @brief      KEY输入模块
  * @{
  */

/** @addtogroup KEY_Cfg
  * @brief      KEY输入的相关数据配置
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  按键的配置信息.
  * @note   按键名称的枚举值必须和数值索引值对应
  */
KEY_ConfigInfoType g_arrtKeyConfigInfo[KEY_MAX_NUM] = {
    {KEY_1, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_2, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_3, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_4, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_5, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_6, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_7, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_8, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
    {KEY_9, KEY_SHORT_MODE, KEY_NOT_PRESS, 1, 0, 20, 0},
};

/* Private function prototypes -----------------------------------------------*/
/* Private function ----------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
