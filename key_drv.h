/**
  ******************************************************************************
  * @file    key_drv.h
  * @author  周鹏程
  * @date    30-August-2019
  * @brief   该文件包含KEY输入硬件接口定义的所有函数原型
  ******************************************************************************
  * @attention
  *
  * 暂无
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_DRV_H_
#define __KEY_DRV_H_

/* Includes ------------------------------------------------------------------*/
#include "typedefs.h"

/** @addtogroup FML_KEY
  * @{
  */

/** @addtogroup KEY_Drv
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  输入/模拟按键名称枚举定义
  * @note   可参考原理图中的部分命名
  */
typedef enum{
    KEY_1 = 0,                              /*!< (0)原理图上的KEY1 */
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    
    /* 勿删除,用来统计按键的数目 */
    KEY_MAX_NUM,
} KEY_NameTypeDef;

/**
  * @brief  KEY开关枚举定义
  */
typedef enum{
    KEY_IS_OFF = 0,                         /*!< (0)断开(无动作) */
    KEY_IS_ON = !KEY_IS_OFF,                /*!< (1)闭合(有动作) */
}KEY_StateTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* KEY 驱动初始化 *************************************************************/
extern uint8_t  KEY_Initialize(void);
    
/* 读取/模拟所有按键的原始状态函数 ********************************************/
extern void     KEY_AllButtonInScan(void);

/* 获取指定输入/模拟按键的原始状态 ********************************************/
extern KEY_StateTypeDef KEY_GetButtonInState(KEY_NameTypeDef key);

#endif /* SOURCE_FML_KEY_KEY_DRV_H_ */
