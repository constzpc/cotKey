/**
  ******************************************************************************
  * @file    keyManage.h
  * @author  const_zpc
  * @date    2020-6-20
  * @brief   该文件包含 KEY 驱动管理的所有函数原型
  ******************************************************************************
  * @attention
  *
  * 暂无
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SOURCE_FML_KEY_MANAGE_H_
#define SOURCE_FML_KEY_MANAGE_H_

/* Includes ------------------------------------------------------------------*/
#include "keyDrive.h"

/** @addtogroup FML_KEY
  * @{
  */

/** @addtogroup KEY_MANAGE
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  KEY动作枚举定义
  */
typedef enum{
    KEY_IS_NOT_PRESS = 0,           /*!< (0)没有按下 */
    KEY_IS_FRIST_PRESS,             /*!< (1)首次按下 */
    KEY_IS_HOLD_PRESS,              /*!< (2)持续按下 */
    KEY_IS_LOOSEN,                  /*!< (3)松开 */
    KEY_IS_LONG_HOLD_PRESS,         /*!< (4)持续按下达到长按时间(仅使能长按模式下有效) */
    KEY_IS_LONG_LOOSEN              /*!< (5)达到长按时间后松开(仅使能长按模式下有效) */
} KEY_ActionTypeDef;

typedef void (*KeyFun)(KEY_NameTypeDef, KEY_ActionTypeDef);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* 初始化函数 ******************************************************************/
extern void FML_KEY_ManageInit(void);

/* KEY 注册/注销函数 ***********************************************************/
extern int  FML_KEY_Register(KEY_NameTypeDef emKeyName, KeyFun pKeyFun);
extern void FML_KEY_UnRegister(KEY_NameTypeDef emKeyName);

/* KEY 扫描任务函数 ************************************************************/
extern void FML_KEY_ScanTask(void);

/**
  * @}
  */

/**
  * @}
  */

#endif /* SOURCE_FML_KEY_MANAGE_H_ */
