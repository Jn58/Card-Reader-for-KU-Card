/*
 * phPlatform_Port_PN7462AU.c
 *
 *  Created on: Jun 2, 2016
 *      Author: nxp79566
 */


#include <ph_Status.h>
#include <ph_NxpBuild.h>
#include <phPlatform_Port_PN7462AU.h>

#if defined NXPBUILD__PH_PN7462AU

#include <phhalTimer.h>
#include <phPlatform.h>

static pphPlatform_TickTimerISRCallBck_t pTickCallBack;
static  uint64_t qwLoadValue;
/* Timer rate. */
static uint32_t dwSysTickTimerFreq;

#define PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD           ( ( volatile uint32_t *) 0xe000e014 )
#define PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL           ( ( volatile uint32_t *) 0xe000e010 )
#define PH_RTOS_NORTOS_MAX_SYSTICK                 800 /** max configurable is 838 milli secs. */
#define PH_RTOS_NORTOS_NVIC_SYSTICK_CUR_VAL        ( ( volatile uint32_t *) 0xe000e018 )
#define PH_RTOS_NORTOS_NVIC_SYSTICK_CLK            0x00000004
#define PH_RTOS_NORTOS_NVIC_SYSTICK_INT            0x00000002
#define PH_RTOS_NORTOS_NVIC_SYSTICK_ENABLE         0x00000001

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */
static void phPlatform_Timer0_CallbackFun(void);
static void phPlatform_Timer1_CallbackFun(void);
static void phPlatform_Timer2_CallbackFun(void);
static void phPlatform_Timer3_CallbackFun(void);

static const pphhalTimer_CallbackFun_t paTimerHalCallBack[E_TIMER_4] = {&phPlatform_Timer0_CallbackFun,
        &phPlatform_Timer1_CallbackFun,
        &phPlatform_Timer2_CallbackFun,
        &phPlatform_Timer3_CallbackFun};

typedef struct phPlatform_TimerMap{
    void *p7462TimerHandle;
    uint8_t bSwTimerId;
} phPlatform_TimerMap_PN7462_t;

phPlatform_TimerMap_PN7462_t gphPlatform_Port_Pn7462Tmrs[PH_PLATFORM_MAX_TIMERS];

void phPlatform_Controller_Init(void)
{
	return ;
}

/**
 * Initialises the HAL timer of PN7462AU
 */
phStatus_t phPlatform_Port_Host_HwTimer_Init(void)
{
    memset(gphPlatform_Port_Pn7462Tmrs, 0, sizeof(gphPlatform_Port_Pn7462Tmrs));
    return phhalTimer_Init();
}


/**
 * Creates the timer based on the Unit provided and provides the available timer to the
 * Platform layer according to the Timer Id
 */
