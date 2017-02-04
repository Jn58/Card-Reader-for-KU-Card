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
* Timer of MicroController Platform.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 5957 $ (v4.040.05.011646)
* $Date: 2016-09-16 18:03:05 +0530 (Fri, 16 Sep 2016) $
*
*/

#ifndef PHPLATFORM_H
#define PHPLATFORM_H

#include <ph_Status.h>
#include <stdlib.h>
#include <phbalReg.h>
#include <phhalHw.h>
#include <phKeyStore.h>

#ifdef  __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PH_PLATFORM

/** \defgroup phPlatform Platform Component
* \brief These Components implement the Platform abstraction layer.
* @{
*/

/* ------------------------------------------------ */
/* General port definition                          */
/* ------------------------------------------------ */
/**
* \name Platform Ports
* @{ */
#define PH_PLATFORM_PORT0               0          /**< Platform Host Port Abstraction for Port0. */
#define PH_PLATFORM_PORT1               1          /**< Platform Host Port Abstraction for Port1. */
#define PH_PLATFORM_PORT2               2          /**< Platform Host Port Abstraction for Port2. */
#define PH_PLATFORM_PORT3               3          /**< Platform Host Port Abstraction for Port3. */
#define PH_PLATFORM_PORT4               4          /**< Platform Host Port Abstraction for Port4. */
/* @} */

#define PH_PLATFORM_SET_HIGH            1          /**< Logic High. */
#define PH_PLATFORM_SET_LOW             0          /**< Logic Low. */

#define PH_PLATFORM_CONFIGURE_SPI       0U
#define PH_PLATFORM_CONFIGURE_I2C       1U

#define PH_PLATFORM_MAX_TIMERS          4U         /**< Maximum number of timers available. */

/**
* \name Timer Configuration
* @{ */
#define PH_PLATFORM_TIMER_SINGLE_SHOT   0x0000U    /**< On Timer expire it will be Stopped and Timercallback will be called. */
#define PH_PLATFORM_TIMER_FREE_RUN      0x0001U    /**< Auto reloads on expire of the Timer and Timercallback will be called. */
/* @} */

/**
* \name Timer Units
* @{ */
#define PH_PLATFORM_TIMER_UNIT_US       1000000U   /**< Indicates that the specified delay is in microseconds.*/
#define PH_PLATFORM_TIMER_UNIT_MS       1000U      /**< Indicates that the specified delay is in milliseconds. */
#define PH_PLATFORM_TIMER_UNIT_S        1U         /**< Indicates that the specified delay is in seconds. */
/* @} */

/**
*
* \brief Tick Timer ISR callback which will be called by ISR handler implementation in Platform Layer.
* \retval  None
*/
typedef void (*pphPlatform_TickTimerISRCallBck_t)(void);

/**
*
* \brief Timer callback interface which will be called once registered timer
* timeout expires.
* \param[in] TimerId  Timer Id for which callback is called.
* \retval  None
*/
typedef void (*pphPlatform_CallBck_t)(uint8_t bTimerId);

/**
 * Status of the timer.
 */
typedef enum phPlatform_Timer_Status
{
    E_TIMER_FREE = 0,//!< E_TIMER_FREE
    E_TIMER_ACTIVE,  //!< E_TIMER_ACTIVE
    E_TIMER_RUNNING, //!< E_TIMER_RUNNING
    E_TIMER_STOPPED  //!< E_TIMER_STOPPED
} phPlatform_Timer_Status_t;

typedef struct phPlatform_Timer_Struct
{
    void                     *pHwTimerHandle;                        /**< Timer handle for the requested hardware timer. */
    pphPlatform_CallBck_t     pTimerCallback;                        /**< The call back function for this timer. */
    uint32_t                  dwUnits;                               /**< Unit configured by user. */
    uint8_t                   bTimerId;                              /**< ID of the timer. */
    phPlatform_Timer_Status_t bTimerStatus;                          /**< Indicates current timer status. */
} phPlatform_Timer_Struct_t;

