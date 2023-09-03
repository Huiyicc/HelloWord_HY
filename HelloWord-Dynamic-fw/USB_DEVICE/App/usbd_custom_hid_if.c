/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

#define CUSTOM_HID_REPORT_ID 0x01

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END = {
        /* USER CODE BEGIN 0 */
        0x05, 0x8c, // USAGE_PAGE (ST Page) /
        0x09, 0x01, // USAGE (Demo Kit) /
        0xa1, 0x01, // COLLECTION (Application) /
        /* 6 */
        // The Input report
        0x09,0x03, // USAGE ID - Vendor defined
        0x15,0x00, // LOGICAL_MINIMUM (0)
        0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255)
        0x75,0x08, // REPORT_SIZE (8)
        0x95,CUSTOM_HID_EPIN_SIZE, //0x95,0x16, REPORT_COUNT (20)
        0x81,0x02, // INPUT (Data,Var,Abs)
        //19
        // The Output report
        0x09,0x04, // USAGE ID - Vendor defined
        0x15,0x00, // LOGICAL_MINIMUM (0)
        0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255)
        0x75,0x08, // REPORT_SIZE (8)
        0x95,CUSTOM_HID_EPOUT_SIZE, //0x95,0x16, REPORT_COUNT (20)
        0x91,0x02, // OUTPUT (Data,Var,Abs)
        //32
        /* USER CODE END 0 */
        0xC0    /*     END_COLLECTION	             */

//        0x05, 0x0C,        // Usage Page (Consumer)
//        0x09, 0x01,        // Usage (Consumer Control)
//        0xA1, 0x01,        // Collection (Application)
//        0xA1, 0x00,        //   Collection (Physical)
//        0x09, 0xE9,        //     Usage (Volume Increment)
//        0x09, 0xEA,        //     Usage (Volume Decrement)
//        0x09, 0xE2,        //     Usage (Mute)
//        0x09, 0xCD,        //     Usage (Play/Pause)
//        0x35, 0x00,        //     Physical Minimum (0)
//        0x45, 0x07,        //     Physical Maximum (7)
//        0x15, 0x00,        //     Logical Minimum (0)
//        0x25, 0x01,        //     Logical Maximum (1)
//        0x75, 0x01,        //     Report Size (1)
//        0x95, 0x04,        //     Report Count (4)
//        0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
//        0x75, 0x01,        //     Report Size (1)
//        0x95, 0x04,        //     Report Count (4)
//        0x81, 0x01,        //     Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
//        0xC0,              //   End Collection
//        0xC0,              // End Collection

// 38 bytes


//                0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
//                0x85, 0x01,                    //   REPORT_ID (1)
//                0x09, 0x06,                    // USAGE (Keyboard)
//                0xa1, 0x01,                    // COLLECTION (Application)
//                0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
//                0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
//                0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
//                0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//                0x75, 0x01,                    //   REPORT_SIZE (1)
//                0x95, 0x08,                    //   REPORT_COUNT (8)
//                0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//                0x95, 0x01,                    //   REPORT_COUNT (1)
//                0x75, 0x08,                    //   REPORT_SIZE (8)
//                0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
//                0x95, 0x05,                    //   REPORT_COUNT (5)
//                0x75, 0x01,                    //   REPORT_SIZE (1)
//                0x05, 0x08,                    //   USAGE_PAGE (LEDs)
//                0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
//                0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
//                0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
//                0x95, 0x01,                    //   REPORT_COUNT (1)
//                0x75, 0x03,                    //   REPORT_SIZE (3)
//                0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
//                0x95, 0x06,                    //   REPORT_COUNT (6)
//                0x75, 0x08,                    //   REPORT_SIZE (8)
//                0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//                0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
//                0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
//                0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
//                0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
//                0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
//                0xc0,   //END_COLLECTION

};
//__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
//        {
//                /* USER CODE BEGIN 0 */
//
//                0x05,0x8c, // USAGE_PAGE (Generic Desktop)
//                //0x05, 0xFF,        // Usage Page (Vendor Defined)
//                0x09, 0x01, // USAGE (Pointer)
//                0xa1, 0x01, // COLLECTION (Application) /
//                /* 6 */
//                // The Input report
//                0x09,0x03, // USAGE ID - Vendor defined
//                0x15,0x00, // LOGICAL_MINIMUM (0)
//                0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255)
//                0x75,0x08, // REPORT_SIZE (8)
//                0x95,CUSTOM_HID_EPIN_SIZE, //0x95,0x16, REPORT_COUNT (20)
//                0x81,0x02, // INPUT (Data,Var,Abs)
//                //19
//                // The Output report
//                0x09,0x04, // USAGE ID - Vendor defined
//                0x15,0x00, // LOGICAL_MINIMUM (0)
//                0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255)
//                0x75,0x08, // REPORT_SIZE (8)
//                0x95,CUSTOM_HID_EPOUT_SIZE, //0x95,0x16, REPORT_COUNT (20)
//                0x91,0x02, // OUTPUT (Data,Var,Abs)
//                //32
//                /* USER CODE END 0 */
//                0xC0    /*     END_COLLECTION	             */
//        };

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);

static int8_t CUSTOM_HID_DeInit_FS(void);

static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
        {
                CUSTOM_HID_ReportDesc_FS,
                CUSTOM_HID_Init_FS,
                CUSTOM_HID_DeInit_FS,
                CUSTOM_HID_OutEvent_FS
        };

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void) {
    /* USER CODE BEGIN 4 */
    return (USBD_OK);
    /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void) {
    /* USER CODE BEGIN 5 */
    return (USBD_OK);
    /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state) {
    /* USER CODE BEGIN 6 */
    UNUSED(event_idx);
    UNUSED(state);

    /* Start next USB packet transfer once data processing is completed */
    USBD_CUSTOM_HID_ReceivePacket(&hUsbDeviceFS);

    return (USBD_OK);
    /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

