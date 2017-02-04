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
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 6231 $ (v4.040.05.011646)
* $Date: 2016-09-29 19:27:47 +0530 (Thu, 29 Sep 2016) $
*
*/

#include <ph_Status.h>
#include <ph_NxpBuild.h>

#ifdef NXPBUILD__PH_KINETIS_K81

#include <phPlatform.h>

#define PORT_TIMER1    0x01
#define PORT_TIMER2    0x02
#define PORT_TIMER3    0x04
#define PORT_TIMER4    0x08


#define LPC_TIMER_MAX_16BIT          0xFFFFU
#define LPC_TIMER_MAX_16BIT_W_PRE    0xFFFE0000U  /* max 16bit x 16bit. */

#define KINETIS_TIMER_MAX_32BIT      0xFFFFFFFFU
#define LPC_TIMER_MAX_32BIT_W_PRE    0xFFFFFFFE00000000U /* max 32bit x 32bit. */

#define LPC_TIMER_MATCH_REGISTER     0x01         /* use match register 1 always. */

typedef struct timer_handler
{
    pit_chnl_t bPIT_Chnl;
    uint8_t    bOption;
    uint8_t    bSwTimerId;
    uint32_t   dwUnits;
    uint32_t   dwTimePeriod;
} timer_handler_t;

static timer_handler_t dwTimersId[4] = {0};

uint32_t phPlatform_Port_GetTimerFreq();

#define phPlatform_Port_GetTimerFreq() CLOCK_GetFreq(kCLOCK_BusClk)

/* Timer rate. */
uint32_t dwSysTickTimerFreq;

/* If a bit set it indicates the corresponding timer is busy.
 * Bit0 is for Timer0, Bit1 for Timer1 etc.
 * */
uint32_t dwTimerBusyFlags;