/**
* \brief OSAL parameter structure
*/
typedef struct
{
    uint16_t                            wId;                             /**< ID of this component, do not modify. */

#ifdef NXPBUILD__PHHAL_HW_RC523
    phhalHw_Rc523_DataParams_t          sHal;                            /**< HAL component holder. */
#endif /* NXPBUILD__PHHAL_HW_RC523 */
#ifdef NXPBUILD__PHHAL_HW_RC663
    phhalHw_Rc663_DataParams_t          sHal;                            /**< HAL component holder. */
#endif /* NXPBUILD__PHHAL_HW_RC663 */
#ifdef NXPBUILD__PHHAL_HW_PN5180
    phhalHw_Pn5180_DataParams_t         sHal;                            /**< HAL component holder. */
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    phhalHw_PN7462AU_DataParams_t       sHal;                            /**< HAL component holder. */
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    phbalReg_LpcOpenSpi_DataParams_t    sBal;                            /**< BAL component holder. */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */
#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    phbalReg_LpcOpenI2c_DataParams_t    sBal;                            /**< BAL component holder. */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    phbalReg_KinetisSpi_DataParams_t    sBal;             /* BAL component holder. */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    phbalReg_KinetisI2C_DataParams_t    sBal;             /* BAL component holder. */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    phbalReg_LinuxUserSpi_DataParams_t sBal;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    phbalReg_LinuxKernelSpi_DataParams_t sBal;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    phbalReg_LinuxUserI2C_DataParams_t sBal;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

#ifdef NXPBUILD__PH_KEYSTORE_SW
    phKeyStore_Sw_DataParams_t          sKeyStore;                       /**< Keystore dataparams, due to design limitation keystore is included as part of platform dataparams. */
#endif /* NXPBUILD__PH_KEYSTORE_SW */

    phPlatform_Timer_Struct_t           gTimers[PH_PLATFORM_MAX_TIMERS]; /**< Timer structure. */
} phPlatform_DataParams_t;

extern phPlatform_DataParams_t *gpphPlatform_DataParams;

/* *****************************************************************************************************************
* Function Prototypes
* ***************************************************************************************************************** */
/**
 * \brief Initializes the Platform component.
 * a. Performs Front-end initialization required to configure reset pins and interrupt pins of
 *    Controller connected to a particular front-end.
 * b. Initialize BAL.
 * c. Initialize OSAL.
 * d. Initialize HAL.
 * e. Initialize Platform Timers.
 *
 * @param[in] pDataParams  Data parameters representing this component.
 *                         Application allocates this Data structure and passes the pointer of the same.
 * @param[in] pISRCallBack Application callback that needs to be called by RF ISR Handler.
 * @param[in] pTxBuffer    Pointer to application allocated transmit buffer used by the phhalHw functions.
 * @param[in] wTxBufSize   Size of the application allocated transmit buffer.
 * @param[in] pRxBuffer    Pointer to application allocated receive buffer used by the phhalHw functions.
 * @param[in] wRxBufSize   Size of the application allocated receive buffer.
 *                         Specify the buffer+1 byte, because one byte is reserved for SPI communication.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phPlatform_Init(
                           phPlatform_DataParams_t *pDataParams,
                           uint8_t                 *pTxBuffer,
                           uint16_t                 wTxBufSize,
                           uint8_t                 *pRxBuffer,
                           uint16_t                 wRxBufSize
                           );

/**
 * \brief De-initialize the Platform component.
 * a. Performs Front-end power down.
 * b. De-initialize HAL.
 *
 * @param[in] pDataParams  Data parameters representing this component.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phPlatform_DeInit(phPlatform_DataParams_t *pDataParams);

/**
 * \brief Performs Front-end reset.
 * When this API is called a pulse will be triggered on "NRSTPD" PIN (Not Reset and Power Down) to
 * reset front-end device.
 *
 * @return void
 */
void phPlatform_FE_Reset(void);

/**
 * \brief Performs Front-end power-down.
 * When this API is called a "NRSTPD" PIN (Not Reset and Power Down) will be configured to
 * power-down the front-end device.
 *
 * @return void
 */
void phPlatform_FE_Powerdown(void);

/**
 * \brief Enter critical section.
 * @return void
 */
extern void phPlatform_EnterCriticalSection(void);

/**
 * \brief Exit critical section.
 * @return void
 */
