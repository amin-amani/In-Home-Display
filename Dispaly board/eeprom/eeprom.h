/**
  ******************************************************************************
  * @file    EEPROM_Emulation/inc/eeprom.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the EEPROM 
  *          emulation firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H
#define STM32F10X_MD
#define     __IO    volatile  
/* Includes ------------------------------------------------------------------*/
//#include <stm32f10x.h>
#include <stm32f10x_type.h>
#include <stm32f10x_flash.h>

/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define PAGE_SIZE  (u16)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
  #define PAGE_SIZE  (u16)0x800  /* Page size = 2KByte */
#endif

/* EEPROM start address in Flash */
#define EEPROM_START_ADDRESS    ((u32)0x08010000) /* EEPROM emulation start address:
                                                  after 64KByte of used Flash memory */

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((u32)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((u32)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((u32)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((u32)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

/* Used Flash pages for EEPROM emulation */
#define PAGE0                   ((u16)0x0000)
#define PAGE1                   ((u16)0x0001)

/* No valid page define */
#define NO_VALID_PAGE           ((u16)0x00AB)

/* Page status definitions */
#define ERASED                  ((u16)0xFFFF)     /* PAGE is empty */
#define RECEIVE_DATA            ((u16)0xEEEE)     /* PAGE is marked to receive data */
#define VALID_PAGE              ((u16)0x0000)     /* PAGE containing valid data */

/* Valid pages in read and write defines */
#define READ_FROM_VALID_PAGE    ((u8)0x00)
#define WRITE_IN_VALID_PAGE     ((u8)0x01)

/* Page full define */
#define PAGE_FULL               ((u8)0x80)

/* Variables' number */
#define NumbOfVar               ((u8)0x03)

/* Exported types ------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u16 EE_Init(void);
u16 EE_ReadVariable(u16 VirtAddress, u16* Data);
u16 EE_WriteVariable(u16 VirtAddress, u16 Data);

#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
