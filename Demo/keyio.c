/**
  **********************************************************************************************************************
  * @file    keyio.c
  * @brief   该文件提供按键硬件接口实现功能
  * @author  const_zpc  any question please send mail to const_zpc@163.com
  * @version V3.0.0
  * @date    2022-03-27
  *
  *
  **********************************************************************************************************************
  * 
  *  移植时在函数 InitKeyIo 中实现 IO 的初始化配置, 同时定义枚举 @reg IoKeyType_e, 为每个实体按键或虚拟按键（有些按键
  *  不一定是一个 IO 对应按键, 如矩形按键、红外遥控按键等）定义一个枚举值（从 0 开始），然后在函数 IoKey_Scan 中得到每
  *  个实体按键或虚拟按键按下时的状态，并和 IO_KEY_ON 绑定
  *  
  **********************************************************************************************************************
  */
  
/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "keyio.h"
#include "sys.h"
#include "stm32f10x.h"

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/

static IoKeyState_e sg_arrKeyState[IO_KEY_MAX_NUM];

/* Private function prototypes ---------------------------------------------------------------------------------------*/
static void InitKeyIo(void);
/* Private function --------------------------------------------------------------------------------------------------*/

static void InitKeyIo(void)
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief      按键驱动初始化
  *
  */
void IoKey_Init(void)
{
    uint8_t i;
    
    for (i = 0; i < IO_KEY_MAX_NUM; i++)
    {
        sg_arrKeyState[i] = IO_KEY_OFF;
    }
    
    InitKeyIo();
}

/**
  * @brief      读取按键 IO 状态
  *
  * @param[in]  eKey 指定按键, 取值为 @reg IoKeyType_e
  * @retval     按键 IO 状态, 取值为 @reg IoKeyState_e.
  */
IoKeyState_e IoKey_Read(IoKeyType_e eKey)
{
    if (eKey < IO_KEY_MAX_NUM)
    {
        return sg_arrKeyState[eKey];
    }
    
    return IO_KEY_OFF;
}

/**
  * @brief      按键 IO 扫描任务
  *
  */
void IoKey_Scan(void)
{
    sg_arrKeyState[IO_KEY_0] = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0 ? IO_KEY_ON : IO_KEY_OFF;
    sg_arrKeyState[IO_KEY_1] = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0 ? IO_KEY_ON : IO_KEY_OFF;
    sg_arrKeyState[IO_KEY_WK_UP] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1 ? IO_KEY_ON : IO_KEY_OFF;
}