extern void phPlatform_ExitCriticalSection(void);

/**
 * \brief Returns whether Controller is in ISR mode or NOT.
 * @return uint32_t 0-> Controller is NOT in ISR handler mode, Non-zero to indicate ISR mode.
 */
extern uint32_t phPlatform_Is_Irq_Context(void);

/**
 * \brief Enter sleep mode.
 * @return void
 */
extern void phPlatform_Sleep(void);

/**
 * \brief Exit sleep mode.
 * @return void
 */
extern void phPlatform_WakeUp(void);

/**
 * \brief Initializes the Timer component.
 * @note This Api must be called before calling any other Timer Api.
 *
 * @param[in]  pDataParams   Pointer to this layers parameter structure.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 */
phStatus_t phPlatform_Timer_Init(phPlatform_DataParams_t *pDataParams);

/**
 * \brief Allocates a free timer.
 *
 * @param[in]  pDataParams   Pointer to this layers parameter structure.
 * @param[in]  dwUnits       Timer tick units.
 * @param[out] ppTimerHandle Timer handle in case of success else NULL.
 *
 * @return void
 */
void phPlatform_Timer_Create(phPlatform_DataParams_t    *pDataParams,
                             uint32_t                    dwUnits,
                             phPlatform_Timer_Struct_t **ppTimerHandle
                             );

/**
 * \brief This API configures the acquired timer with timeout duration and callback to be called upon timer expire.
 *
 * @param[in] pTimerHandle  Timer handle returned by \ref phPlatform_Timer_Create.
 * @param[in] dwTimePeriod  Time delay count.
 * @param[in] pCallBackFunc Callback to be called on time out.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_PARAMETER_OVERFLOW If timer cannot be configured to the provide time by parameter dwTimePeriod.
 */
phStatus_t phPlatform_Timer_Configure(phPlatform_Timer_Struct_t       *pTimerHandle,
                                      uint32_t                         dwTimePeriod,
                                      pphPlatform_CallBck_t            pCallBackFunc
                                      );

/**
 * \brief Start the timer.
 *
 * @param[in] pTimerHandle Timer handle returned by \ref phPlatform_Timer_Create.
 * @param[in] wOption Parameter to mention Single shot \ref PH_PLATFORM_TIMER_SINGLE_SHOT
 *                    or Free running timer \ref PH_PLATFORM_TIMER_FREE_RUN.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not be started.
 */
phStatus_t phPlatform_Timer_Start(phPlatform_Timer_Struct_t *pTimerHandle,
                                  uint16_t                   wOption
                                  );

/**
 * \brief Get Elapsed Delay from timer which is created and started before calling this API.
 *
 * @param[in]  pTimerHandle      Timer handle returned by \ref phPlatform_Timer_Create.
 * @param[out] dwGetElapsedDelay Measured Delay after Timer Start in sec/milli/micro seconds based on input option.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 */
phStatus_t phPlatform_Timer_GetElapsedDelay(phPlatform_Timer_Struct_t *pTimerHandle,
                                            uint32_t                  *dwGetElapsedDelay
                                            );

/**
 * \brief Stop the timer.
 *
 * @param[in] pTimerHandle Timer handle returned by \ref phPlatform_Timer_Create.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not be stopped.
 */
phStatus_t phPlatform_Timer_Stop(phPlatform_Timer_Struct_t *pTimerHandle);

/**
 * \brief Resets the timer allocated to the application which is created by \ref phPlatform_Timer_Create.
 *
 * @param[in] pTimerHandle Timer handle returned by \ref phPlatform_Timer_Create.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not be stopped.
 */
phStatus_t phPlatform_Timer_Reset(phPlatform_Timer_Struct_t *pTimerHandle);

/**
 * \brief Delete or Deallocate the timer.
 *
 * @param[in] pTimerHandle Timer handle returned by \ref phPlatform_Timer_Create.
 *
 * @return Status of the API
 * @retval #PH_ERR_SUCCESS Operation successful.
 * @retval #PH_ERR_INVALID_PARAMETER Invalid input parameter.
 * @retval #PH_ERR_RESOURCE_ERROR Timer could not be stopped.
 */
