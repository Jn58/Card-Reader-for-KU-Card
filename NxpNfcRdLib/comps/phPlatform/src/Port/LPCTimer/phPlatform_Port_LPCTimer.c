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
* $Revision: 5450 $ (v4.040.05.011646)
* $Date: 2016-07-06 13:27:31 +0530 (Wed, 06 Jul 2016) $
*
*/

#include <ph_Status.h>
#include <ph_NxpBuild.h>

#if defined (NXPBUILD__PH_LPC11U68) || \
    defined (NXPBUILD__PH_LPC1769)

#include <phPlatform.h>
#include <board.h>


#define PORT_TIMER1    0x01
#define PORT_TIMER2    0x02
#define PORT_TIMER3    0x04
#define PORT_TIMER4    0x08


#define LPC_TIMER_MAX_16BIT          0xFFFFU
#define LPC_TIMER_MAX_16BIT_W_PRE    0xFFFE0000U  /* max 16bit x 16bit. */

#define LPC_TIMER_MAX_32BIT          0xFFFFFFFFU
#define LPC_TIMER_MAX_32BIT_W_PRE    0xFFFFFFFE00000000U /* max 32bit x 32bit. */

#define LPC_TIMER_MATCH_REGISTER     0x01         /* use match register 1 always. */

#define LPC_TIMER_SYSTICK_MAX           0xFFFFFFU    /* [23:0] bits Timer. */

#define LPC_TIMER_SYSTICK_CLK            0x00000004
#define LPC_TIMER_SYSTICK_INT            0x00000002
#define LPC_TIMER_SYSTICK_ENABLE         0x00000001

static pphPlatform_TickTimerISRCallBck_t pTickCallBack;
static  uint64_t qwLoadValue;

/* Timer rate. */
static uint32_t dwSysTickTimerFreq;
/* If a bit set it indicates the corresponding timer is busy.
 * Bit0 is for Timer0, Bit1 for Timer1 etc.
 * */
static uint32_t dwTimerBusyFlags;

static uint8_t bSwTimerId[PH_PLATFORM_MAX_TIMERS];

#if (PH_PLATFORM_MAX_TIMERS != 4)
#warning "Number of LPC Hardware Timers are NOT matching with the PH_PLATFORM_MAX_TIMERS."
#endif

uint32_t phPlatform_Port_GetTimerFreq(void *pTimerHandle);

#if defined NXPBUILD__PH_LPC11U68
/* LPC11U68 has two 16bit timers and two 32 bit timers. */
static const LPC_TIMER_T * dwTimersId[4] = {LPC_TIMER16_0, LPC_TIMER16_1, LPC_TIMER32_0, LPC_TIMER32_1};
static const IRQn_Type eLpcIrq[4] = {TIMER_16_0_IRQn, TIMER_16_1_IRQn, TIMER_32_0_IRQn, TIMER_32_1_IRQn};
#define phPlatform_Port_GetTimerFreq(x) dwSysTickTimerFreq
#elif defined NXPBUILD__PH_LPC1769
static const LPC_TIMER_T * dwTimersId[4] = {LPC_TIMER0, LPC_TIMER1, LPC_TIMER2, LPC_TIMER3};
static const IRQn_Type eLpcIrq[4] = {TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn};

uint32_t phPlatform_Port_GetTimerFreq(void *pTimerHandle)
{
    uint8_t bTimerInex;
    CHIP_SYSCTL_CLOCK_T eLpcChipCtrlClk[4] = {SYSCTL_CLOCK_TIMER0, SYSCTL_CLOCK_TIMER1, SYSCTL_CLOCK_TIMER2,
            SYSCTL_CLOCK_TIMER3};

    for(bTimerInex = 0; (bTimerInex<PH_PLATFORM_MAX_TIMERS) && (dwTimersId[bTimerInex] != pTimerHandle);
            bTimerInex++);

    return Chip_Clock_GetPeripheralClockRate(eLpcChipCtrlClk[bTimerInex]);
}
#endif


