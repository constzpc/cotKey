/**
  ******************************************************************************
  * @file    keyDrive.h
  * @author  const_zpc
  * @date    2020-6-20
  * @brief   该文件包含 KEY 输入驱动处理的所有函数原型
  ******************************************************************************
  * @attention
  *
  * 暂无
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SOURCE_FML_KEY_DRIVE_H_
#define SOURCE_FML_KEY_DRIVE_H_

/* Includes ------------------------------------------------------------------*/
#include "typedefs.h"

/** @addtogroup FML_KEY
  * @{
  */

/** @addtogroup KEY_DRIVE
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  输入/模拟按键名称枚举定义
  * @note   可参考原理图中的部分命名
  */
typedef enum{
    SCH_KEY_1 = 0,                                  /*!< (0)原理图上的KEY1 */
    SCH_KEY_2,
    SCH_KEY_3,
    SCH_KEY_4,
    SCH_KEY_5,
    SCH_KEY_6,
    SCH_KEY_7,
    SCH_KEY_8,
    SCH_KEY_9,
    SCH_KEY_10,
    SCH_KEY_11,
    SCH_KEY_12,
    SCH_KEY_13,
    SCH_KEY_14,
    SCH_KEY_15,
    SCH_KEY_16,
    
    /* 勿删除,用来统计按键的数目 */
    KEY_MAX_NUM
} KEY_NameTypeDef;

/**
  * @brief  KEY开关枚举定义
  */
typedef enum{
    KEY_IS_OFF = 0,                         	/*!< (0)断开(无动作) */
    KEY_IS_ON = !KEY_IS_OFF                     /*!< (1)闭合(有动作) */
} KEY_StateTypeDef;

/**
  * @brief  按键模式选择枚举
  * @note   长按模式：区分短按和长按两种状态，并不是时间长短
  */
typedef enum{
    KEY_NORMAL_MODE = 0,                        /*!< (0)普通模式(默认) */
    KEY_LONG_MODE                               /*!< (1)支持长按模式 */
} KEY_ModeTypeDef;

/* Exported constants --------------------------------------------------------*/

/* 按键的动作信息 */
#define KEY_NOT_PRESS           ((uint8_t)0x00) /* 没有按下 */
#define KEY_FRIST_PRESS         ((uint8_t)0x01) /* 首次按下 */
#define KEY_SHORT_HOLD_PRESS    ((uint8_t)0x02) /* 短按持续按下 */
#define KEY_SHORT_LOOSEN        ((uint8_t)0x04) /* 短按松开 */
#define KEY_LONG_HOLD_PRESS     ((uint8_t)0x08) /* 长按持续按下 */
#define KEY_LONG_LOOSEN         ((uint8_t)0x10) /* 长按松开 */

/* Exported macro ------------------------------------------------------------*/

/**
  * @brief  确认获取的按键动作和期望的按键动作是否符合
  * @note   keyAction 为获取的按键动作, Action 为期望的按键动作
  * @retval 0,不符合; 1,符合
  */
#define FML_KEY_CONFIRM_ACTION(keyAction, Action)  ((keyAction) == (Action) ? 1 : 0)

/* Exported functions --------------------------------------------------------*/

/* KEY 硬件初始化函数 */
extern void FML_KEY_DriveInit(void);

/* KEY 配置初始化函数 */
extern void FML_KEY_ConfigInit(void);

/* KEY 信息处理函数 */
extern void FML_KEY_ConfigHandle(uint8_t callTime);

/* KEY 检测按键是否存在动作函数 */
extern int  FML_KEY_CheckIsAction(KEY_NameTypeDef emKeyName);

/* KEY 读取按键动作函数 */
extern int  FML_KEY_ReadAction(KEY_NameTypeDef emKeyName);

/* KEY 设置按键模式函数 */
extern int  FML_KEY_ChangeMode(KEY_NameTypeDef emKeyName, KEY_ModeTypeDef  emMode);

/* KEY 设置按键动作有效时间函数 */
extern int  FML_KEY_ChangeTime(KEY_NameTypeDef emKeyName, uint16_t shortTime, uint16_t longTime);

/* KEY 复位按键动作函数 */
extern int  FML_KEY_ResetAction(KEY_NameTypeDef emKeyName);

/**
  * @}
  */

/**
  * @}
  */

#endif /* SOURCE_FML_KEY_DRIVE_H_ */