phStatus_t phPlatform_Timer_Delete(phPlatform_Timer_Struct_t *pTimerHandle);

/**
 * \brief DeInitializes the Timer component.
 *
 * @param[in]  pDataParams   Pointer to this layers parameter structure.
 *
 * @return void
 */
void phPlatform_Timer_DeInit(phPlatform_DataParams_t *pDataParams);

/**
 * \brief Initialize Controller related IPs or clock.
 *
 * @return void
 */
void phPlatform_Controller_Init(void);
/** @} */

/* *****************************************************************************************************************
* Internal Functions
* ***************************************************************************************************************** */
/**
 * \brief This API will be called by the Timer ISR Handler implementation in port layer.
 * This is an internal function that will be called by HOST porting implementation when
 * Timer ISR Handler is executed.
 *
 * @param[in] bTimerId Timer ID.
 * @return void
 */
void phPlatform_Timer_IsrHandler(uint8_t bTimerId);

/**
 * \brief Initializes the Tick Timer required by the NullOs implementation.
 *
 * @return void
 */
extern phStatus_t phPlatform_InitTickTimer(pphPlatform_TickTimerISRCallBck_t pTickTimerCallback);

/**
 * \brief Start the Tick timer.
 *
 * @return void
 */
extern phStatus_t phPlatform_StartTickTimer(uint32_t dwTimeMilliSecs);

/**
 * \brief Stop the Tick timer.
 *
 * @return void
 */
extern phStatus_t phPlatform_StopTickTimer(void);

#if defined (NXPBUILD__PHHAL_HW_PN7462AU) && defined (NXPBUILD__PH_PN7462AU)
#include <phPlatform_Port_PN7462AU.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC663) && defined (NXPBUILD__PH_LPC11U68)
#include <phPlatform_Port_LPC11u68_RC663.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC523) && defined (NXPBUILD__PH_LPC11U68)
#include <phPlatform_Port_LPC11u68_RC523.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC663) && defined (NXPBUILD__PH_LPC1769)
#include <phPlatform_Port_LPC1769_RC663.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC523) && defined (NXPBUILD__PH_LPC1769)
#include <phPlatform_Port_LPC1769_RC523.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_PN5180) && defined (NXPBUILD__PH_LPC1769)
#include <phPlatform_Port_LPC1769_PN5180.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_PN5180) && defined (NXPBUILD__PH_KINETIS_K81)
#include <phPlatform_Port_K81_PN5180.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_PN5180) && defined (NXPBUILD__PH_KINETIS_K82)
#include <phPlatform_Port_K82_PN5180.h>
#endif


#if defined (NXPBUILD__PHHAL_HW_RC523) && defined (NXPBUILD__PH_RASPBERRY_PI)
#include <phPlatform_Port_Pi_RC523.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC663) && defined (NXPBUILD__PH_RASPBERRY_PI)
#include <phPlatform_Port_Pi_RC663.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_PN5180) && defined (NXPBUILD__PH_RASPBERRY_PI)
#include <phPlatform_Port_Pi_PN5180.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_PN5180) && defined (NXPBUILD__PH_KINETIS_K82)
#include <phPlatform_Port_K82_PN5180.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC663) && defined (NXPBUILD__PH_KINETIS_K82)
#include <phPlatform_Port_K82_RC663.h>
#endif

#if defined (NXPBUILD__PHHAL_HW_RC523) && defined (NXPBUILD__PH_KINETIS_K82)
#include <phPlatform_Port_K82_RC523.h>
#endif

#include <phPlatform_Port_Host.h>

#if defined (NXPBUILD__PH_LPC1769) || defined (NXPBUILD__PH_LPC11U68) || defined (NXPBUILD__PH_PN7462AU) || defined (NXPBUILD__PH_KINETIS)
#  include <phPlatform_Port_Cortex.h>
#elif defined (NXPBUILD__PH_RASPBERRY_PI)
#  include <phPlatform_Port_Pi.h>
#else
#  error "Enable any Platform."
#endif



#endif /* NXPBUILD__PH_PLATFORM */

#ifdef  __cplusplus
}
#endif /* __cplusplus */

#endif /* PHPLATFORM_H */