phStatus_t phPlatform_Port_Host_HwTimer_Create(uint32_t dwUnits, uint8_t bTimerId, void** ppTimerHandle)
{
    phStatus_t wStatus = PH_ERR_INTERNAL_ERROR;
    phhalTimer_Units_t phhalTimer_Units;
    uint8_t bCnt;

    switch(dwUnits)
    {
    case PH_PLATFORM_TIMER_UNIT_US:
        phhalTimer_Units = E_TUNIT_MICRO_SECS;
        break;
    case PH_PLATFORM_TIMER_UNIT_MS:
        phhalTimer_Units = E_TUNIT_MILLI_SECS;
        break;
    case PH_PLATFORM_TIMER_UNIT_S:
        phhalTimer_Units = E_TUNIT_SECS;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
        break;
    }

    wStatus = phhalTimer_RequestTimer(phhalTimer_Units, (phhalTimer_Timers_t **)ppTimerHandle);

    if(*ppTimerHandle == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    phPlatform_EnterCriticalSection();

    for(bCnt = 0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
    {
        if(gphPlatform_Port_Pn7462Tmrs[bCnt].p7462TimerHandle == NULL){
            gphPlatform_Port_Pn7462Tmrs[bCnt].p7462TimerHandle = *ppTimerHandle;
            gphPlatform_Port_Pn7462Tmrs[bCnt].bSwTimerId = bTimerId;
            break;
        }
    }

    phPlatform_ExitCriticalSection();

    return wStatus;
}

/**
 * Configures the Hardware timer for the provide time period
 */
phStatus_t phPlatform_Port_Host_HwTimer_Configure(void *pTimerHandle, uint32_t dwUnits, uint32_t dwTimePeriod)
{

    uint8_t bCnt;

    /* Find the bCnt*/
    phPlatform_EnterCriticalSection();

    for(bCnt = 0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
    {
        if(gphPlatform_Port_Pn7462Tmrs[bCnt].p7462TimerHandle == pTimerHandle){
            break;
        }
    }
    phPlatform_ExitCriticalSection();

    if(bCnt == PH_PLATFORM_MAX_TIMERS)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }


    return phhalTimer_Configure((phhalTimer_Timers_t *)pTimerHandle,  dwTimePeriod, (pphhalTimer_CallbackFun_t) paTimerHalCallBack[bCnt]);
}

/**
 * This api is used to start the HW timer based on one of the the two options
 */
phStatus_t phPlatform_Port_Host_HwTimer_Start(void * pTimerHandle, uint16_t wOption)
{
    phhalTimer_Mode_t phhalTimer_Mode;
    switch(wOption)
    {
    case PH_PLATFORM_TIMER_SINGLE_SHOT:
        phhalTimer_Mode = E_TIMER_SINGLE_SHOT;
        break;
    case PH_PLATFORM_TIMER_FREE_RUN:
        phhalTimer_Mode = E_TIMER_FREE_RUNNING;
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
        break;
    }
    return phhalTimer_Start((phhalTimer_Timers_t *)pTimerHandle, phhalTimer_Mode);
}

/**
 * API to get the running status of the timer, API details about the elapsed delay
 */
phStatus_t phPlatform_Port_Host_HwTimer_GetElapsedDelay(void * pTimerHandle, uint32_t dwUnits,
        uint32_t *pdwGetElapsedDelay)
{
    return phhalTimer_GetStatus((phhalTimer_Timers_t *)pTimerHandle, pdwGetElapsedDelay);
}

/**
 * This API stops the timer whose handle is provided
 */
phStatus_t phPlatform_Port_Host_HwTimer_Stop(void * pTimerHandle)
{
    return phhalTimer_Stop((phhalTimer_Timers_t *)pTimerHandle);
}


/**
 * This API resets the timer. It first stops the timer and then resets it to Zero
 */
phStatus_t phPlatform_Port_Host_HwTimer_Reset(void * pTimerHandle)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
    wStatus = phhalTimer_Stop((phhalTimer_Timers_t *)pTimerHandle);
    return wStatus;
}


/**
 * This api releases the hal timer which means deletes the SW timer whose handle
 * is provided
 */
phStatus_t phPlatform_Port_Host_HwTimer_Delete(void * pTimerHandle)
{

    uint8_t bCnt = 0;
    phPlatform_EnterCriticalSection();

    for(bCnt = 0; bCnt < PH_PLATFORM_MAX_TIMERS; bCnt++)
    {
        if(gphPlatform_Port_Pn7462Tmrs[bCnt].p7462TimerHandle == pTimerHandle){
            gphPlatform_Port_Pn7462Tmrs[bCnt].p7462TimerHandle = NULL;
            gphPlatform_Port_Pn7462Tmrs[bCnt].bSwTimerId = 0;
            break;
        }
    }

    phPlatform_ExitCriticalSection();
    return phhalTimer_ReleaseTimer((phhalTimer_Timers_t *)pTimerHandle);
}


/**
 * Deinitialises the HW timer module
 */
void phPlatform_Port_Host_HwTimer_DeInit(void)
{
    phhalTimer_DeInit();
}

/**
 * This api initialises the tick timer for OS
 */
phStatus_t phPlatform_InitTickTimer(pphPlatform_TickTimerISRCallBck_t pTickTimerCallback)
{
    pTickCallBack = pTickTimerCallback;

    qwLoadValue = 0;

    /* SysTick Timer rate is system clock rate. */
    dwSysTickTimerFreq = 2000000;

    /* Disable systick and clear the Load value. */
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD) = 0x0;

    return PH_ERR_SUCCESS;
}


/**
 * This api configures the tick timer for the OS
 */
static void phPlatform_ConfigTick(void)
{
    /* Disable systick */
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;

    /* Configure SysTick count down value to interrupt at the requested time. */
    if(qwLoadValue > PH_RTOS_NORTOS_MAX_SYSTICK)
    {
        qwLoadValue -= PH_RTOS_NORTOS_MAX_SYSTICK;
        *(PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD) = PH_RTOS_NORTOS_MAX_SYSTICK;
    }
    else
    {
        *(PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD) = (uint32_t)(qwLoadValue & PH_RTOS_NORTOS_MAX_SYSTICK);
        qwLoadValue = 0;
    }

    /*Clear the current count value and also SysTick CTRL.COUNTFLAG. */
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CUR_VAL) = 0;

    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = PH_RTOS_NORTOS_NVIC_SYSTICK_CLK | PH_RTOS_NORTOS_NVIC_SYSTICK_INT | PH_RTOS_NORTOS_NVIC_SYSTICK_ENABLE;
}


/**
 * This API is used to start the TICK timer of the OS
 */
phStatus_t phPlatform_StartTickTimer(uint32_t dwTimeMilliSecs)
{
    qwLoadValue = ((uint64_t)dwTimeMilliSecs * (uint64_t)dwSysTickTimerFreq)/PH_PLATFORM_TIMER_UNIT_MS;

    phPlatform_ConfigTick();

    return PH_ERR_SUCCESS;
}


/**
 * This api is used to stop the tick timer of the OS
 */
phStatus_t phPlatform_StopTickTimer(void)
{
    /* Disable systick and clear the Load value. */
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_CTRL) = 0x0;
    *(PH_RTOS_NORTOS_NVIC_SYSTICK_LOAD) = 0x0;

    return PH_ERR_SUCCESS;
}


/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

static void phPlatform_Timer0_CallbackFun(void)
{
    phPlatform_Timer_IsrHandler(gphPlatform_Port_Pn7462Tmrs[0].bSwTimerId);
}

static void phPlatform_Timer1_CallbackFun(void)
{
    phPlatform_Timer_IsrHandler(gphPlatform_Port_Pn7462Tmrs[0].bSwTimerId);
}

static void phPlatform_Timer2_CallbackFun(void)
{
    phPlatform_Timer_IsrHandler(gphPlatform_Port_Pn7462Tmrs[0].bSwTimerId);
}

static void phPlatform_Timer3_CallbackFun(void)
{
    phPlatform_Timer_IsrHandler(gphPlatform_Port_Pn7462Tmrs[0].bSwTimerId);
}

#endif /* NXPBUILD__PH_PN7462AU */
/******************************************************************************
**                            End Of File
******************************************************************************/
