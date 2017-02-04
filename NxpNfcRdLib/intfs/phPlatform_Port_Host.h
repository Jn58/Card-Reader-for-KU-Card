/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Platform file abstracting functions of Host controller.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 5812 $ (v4.040.05.011646)
* $Date: 2016-08-30 20:13:02 +0530 (Tue, 30 Aug 2016) $
*
*/

#ifndef PHPLATFORM_PORT_HOST_H
#define PHPLATFORM_PORT_HOST_H

#include <ph_Status.h>

#ifdef PH_PLATFORM_HAS_ICFRONTEND

/** @defgroup phPlatform_Port_Host Platform HOST Porting Component
* \brief These Components implement the Platform HOST porting abstraction layer.
* These API provide easy to port NfcRdLib on different Host platform.
* @{
*/

/**
 * @brief Functional Abstraction of various PINs between Host and Frontend.
 */
typedef enum phPlatform_Port_Host_config
{
    PHPLATFORM_PORT_PIN_RESET              = 0x1U,    /**< Reset the Frontend  */
    PHPLATFORM_PORT_PIN_IRQ                = 0x2U,    /**< Interrupt from the Frontend to Host */
    PHPLATFORM_PORT_PIN_SPI                = 0x3U,    /**< SPI Pins of the Host */
    PHPLATFORM_PORT_PIN_I2C                = 0x4U,    /**< I2C Pins of the Host */
    PHPLATFORM_PORT_PIN_BUSY               = 0x5U,    /**< Frontend's Busy Status, if Applicable */
    PHPLATFORM_PORT_PIN_DWL                = 0x6U,    /**< Assert Download mode of Frontend, if Applicable */
    PHPLATFORM_PORT_PIN_IFSEL0             = 0x7U,    /**< IFSEL0 of the Frontend, if Applicable */
    PHPLATFORM_PORT_PIN_IFSEL1             = 0x8U,    /**< IFSEL1 of the Frontend, if Applicable */
    PHPLATFORM_PORT_PIN_AD0                = 0x9U,    /**< Address Select Pin of Frontend, if Applicable */
    PHPLATFORM_PORT_PIN_AD1                = 0xAU,    /**< Address Select Pin of Frontend, if Applicable */
    PHPLATFORM_PORT_PIN_AD2                = 0xBU     /**< Address Select Pin of Frontend, if Applicable */
} phPlatform_Port_Host_config_t;

/**
 * @brief Configures the interrupt generation condition.
 */
typedef enum phPlatform_Port_Host_Interrupt
{
    PHPLATFORM_PORT_INT_DISABLED       = 0x0U,    /**< Interrupt request is disabled. */
    PHPLATFORM_PORT_INT_RISING_EDGE    = 0x1U,    /**< Interrupt on rising edge. */
    PHPLATFORM_PORT_INT_FALLING_EDGE   = 0x2U     /**< Interrupt on falling edge. */
} phPlatform_Port_Host_Interrupt_t;

/**
 * @brief This API is used to configure and initialize the HOST pins (ex : Reset, IRQ, AD etc) which are connected to
 * respective front-ends.
 * Port and Pin number are abstracted in specific Platform configuration header file.
 * For any new combination of Host and Front-end, new configuration header file can be added to easy the porting.
 *
 * @param[in] bConfig : HOST pin that needs to be configured.
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phPlatform_Port_Host_SetPinConfig(phPlatform_Port_Host_config_t bConfig);

/**
 * @brief This API is used to set the state of HOST pin.
 * Port and Pin number are abstracted in specific Platform configuration header file.
 * For any new combination of Host and Front-end, new configuration header file can be added to easy the porting.
 *
 * @param[in] bConfig : HOST pin needs to be set.
 * @param[in] bVal    : PH_PLATFORM_SET_HIGH for high, PH_PLATFORM_SET_LOW for low.
 * @return  Nothing
 */
void phPlatform_Port_Host_SetPinValue(phPlatform_Port_Host_config_t bConfig, uint8_t bVal);

