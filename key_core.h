/**
  ******************************************************************************
  * @file    key_core.h
  * @author  周鹏程
  * @version V2.0.0
  * @date    18-October-2019
  * @brief   该文件包含KEY输入相关数据处理的所有函数原型
  ******************************************************************************
  * @attention
  *
  * 暂无
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_KEY_CORE_H_
#define BSP_KEY_CORE_H_

/* Includes ------------------------------------------------------------------*/
#include "key_cfg.h"
#include "key_drv.h"

/** @addtogroup FML_KEY
  * @{
  */

/** @addtogroup KEY_Core
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/**
  * @brief  确认获取的按键动作和期望的按键动作是否符合
  * @note   keyAction 为获取的按键动作, Action 为期望的按键动作
  * @retval 0,不符合; 1,符合
  */
#define KEY_CONFIRM_ACTION(keyAction, Action)  ((keyAction) == (Action) ? 1 : 0)

/* Exported functions --------------------------------------------------------*/

/* KEY 信息初始化函数 *********************************************************/
extern void     KEY_ConfigInfoInit(void);

/* KEY 信息处理函数 ***********************************************************/
extern void     KEY_ConfigInfoHandle(void);

/* KEY 检测按键是否存在动作函数 ***********************************************/
extern uint8_t  KEY_CheckIsAction(uint8_t index);

/* KEY 读取按键动作函数 *******************************************************/
extern uint8_t  KEY_ReadAction(uint8_t index);

/* KEY 设置按键模式函数 *******************************************************/
extern void     KEY_ChangeMode(uint8_t index, KEY_ModeTypeDef  emMode);

/* KEY 设置按键动作有效时间函数 ***********************************************/
extern void     KEY_ChangeShortTime(uint8_t index, uint16_t time);
extern void     KEY_ChangeLongTime(uint8_t index, uint16_t time);

/* KEY 复位按键动作函数 *******************************************************/
extern void     KEY_ResetAction(uint8_t index);

/* KEY 使能/禁止按键扫描函数 **************************************************/
extern void     KEY_EnableScan(uint8_t index, uint8_t enable);

/**
  * @}
  */

/**
  * @}
  */

#endif /* SOURCE_FML_KEY_KEY_CORE_H_ */