phStatus_t phPlatform_Port_Host_HwTimer_Init(void)
{
    pit_config_t pitConfig;         /* Structure of initialize PIT */
    uint8_t bTimerIndex;

    dwTimerBusyFlags = 0;
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    for(bTimerIndex = 0; bTimerIndex < PH_PLATFORM_MAX_TIMERS; bTimerIndex++)
    {
        dwTimersId[bTimerIndex].bPIT_Chnl = bTimerIndex;

        PIT_SetTimerChainMode(PIT, dwTimersId[bTimerIndex].bPIT_Chnl, false);
        PIT_StopTimer(PIT, dwTimersId[bTimerIndex].bPIT_Chnl);
        PIT_DisableInterrupts(PIT, dwTimersId[bTimerIndex].bPIT_Chnl, kPIT_TimerInterruptEnable);
        PIT_ClearStatusFlags(PIT, dwTimersId[bTimerIndex].bPIT_Chnl, kPIT_TimerFlag);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Create(uint32_t dwUnits, uint8_t bTimerId, void** ppTimerHandle)
{
    uint32_t dwTimer;

    /* shared memory resource, handle in critical section. */
    phPlatform_EnterCriticalSection();

    /* check for timer availability. */
    for(dwTimer = 0; dwTimer <  PH_PLATFORM_MAX_TIMERS; dwTimer++)
    {
        if(!(dwTimerBusyFlags & (E_TIMER_ACTIVE << dwTimer)))
        {
            dwTimerBusyFlags  |= (E_TIMER_ACTIVE << (uint32_t)dwTimer);
            break;
        }
    }

    phPlatform_ExitCriticalSection();

    if(dwTimer == PH_PLATFORM_MAX_TIMERS)
    {
        *ppTimerHandle = NULL;
        return PH_ERR_RESOURCE_ERROR;
    }
    else
    {
        *ppTimerHandle = (void *)&dwTimersId[dwTimer];

        PIT_StopTimer(PIT, dwTimersId[dwTimer].bPIT_Chnl);
        PIT_EnableInterrupts(PIT, dwTimersId[dwTimer].bPIT_Chnl, kPIT_TimerInterruptEnable);
        PIT_ClearStatusFlags(PIT, dwTimersId[dwTimer].bPIT_Chnl, kPIT_TimerFlag);

        dwTimersId[dwTimer].bSwTimerId = bTimerId;

        return PH_ERR_SUCCESS;
    }

}

phStatus_t phPlatform_Port_Host_HwTimer_Configure(void *pTimerHandle, uint32_t dwUnits, uint32_t dwTimePeriod)
{
    uint64_t          qwTimerCnt;
    uint32_t          dwTimerFreq;
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;

    dwTimerFreq = phPlatform_Port_GetTimerFreq();

    /* Timer count = (delay * freq)/Units. */
    qwTimerCnt  = (dwTimePeriod * dwTimerFreq);
    qwTimerCnt  /= dwUnits;

    /* 32-bit timers. */
    if(qwTimerCnt > (uint64_t)KINETIS_TIMER_MAX_32BIT)
    {
        return PH_ERR_PARAMETER_OVERFLOW;
    }
    else
    {
         phandle->dwTimePeriod = dwTimePeriod;
         phandle->dwUnits = dwUnits;

        /* Set timer period for channel 0 */
        PIT_SetTimerPeriod(PIT, phandle->bPIT_Chnl, (uint32_t)qwTimerCnt);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Start(void * pTimerHandle, uint16_t wOption)
{
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;

    /* Setup timer to restart when match occurs, Free Running mode. */
    if(wOption == PH_PLATFORM_TIMER_SINGLE_SHOT)
    {
        phandle->bOption = PH_PLATFORM_TIMER_SINGLE_SHOT;
    }
    else
    {
        phandle->bOption = PH_PLATFORM_TIMER_FREE_RUN;
    }

    PIT_StartTimer(PIT, phandle->bPIT_Chnl);

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_GetElapsedDelay(void * pTimerHandle, uint32_t dwUnits,
        uint32_t *dwGetElapsedDelay)
{
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;
    uint32_t          wElapsedCnt;
    uint32_t          dwTimerFreq;

    dwTimerFreq = phPlatform_Port_GetTimerFreq();

    wElapsedCnt = PIT_GetCurrentTimerCount(PIT, phandle->bPIT_Chnl);

    *dwGetElapsedDelay = (uint32_t)((wElapsedCnt * dwUnits)/dwTimerFreq) ;

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Stop(void * pTimerHandle)
{
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;

    PIT_StopTimer(PIT, phandle->bPIT_Chnl);
    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Reset(void * pTimerHandle)
{
    uint64_t          qwTimerCnt;
    uint32_t          dwTimerFreq;
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;

    dwTimerFreq = phPlatform_Port_GetTimerFreq();

    /* Timer count = (delay * freq)/Units. */
    qwTimerCnt  = (phandle->dwTimePeriod * dwTimerFreq);
    qwTimerCnt  /= phandle->dwUnits;

    /* 32-bit timers. */
    if(qwTimerCnt > (uint64_t)KINETIS_TIMER_MAX_32BIT)
    {
        return PH_ERR_PARAMETER_OVERFLOW;
    }
    else
    {
        /* Set timer period for channel 0 */
        PIT_SetTimerPeriod(PIT, phandle->bPIT_Chnl, (uint32_t)qwTimerCnt);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Delete(void * pTimerHandle)
{
    timer_handler_t * phandle = (timer_handler_t *) pTimerHandle;

    PIT_StopTimer(PIT, phandle->bPIT_Chnl);
    PIT_DisableInterrupts(PIT, phandle->bPIT_Chnl, kPIT_TimerInterruptEnable);

    /* Free the Timer flag. */
    dwTimerBusyFlags &= (~(1 << phandle->bPIT_Chnl));

    return PH_ERR_SUCCESS;
}

void phPlatform_Port_Host_HwTimer_DeInit(void)
{
    uint8_t bTimerIndex;

    for(bTimerIndex = 0; bTimerIndex < PH_PLATFORM_MAX_TIMERS; bTimerIndex++)
    {
        PIT_StopTimer(PIT, dwTimersId[bTimerIndex].bPIT_Chnl);
        PIT_DisableInterrupts(PIT, dwTimersId[bTimerIndex].bPIT_Chnl, kPIT_TimerInterruptEnable);
    }

    PIT_Deinit(PIT);
}

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);

    if (dwTimersId[kPIT_Chnl_0].bOption == PH_PLATFORM_TIMER_SINGLE_SHOT)
    {
        PIT_StopTimer(PIT, kPIT_Chnl_0);
        PIT_DisableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    }

    phPlatform_Timer_IsrHandler(dwTimersId[0].bSwTimerId);
}

void PIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, PIT_TFLG_TIF_MASK);

    if (dwTimersId[kPIT_Chnl_1].bOption == PH_PLATFORM_TIMER_SINGLE_SHOT)
    {
        PIT_StopTimer(PIT, kPIT_Chnl_1);
        PIT_DisableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
    }

    phPlatform_Timer_IsrHandler(dwTimersId[1].bSwTimerId);
}

void PIT2_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, PIT_TFLG_TIF_MASK);

    if (dwTimersId[kPIT_Chnl_2].bOption == PH_PLATFORM_TIMER_SINGLE_SHOT)
    {
        PIT_StopTimer(PIT, kPIT_Chnl_2);
        PIT_DisableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
    }

    phPlatform_Timer_IsrHandler(dwTimersId[2].bSwTimerId);
}

void PIT3_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, PIT_TFLG_TIF_MASK);

    if (dwTimersId[kPIT_Chnl_3].bOption == PH_PLATFORM_TIMER_SINGLE_SHOT)
    {
        PIT_StopTimer(PIT, kPIT_Chnl_3);
        PIT_DisableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
    }

    phPlatform_Timer_IsrHandler(dwTimersId[3].bSwTimerId);
}


#endif /* NXPBUILD__PH_KINETIS_K81 */
/******************************************************************************
**                            End Of File
******************************************************************************/
