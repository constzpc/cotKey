/**
  ******************************************************************************
  * @file    key_cfg.h
  * @author  周鹏程
  * @date    30-August-2019
  * @brief   该文件包含KEY输入相关数据配置的所有函数原型
  ******************************************************************************
  * @attention
  *
  * 暂无
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SOURCE_FML_KEY_KEY_CFG_H_
#define SOURCE_FML_KEY_KEY_CFG_H_

/* Includes ------------------------------------------------------------------*/
#include "key_drv.h"

/** @addtogroup FML_KEY
  * @{
  */

/** @addtogroup KEY_Cfg
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  按键模式选择枚举
  * @note   长按模式：区分短按和长按两种状态，并不是时间长短
  */
typedef enum{
    KEY_SHORT_MODE = 0,                         /*!< (0)短按按模式(默认) */
    KEY_LONG_MODE,                              /*!< (1)长按模式 */
}KEY_ModeTypeDef;

/**
  * @brief 按键配置信息结构体定义
  */
typedef struct{
    KEY_NameTypeDef emKeyName;                  /*!< 指定(模拟)输入按键名称枚举定义 */

    KEY_ModeTypeDef  emLongMode;                /*!< 指定所选按键长按模式的禁用/使能
                                                     This parameter can be any value of @ref KEY_ModeTypeDef */

    uint8_t actionInfo;                         /*!< 指定所选按键的动作信息 */

    uint8_t funEnable;                          /*!< 指定所选按键的功能使能(1)/禁止(0) */

    uint16_t  pressTic;                         /*!< 指定所选按键的按下累计时间
                                                     该参数单位为1ms,该值初值为0 */

    uint16_t shortPressMaxTic;                  /*!< 指定所选按键的短按有效时间
                                                     该参数单位为1ms,但该值为 KEY_ConfigInfoHandle 函数调用周期 的整数倍 */

    uint16_t longPressMaxTic;                   /*!< 指定所选按键的长按有效时间(长按模式禁止时无效)
                                                     该参数单位为1ms,但该值为 KEY_ConfigInfoHandle 函数调用周期 的整数倍 */
} KEY_ConfigInfoType;

/* Exported constants --------------------------------------------------------*/

/* 按键的动作信息 */
#define KEY_NOT_PRESS           ((uint8_t)0x00) /* 没有按下 */
#define KEY_FRIST_PRESS         ((uint8_t)0x01) /* 首次按下 */
#define KEY_SHORT_HOLD_PRESS    ((uint8_t)0x02) /* 短按持续按下 */
#define KEY_SHORT_LOOSEN        ((uint8_t)0x04) /* 短按松开 */
#define KEY_LONG_HOLD_PRESS     ((uint8_t)0x08) /* 长按持续按下 */
#define KEY_LONG_LOOSEN         ((uint8_t)0x10) /* 长按松开 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern KEY_ConfigInfoType g_arrtKeyConfigInfo[KEY_MAX_NUM];

/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */


#endif /* SOURCE_FML_KEY_KEY_CFG_H_ */
