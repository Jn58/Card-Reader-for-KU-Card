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
 * Platform file to abstract HOST and front-end.
 * $Author: Purnank G (ing05193) $
 * $Revision: 6060 $ (v4.040.05.011646)
 * $Date: 2016-09-20 17:41:54 +0530 (Tue, 20 Sep 2016) $
 *
 */

#include <ph_Status.h>

#ifdef NXPBUILD__PH_PLATFORM

#include <phPlatform.h>
#include <phOsal.h>

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#   include <phFlashBoot.h>
#endif

/* Platform dataparams will contain registered application callback
 * that needs to be called in Interrupt handler. */
phPlatform_DataParams_t *gpphPlatform_DataParams;

phStatus_t phPlatform_Init(
        phPlatform_DataParams_t  *pDataParams,
        uint8_t                  *pTxBuffer,
        uint16_t                  wTxBufSize,
        uint8_t                  *pRxBuffer,
        uint16_t                  wRxBufSize)
{
    phStatus_t  wStatus;
    pDataParams->wId     = PH_COMP_PLATFORM;

    gpphPlatform_DataParams = pDataParams;

#ifdef PH_PLATFORM_HAS_ICFRONTEND
    /* Configure reset and interrupt pins of Controller connected to a particular front-end. */
    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_RESET);
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_LpcOpenSpi_Init(&pDataParams->sBal,
            sizeof(phbalReg_LpcOpenSpi_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_LpcOpenI2c_Init(&pDataParams->sBal,
            sizeof(phbalReg_LpcOpenI2c_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_KinetisSpi_Init(&pDataParams->sBal,
            sizeof(phbalReg_KinetisSpi_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_KinetisI2C_Init(&pDataParams->sBal,
            sizeof(phbalReg_KinetisI2C_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_LinuxUserSpi_Init(&pDataParams->sBal,
            sizeof(phbalReg_LinuxUserSpi_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_LinuxKernelSpi_Init(&pDataParams->sBal,
            sizeof(phbalReg_LinuxKernelSpi_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    /* Initialize the Reader BAL (Bus Abstraction Layer) component. */
    PH_CHECK_SUCCESS_FCT(wStatus, phbalReg_LinuxUserI2C_Init(&pDataParams->sBal,
            sizeof(phbalReg_LinuxUserI2C_DataParams_t)));
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
#ifdef NXPBUILD__PHHAL_HW_RC523
    /* Initialize the 523 HAL component */
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Rc523_Init(
            &pDataParams->sHal,
            sizeof(phhalHw_Rc523_DataParams_t),
            &pDataParams->sBal,
#ifdef NXPBUILD__PH_KEYSTORE_SW
            &pDataParams->sKeyStore,
#else
            NULL,
#endif
            pTxBuffer,
            wTxBufSize,
            pRxBuffer,
            wRxBufSize
    ));
#endif /* NXPBUILD__PHHAL_HW_RC523 */

#ifdef NXPBUILD__PHHAL_HW_RC663
/* Initialize the RC663 HAL component */
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Rc663_Init(
            &pDataParams->sHal,
            sizeof(phhalHw_Rc663_DataParams_t),
            &pDataParams->sBal,
            NULL,
            pTxBuffer,
            wTxBufSize,
            pRxBuffer,
            wRxBufSize
    ));
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    /* Initialize the Pn5180 HAL component */
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_Pn5180_Init(
            &pDataParams->sHal,
            sizeof(phhalHw_Pn5180_DataParams_t),
            &pDataParams->sBal,
#ifdef NXPBUILD__PH_KEYSTORE_SW
            &pDataParams->sKeyStore,
#else
            NULL,
#endif
            pTxBuffer,
            wTxBufSize,
            pRxBuffer,
            wRxBufSize
    ));
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
    phFlashBoot_Main();
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_PN7462AU_Init(&pDataParams->sHal,
            sizeof(phhalHw_PN7462AU_DataParams_t),
            NULL,
#ifdef NXPBUILD__PH_KEYSTORE_SW
            &pDataParams->sKeyStore,
#else
            NULL,
#endif
            pTxBuffer,
            wTxBufSize,
            pRxBuffer,
            wRxBufSize
    ));
#endif /* NXPBUILD__PHHAL_HW_PN7462AU*/
    PH_CHECK_SUCCESS_FCT(wStatus, phPlatform_Timer_Init(pDataParams));

#ifdef PH_PLATFORM_HAS_ICFRONTEND
    /* This has to be done at last. As interrupts may trigger for the Default FE config.
     * Basically it is mandatory for Linux since IRQ posting is asynchronous.
     * */
    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_IRQ);

#endif /* PH_PLATFORM_HAS_ICFRONTEND */

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_DeInit(phPlatform_DataParams_t  *pDataParams)
{
    phStatus_t  wStatus;

    /* De-Initialize the Reader HAL (Hardware Abstraction Layer) component */
    PH_CHECK_SUCCESS_FCT(wStatus, phhalHw_DeInit(&pDataParams->sHal));

    /* Perform Front-end power down. */
    phPlatform_FE_Powerdown();


    return PH_ERR_SUCCESS;
}

void phPlatform_FE_Reset(void)
{
#ifdef PH_PLATFORM_HAS_ICFRONTEND
    uint32_t volatile i;

    /* send the reset pulse 0-1-0 to reset device */
    /* RSET signal low - PDOWN to '0' */
    phPlatform_Port_Host_SetPinValue(PHPLATFORM_PORT_PIN_RESET, RESET_POWERUP_LEVEL);

    //TODO: Replace below delay with real timer delay.
    /* delay of ~1,2 ms */
    for (i = 0x5000; i > 0; i --);

    phPlatform_Port_Host_SetPinValue(PHPLATFORM_PORT_PIN_RESET, RESET_POWERDOWN_LEVEL);

    /* delay of ~1,2 ms */
    for (i = 0x5000; i > 0; i --);

    /* RSET signal low - PDOWN to '0' */
    phPlatform_Port_Host_SetPinValue(PHPLATFORM_PORT_PIN_RESET, RESET_POWERUP_LEVEL);

    /* delay of ~1,2 ms */
    for (i = 0x5000; i > 0; i --);
#endif /* PH_PLATFORM_HAS_ICFRONTEND */
}

void phPlatform_FE_Powerdown(void)
{
#ifdef PH_PLATFORM_HAS_ICFRONTEND
    uint32_t volatile i;

    /* RSET signal low - PDOWN to '0' */
    phPlatform_Port_Host_SetPinValue(PHPLATFORM_PORT_PIN_RESET, RESET_POWERUP_LEVEL);

    /* delay of ~1,2 ms */
    for (i = 0x5000; i > 0; i --);

    /* RSET signal high to reset the RC663 IC - PDOWN to '1' */
    phPlatform_Port_Host_SetPinValue(PHPLATFORM_PORT_PIN_RESET, RESET_POWERDOWN_LEVEL);
#endif /* PH_PLATFORM_HAS_ICFRONTEND */
}

phStatus_t phPlatform_Timer_Init(phPlatform_DataParams_t *pDataParams)
{
    uint8_t bCnt;

    for(bCnt=0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
    {
        pDataParams->gTimers[bCnt].pTimerCallback = NULL;
        pDataParams->gTimers[bCnt].bTimerStatus = E_TIMER_FREE;
    }

    return phPlatform_Port_Host_HwTimer_Init();
}

void phPlatform_Timer_Create(phPlatform_DataParams_t *pDataParams, uint32_t dwUnits,
        phPlatform_Timer_Struct_t **ppTimerHandle)
{
    uint8_t bCnt;
    phStatus_t wStatus;

    *ppTimerHandle = (phPlatform_Timer_Struct_t *)NULL;
    do{
        /* Check for Memory Resource to Handle Hw Timer. */
        /* shared memory, keep under critical section. */
        phPlatform_EnterCriticalSection();
        for(bCnt=0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
        {
            if(pDataParams->gTimers[bCnt].bTimerStatus == E_TIMER_FREE)
            {
                pDataParams->gTimers[bCnt].bTimerStatus = E_TIMER_ACTIVE;
                pDataParams->gTimers[bCnt].bTimerId = bCnt;
                pDataParams->gTimers[bCnt].dwUnits = dwUnits;
                *ppTimerHandle  = &pDataParams->gTimers[bCnt];
                break;
            }
        }
        phPlatform_ExitCriticalSection();
        /* Insufficient Memory resource. */
        if(bCnt == PH_PLATFORM_MAX_TIMERS)
        {
            break;
        }

        /* check for hw timer availability. */
        wStatus = phPlatform_Port_Host_HwTimer_Create(dwUnits, bCnt, &pDataParams->gTimers[bCnt].pHwTimerHandle);
        if ((pDataParams->gTimers[bCnt].pHwTimerHandle == NULL) || (wStatus != PH_ERR_SUCCESS))
        {
            /* Hw Timer s not avail, release acquired Shared memory resource. */
            phPlatform_EnterCriticalSection();
            pDataParams->gTimers[bCnt].bTimerStatus = E_TIMER_FREE;
            *ppTimerHandle = (phPlatform_Timer_Struct_t *)NULL;
            phPlatform_ExitCriticalSection();
            break;
        }

    }while(0);
}

phStatus_t phPlatform_Timer_Configure(phPlatform_Timer_Struct_t *pTimerHandle, uint32_t dwTimePeriod,
        pphPlatform_CallBck_t pCallBackFunc)
{
    pTimerHandle->pTimerCallback = pCallBackFunc;
    return phPlatform_Port_Host_HwTimer_Configure(pTimerHandle->pHwTimerHandle, pTimerHandle->dwUnits,
            dwTimePeriod);
}

phStatus_t phPlatform_Timer_Start(phPlatform_Timer_Struct_t *pTimerHandle, uint16_t wOption)
{
    phStatus_t wStatus;
    wStatus = phPlatform_Port_Host_HwTimer_Start(pTimerHandle->pHwTimerHandle, wOption);

    if(wStatus == PH_ERR_SUCCESS)
    {
        pTimerHandle->bTimerStatus = E_TIMER_RUNNING;
    }

    return wStatus;
}

phStatus_t phPlatform_Timer_GetElapsedDelay(phPlatform_Timer_Struct_t *pTimerHandle, uint32_t *dwGetElapsedDelay)
{
    return phPlatform_Port_Host_HwTimer_GetElapsedDelay(pTimerHandle->pHwTimerHandle, pTimerHandle->dwUnits,
            dwGetElapsedDelay);
}

phStatus_t phPlatform_Timer_Stop(phPlatform_Timer_Struct_t *pTimerHandle)
{
    phStatus_t wStatus;
    wStatus = phPlatform_Port_Host_HwTimer_Stop(pTimerHandle->pHwTimerHandle);

    if(wStatus == PH_ERR_SUCCESS)
    {
        pTimerHandle->bTimerStatus = E_TIMER_STOPPED;
    }

    return wStatus;
}

phStatus_t phPlatform_Timer_Reset(phPlatform_Timer_Struct_t *pTimerHandle)
{
    phStatus_t wStatus;
    wStatus = phPlatform_Port_Host_HwTimer_Reset(pTimerHandle->pHwTimerHandle);

    if(wStatus == PH_ERR_SUCCESS)
    {
        pTimerHandle->bTimerStatus = E_TIMER_ACTIVE;
    }

    return wStatus;
}

phStatus_t phPlatform_Timer_Delete(phPlatform_Timer_Struct_t *pTimerHandle)
{
    phStatus_t wStatus;
    wStatus = phPlatform_Port_Host_HwTimer_Delete(pTimerHandle->pHwTimerHandle);

    if(wStatus == PH_ERR_SUCCESS)
    {
        pTimerHandle->pTimerCallback = NULL;
        pTimerHandle->bTimerStatus = E_TIMER_FREE;
    }

    return wStatus;
}

void phPlatform_Timer_DeInit(phPlatform_DataParams_t *pDataParams)
{
    uint8_t bCnt;

    for(bCnt=0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
    {
        pDataParams->gTimers[bCnt].pTimerCallback = NULL;
        pDataParams->gTimers[bCnt].bTimerStatus = E_TIMER_FREE;
    }

    phPlatform_Port_Host_HwTimer_DeInit();
}

void phPlatform_Timer_IsrHandler(uint8_t bTimerId)
{
    if(gpphPlatform_DataParams->gTimers[bTimerId].pTimerCallback != NULL)
    {
        gpphPlatform_DataParams->gTimers[bTimerId].pTimerCallback(bTimerId);
    }
}

/*==============================================================================
 * Function:    RF_IRQ_Handler
 *
 * brief:   Pin of interrupt EINT3 is attached to interrupt output pin of IC.
 *          The purpose of this interrupt handler is to detect RF signal from an
 *          External Peer .
 *
 * ---------------------------------------------------------------------------*/

/**
 * \brief Implementation of RF IRQ Handler.
 * This function will be called when RF event is been triggered from CLIF to HOST Processor.
 *
 * @return void
 */
#ifdef PH_PLATFORM_HAS_ICFRONTEND
void RF_IRQ_Handler(void)
{
    /* Read the interrupt status of external interrupt attached to the reader IC IRQ pin */
    if (phPlatform_Port_Host_GetPinValue(PHPLATFORM_PORT_PIN_IRQ))
    {
        /* Call application registered callback. */
        if (gpphPlatform_DataParams->sHal.pRFISRCallback != NULL)
        {
            gpphPlatform_DataParams->sHal.pRFISRCallback(&gpphPlatform_DataParams->sHal);
        };
    }
    phPlatform_Port_Host_ClearInt();
}
#endif /* PH_PLATFORM_HAS_ICFRONTEND */
#endif /* NXPBUILD__PH_PLATFORM */
/******************************************************************************
 **                            End Of File
 ******************************************************************************/