phStatus_t phPlatform_Port_Host_HwTimer_Init(void)
{

    dwTimerBusyFlags = 0;

    /* SysTick Timer rate is system clock rate. */
    dwSysTickTimerFreq = Chip_Clock_GetSystemClockRate();

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Create(uint32_t dwUnits, uint8_t bTimerId, void** ppTimerHandle)
{

    uint32_t dwTimer;

    /* shared memory resource, handle in critical section. */
    phPlatform_EnterCriticalSection();

    /* check for timer availability. */
#if defined(NXPBUILD__PH_LPC11U68)
    /* Try to assign 16-bit timers for milli and micro secs timer. */
    if(((dwUnits == PH_PLATFORM_TIMER_UNIT_US) || (dwUnits == PH_PLATFORM_TIMER_UNIT_MS)) &&
            (dwTimerBusyFlags ^ 0x03))
    {
        /* 16-bit timer available. */
        dwTimer = (dwTimerBusyFlags & PORT_TIMER1)? PORT_TIMER2 : PORT_TIMER1;
        dwTimerBusyFlags  |= (1 << (uint32_t)dwTimer);
    }
    else
#endif
    {
        for(dwTimer = 0; dwTimer <  PH_PLATFORM_MAX_TIMERS; dwTimer++)
        {
            if(!(dwTimerBusyFlags & (E_TIMER_ACTIVE << dwTimer)))
            {
                dwTimerBusyFlags  |= (E_TIMER_ACTIVE << (uint32_t)dwTimer);
                break;
            }
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
        *ppTimerHandle = (void *)dwTimersId[dwTimer];

        /* Initialize 16-bit timer 0 clock */
        Chip_TIMER_Init(*ppTimerHandle);
        /* Resets the timer terminal and prescale counts to 0 */
        Chip_TIMER_Reset(*ppTimerHandle);
        /* Enables match interrupt that fires when terminal count matches the match counter value */
        Chip_TIMER_MatchEnableInt(*ppTimerHandle, LPC_TIMER_MATCH_REGISTER);

        bSwTimerId[dwTimer] = bTimerId;
        return PH_ERR_SUCCESS;
    }

}

phStatus_t phPlatform_Port_Host_HwTimer_Configure(void *pTimerHandle, uint32_t dwUnits, uint32_t dwTimePeriod)
{
    uint64_t qwTimerCnt;
    uint32_t dwPrescale;
    uint32_t dwTimerFreq;

    dwTimerFreq = phPlatform_Port_GetTimerFreq(pTimerHandle);

    Chip_TIMER_Reset(pTimerHandle);

    /* Timer count = (delay * freq)/Units. */
    qwTimerCnt  = (dwTimePeriod * dwTimerFreq);
    qwTimerCnt  /= dwUnits;

#if defined(NXPBUILD__PH_LPC11U68)
    /* check whether it is a 16-bit timer. */
    if((dwTimersId[0] == pTimerHandle) || (dwTimersId[1] == pTimerHandle))
    {
        if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_16BIT_W_PRE)
        {
            return PH_ERR_PARAMETER_OVERFLOW;
        }
        else if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_16BIT)
        {
            /* prescale is required. */
            for(dwPrescale=2;(dwPrescale<=LPC_TIMER_MAX_16BIT)&&((qwTimerCnt/dwPrescale)>LPC_TIMER_MAX_16BIT);
                    dwPrescale++);

            qwTimerCnt /= dwPrescale;

            /* Setup 16-bit prescale value to extend range */
            Chip_TIMER_PrescaleSet(pTimerHandle, dwPrescale);
        }

        /* Setup 16-bit timer's duration (16-bit match time) */
        Chip_TIMER_SetMatch(pTimerHandle, LPC_TIMER_MATCH_REGISTER, qwTimerCnt);
    }
    else
#endif
    {
        /* 32-bit timers. */
        if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_32BIT_W_PRE)
        {
            return PH_ERR_PARAMETER_OVERFLOW;
        }
        else if(qwTimerCnt > (uint64_t)LPC_TIMER_MAX_32BIT)
        {
            /* prescale is required. */
            for(dwPrescale=2;(dwPrescale<=LPC_TIMER_MAX_32BIT)&&((qwTimerCnt/dwPrescale)>LPC_TIMER_MAX_32BIT);
                    dwPrescale++);

            qwTimerCnt /= dwPrescale;

            /* Setup 16-bit prescale value to extend range */
            Chip_TIMER_PrescaleSet(pTimerHandle, dwPrescale);
        }

        /* Setup 16-bit timer's duration (16-bit match time) */
        Chip_TIMER_SetMatch(pTimerHandle, LPC_TIMER_MATCH_REGISTER, qwTimerCnt);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Start(void * pTimerHandle, uint16_t wOption)
{
    uint8_t bTimerInex;

    /* Setup timer to restart when match occurs, Free Running mode. */
    if(wOption & PH_PLATFORM_TIMER_FREE_RUN)
    {
        Chip_TIMER_ResetOnMatchEnable(pTimerHandle, LPC_TIMER_MATCH_REGISTER);
    }
    else
    {
        Chip_TIMER_ResetOnMatchDisable(pTimerHandle, LPC_TIMER_MATCH_REGISTER);
    }

    for(bTimerInex = 0; (bTimerInex < PH_PLATFORM_MAX_TIMERS) && (dwTimersId[bTimerInex] != pTimerHandle);
            bTimerInex++);

    Chip_TIMER_ClearMatch((LPC_TIMER_T *)dwTimersId[bTimerInex], LPC_TIMER_MATCH_REGISTER);

    /* Start timer */
    Chip_TIMER_Enable(pTimerHandle);

    /* Clear timer for any pending interrupt */
    NVIC_ClearPendingIRQ(eLpcIrq[bTimerInex]);

    /* Enable timer interrupt */
    NVIC_EnableIRQ(eLpcIrq[bTimerInex]);

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_GetElapsedDelay(void * pTimerHandle, uint32_t dwUnits,
        uint32_t *pdwGetElapsedDelay)
{
    uint64_t dwElapsedCnt;
    uint32_t dwTimerFreq;

    dwTimerFreq = phPlatform_Port_GetTimerFreq(pTimerHandle);

    dwElapsedCnt = (uint64_t)Chip_TIMER_ReadPrescale(pTimerHandle);

    if(dwElapsedCnt)
    {
        dwElapsedCnt *= Chip_TIMER_ReadCount(pTimerHandle);
    }
    else
    {
        dwElapsedCnt = Chip_TIMER_ReadCount(pTimerHandle);
    }

    *pdwGetElapsedDelay = (uint32_t)((dwElapsedCnt * dwUnits)/dwTimerFreq) ;

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Stop(void * pTimerHandle)
{
    Chip_TIMER_Disable(pTimerHandle);

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Reset(void * pTimerHandle)
{
    Chip_TIMER_Disable(pTimerHandle);

    Chip_TIMER_Reset(pTimerHandle);

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Delete(void * pTimerHandle)
{
    uint8_t bTimerInex;

    Chip_TIMER_Disable(pTimerHandle);
    Chip_TIMER_DeInit(pTimerHandle);

    for(bTimerInex = 0; (bTimerInex<PH_PLATFORM_MAX_TIMERS) && (dwTimersId[bTimerInex] != pTimerHandle);
            bTimerInex++);

    /* Disable timer interrupt */
    NVIC_DisableIRQ(eLpcIrq[bTimerInex]);

    /* Free the Timer flag. */
    dwTimerBusyFlags &= (~(1 << bTimerInex));

    return PH_ERR_SUCCESS;
}

void phPlatform_Port_Host_HwTimer_DeInit(void)
{
    uint8_t bTimerInex;

    for(bTimerInex = 0; bTimerInex < PH_PLATFORM_MAX_TIMERS; bTimerInex++)
    {
        phPlatform_Port_Host_HwTimer_Delete((void *)dwTimersId[bTimerInex]);
    }
}

phStatus_t phPlatform_InitTickTimer(pphPlatform_TickTimerISRCallBck_t pTickTimerCallback)
{
    pTickCallBack = pTickTimerCallback;

    qwLoadValue = 0;

    /* SysTick Timer rate is system clock rate. */
    dwSysTickTimerFreq = Chip_Clock_GetSystemClockRate();

    /* Disable systick and clear the Load value. */
    SysTick->CTRL = 0x0;
    SysTick->LOAD = 0x0;

    return PH_ERR_SUCCESS;
}

static void phPlatform_ConfigTick(void)
{
    /* Disable systick */
    SysTick->CTRL = 0x0;

    /* Configure SysTick count down value to interrupt at the requested time. */
    if(qwLoadValue > LPC_TIMER_SYSTICK_MAX)
    {
        qwLoadValue -= LPC_TIMER_SYSTICK_MAX;
        SysTick->LOAD = LPC_TIMER_SYSTICK_MAX;
    }
    else
    {
        SysTick->LOAD = (uint32_t)(qwLoadValue & LPC_TIMER_SYSTICK_MAX);
        qwLoadValue = 0;
    }

    /*Clear the current count value and also SysTick CTRL.COUNTFLAG. */
    SysTick->VAL = 0;

    SysTick->CTRL = LPC_TIMER_SYSTICK_CLK | LPC_TIMER_SYSTICK_INT | LPC_TIMER_SYSTICK_ENABLE;
}

phStatus_t phPlatform_StartTickTimer(uint32_t dwTimeMilliSecs)
{
    qwLoadValue = ((uint64_t)dwTimeMilliSecs * (uint64_t)dwSysTickTimerFreq)/PH_PLATFORM_TIMER_UNIT_MS;

    phPlatform_ConfigTick();

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_StopTickTimer(void)
{
    /* Disable systick and clear the Load value. */
    SysTick->CTRL = 0x0;
    SysTick->LOAD = 0x0;

    return PH_ERR_SUCCESS;
}

/* During FreeRTOS build SysTick_Handler shall be Taken from FreeRTOS port and this API is ignored. */
#ifdef NXPBUILD__PH_OSAL_FREERTOS
/* FreeRTOS would bring it's own SysTick_Handler */
#endif
#ifdef NXPBUILD__PH_OSAL_NULLOS
void SysTick_Handler(void)
{
    if(qwLoadValue)
    {
        if(qwLoadValue > LPC_TIMER_SYSTICK_MAX)
        {
            qwLoadValue -= LPC_TIMER_SYSTICK_MAX;
        }
        else
        {
            /* Timer to be configured. */
            phPlatform_ConfigTick();
        }
    }
    else
    {
        /* Stop the Timer and call the callBack. */
        phPlatform_StopTickTimer();

        pTickCallBack();
    }
}
#endif /* NXPBUILD__PH_OSAL_NULLOS */

void TIMER0_IRQHandler(void)
{
    Chip_TIMER_ClearMatch((LPC_TIMER_T *)dwTimersId[0], LPC_TIMER_MATCH_REGISTER);

    phPlatform_Timer_IsrHandler(bSwTimerId[0]);
}

void TIMER1_IRQHandler(void)
{
    Chip_TIMER_ClearMatch((LPC_TIMER_T *)dwTimersId[1], LPC_TIMER_MATCH_REGISTER);

    phPlatform_Timer_IsrHandler(bSwTimerId[1]);
}

void TIMER2_IRQHandler(void)
{
    Chip_TIMER_ClearMatch((LPC_TIMER_T *)dwTimersId[2], LPC_TIMER_MATCH_REGISTER);

    phPlatform_Timer_IsrHandler(bSwTimerId[2]);
}

void TIMER3_IRQHandler(void)
{
    Chip_TIMER_ClearMatch((LPC_TIMER_T *)dwTimersId[3], LPC_TIMER_MATCH_REGISTER);

    phPlatform_Timer_IsrHandler(bSwTimerId[3]);
}


#endif /* NXPBUILD__PH_LPC11U68 || NXPBUILD__PH_LPC1769 */
/******************************************************************************
**                            End Of File
******************************************************************************/
