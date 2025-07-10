 #ifndef __BSP_IIC_SOFTWARE_H
 #define __BSP_IIC_SOFTWARE_H

 #include "stm32f1xx.h"

 /* IIC引脚宏定义　
  * IIC_SDA---->PB11
  * IIC_SCL---->PB10
  * 用户可自定义引脚 详见STM32官方数据手册
  */
#define IIC_NUM                  I2C2
#define IIC_GPIO_CLK_ENABLE      __HAL_RCC_GPIOB_CLK_ENABLE
#define IIC_CLK_ENABLE           __HAL_RCC_I2C2_CLK_ENABLE
#define IIC_GPIO_PORT            GPIOB
#define IIC_SDA_GPIO_PIN         GPIO_PIN_11
#define IIC_SCL_GPIO_PIN         GPIO_PIN_10

 /* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
 /* 条件编译： 1 选择GPIO的HAL库实现IO读写 */
 #if 0
     #define IIC_SDA_0    HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET)
     #define IIC_SDA_1    HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)
     #define IIC_SCL_0    HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET)
     #define IIC_SCL_1    HAL_GPIO_WritePin(IIC_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET)
    
     #define IIC_SDA_READ HAL_GPIO_ReadPin(IIC_GPIO_PORT, IIC_SDA_GPIO_PIN)

 /* 这个分支选择直接寄存器操作实现IO读写 
  * 注意：如下写法，在IAR最高级别优化时，会被编译器错误优化
  */
 #else
     #define IIC_SDA_0 IIC_GPIO_PORT->BSRR = IIC_SDA_GPIO_PIN << 16u     /* SDA = 0 */
     #define IIC_SDA_1 IIC_GPIO_PORT->BSRR = IIC_SDA_GPIO_PIN            /* SDA = 1 */
     #define IIC_SCL_0 IIC_GPIO_PORT->BSRR = IIC_SCL_GPIO_PIN << 16u     /* SCL = 0 */
     #define IIC_SCL_1 IIC_GPIO_PORT->BSRR = IIC_SCL_GPIO_PIN            /* SCL = 1 */
    
     #define IIC_SDA_READ (IIC_GPIO_PORT->IDR & IIC_SDA_GPIO_PIN) >> 11
    
 #endif

 void IIC_GPIO_Config(void);
 void IIC_Start(void);
 void IIC_Stop(void);
 void IIC_ACK(uint8_t ack);
 uint8_t IIC_Wait_ACK(void);
 void IIC_SendByte(uint8_t byte);
 uint8_t IIC_ReciveByte(void);

 #endif