/**
 * @brief Get a GPIO pin state via the GPIO byte register.
 * Port and Pin number are abstracted in specific Platform configuration header file.
 * For any new combination of Host and Front-end, new configuration header file can be added to easy the porting.
 *
 * @param[in] bConfig : Get the state of HOST pin.
 * @return Status of the API
 * @return true if HOST pin is in high state else false.
 */
bool phPlatform_Port_Host_GetPinValue(phPlatform_Port_Host_config_t bConfig);

/**
 * @brief   Clear interrupt status.
 * Port and Pin number are abstracted in specific Platform configuration header file.
 * For any new combination of Host and Front-end, new configuration header file can be added to easy the porting.
 *
 * @return  Nothing
 */
void phPlatform_Port_Host_ClearInt(void);

#endif /* PH_PLATFORM_HAS_ICFRONTEND */

/**
 * \brief Initialize the Host Timer module.
 * @note This API will be called by \ref phPlatform_Timer_Init API.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INTERNAL_ERROR For unsuccessful operation.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Init(void);

/**
 * \brief Allocates a free Host timer.
 * @note This API will be called by \ref phPlatform_Timer_Create API.
 *
 * @param[in] dwUnits Timer Resolution (e.g. \ref PH_PLATFORM_TIMER_UNIT_S)
 * @param[in] bTimerId  Timer Number
 * @param[out] ppTimerHandle Handle to the timer in case of success else NULL.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_RESOURCE_ERROR If host timer cannot be created.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Create(uint32_t dwUnits, uint8_t bTimerId, void** ppTimerHandle);

/**
 * \brief This API configures the acquired timer with timeout duration.
 * @note This API will be called by \ref phPlatform_Timer_Configure API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create.
 * @param[in] dwUnits Timer Resolution configured during \ref phPlatform_Port_Host_HwTimer_Create.
 * @param[in] dwTimePeriod
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_PARAMETER_OVERFLOW If timer cannot be configured to the provide time by parameter dwTimePeriod.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Configure(void *pTimerHandle, uint32_t dwUnits, uint32_t dwTimePeriod);

/**
 * \brief Start the Host timer, upon timer expire \ref phPlatform_Timer_IsrHandler API will be called with timer ID.
 * @note This API will be called by \ref phPlatform_Timer_Start API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create.
 * @param[in] wOption Parameter to mention Single shot \ref PH_PLATFORM_TIMER_SINGLE_SHOT
 *                    or Free running timer \ref PH_PLATFORM_TIMER_FREE_RUN.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR If host timer could not be started.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Start(void * pTimerHandle, uint16_t wOption);

/**
 * \brief Get Elapsed Delay from host timer which is created and started before calling this API.
 * @note This API will be called by \ref phPlatform_Timer_GetElapsedDelay API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create.
 * @param[in] dwUnits Timer Resolution configured during \ref phPlatform_Port_Host_HwTimer_Create.
 * @param[out] pdwGetElapsedDelay Duration of delay as per the passed resolution.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 */
phStatus_t phPlatform_Port_Host_HwTimer_GetElapsedDelay(void * pTimerHandle, uint32_t dwUnits, uint32_t *pdwGetElapsedDelay);

/**
 * \brief Stop the host timer.
 * @note This API will be called by \ref phPlatform_Timer_Stop API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not be stopped.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Stop(void * pTimerHandle);

/**
 * \brief Reset the host timer.
 * @note This API will be called by \ref phPlatform_Timer_Reset API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Could not reset timer.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Reset(void * pTimerHandle);

/**
 * \brief Delete the host timer.
 * @note This API will be called by \ref phPlatform_Timer_Delete API.
 *
 * @param[in] pTimerHandle Handle to timer as returned by \ref phPlatform_Port_Host_HwTimer_Create
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not delete.
 */
phStatus_t phPlatform_Port_Host_HwTimer_Delete(void * pTimerHandle);

/**
 * \brief DeInitializes the Timer component.
 * @note This API will be called by \ref phPlatform_Timer_DeInit API.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 */
void phPlatform_Port_Host_HwTimer_DeInit(void);

/**
 * @}
 */

#endif /* PHPLATFORM_PORT_HOST_H */
