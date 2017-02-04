/*
 * phPlatform_Port_LinuxPi.c
 *
 *  Created on: May 24, 2016
 *      Author: nxp69678
 */


#include <ph_Status.h>

#ifdef NXPBUILD__PH_RASPBERRY_PI

#include <phPlatform.h>
#include <phPlatform_Port_Host.h>
#include <phhalHw_Rc523_Reg.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

phOsal_ThreadObj_t gphPiThreadObj;

#define PHPLATFORM_PORT_PI_IRQ_POLLING_TASK_PRIO    0
#define PHPLATFORM_PORT_PI_IRQ_POLLING_TASK_STACK   0x20000

typedef struct piTimer_config{
    timer_t pPiTimer;
    unsigned int dwTimePeriod;
    unsigned int dwUnits;
}piTimer_config_t;

static void phPlatform_Port_TimerCallBack(union sigval uTimerSignal);

void phPlatform_Controller_Init(void)
{
	return ;
}

void phPlatform_Config_FE_Device(phPlatform_DataParams_t * pHal)
{
#ifdef NXPBUILD__PHHAL_HW_RC523
    /* Add DG: Configure IRQ pin - required for Explore-NFC board */
    phhalHw_Rc523_WriteRegister(&pHal->sHal, PHHAL_HW_RC523_REG_COMMIEN, 0);
    phhalHw_Rc523_WriteRegister(&pHal->sHal, PHHAL_HW_RC523_REG_DIVIEN, PHHAL_HW_RC523_BIT_IRQPUSHPULL);
#endif /* NXPBUILD__PHHAL_HW_RC523 */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    uint8_t param = 0;
    phhalHw_Pn5180_Instr_ReadE2Prom(&pHal->sHal, 0x1a, &param, 1);
    if (param != 0x1)
    {
        //Configure IRQ pin polarity & clearing method (IRQ pin active high, NON auto-clear)
        param = 0x1;
        phhalHw_Pn5180_Instr_WriteE2Prom(&pHal->sHal, 0x1a, &param, 1);
        /* another read required before reset, to ensure parameter is written. Otherwise the reset could happen whilst write operation is still ongoing (takes ~3ms) */
        phhalHw_Pn5180_Instr_ReadE2Prom(&pHal->sHal, 0x1a, &param, 1);

        /* Reset Pn512 Front-end. */
        phPlatform_FE_Reset();

    }
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_RC663
    phStatus_t status;
    /*
     * Set irq0en (08h) register bit 8 to 0 to configure interrupt as active high
     * */
    phhalHw_Rc663_WriteRegister(&pHal->sHal, PHHAL_HW_RC663_REG_IRQ0EN, 0x00);
#endif /* NXPBUILD__PHHAL_HW_RC663 */
}

/*
 * \brief: The purpose of this Thread is to detect RF signal from an External Peer .
 */
void phPlatform_Port_Host_IrqPolling(void* param)
{
    uint8_t bgpioVal = 0;
    uint8_t bhighOrLow = 0;

    if(IRQ_PIN_TRIGGER_TYPE ==  PHPLATFORM_PORT_INT_RISING_EDGE)
    {
        bhighOrLow = 1;
    }

    if(PiGpio_read(PIN_IRQ, &bgpioVal) != PH_ERR_SUCCESS)
    {
#if 0
        PiGpio_unexport(PIN_IRQ);
        PiGpio_export(PIN_IRQ);
        PiGpio_set_direction(PIN_IRQ, false);

        if(INT_TRIGGER_TYPE ==  PHPLATFORM_PORT_INT_RISING_EDGE)
        {
            PiGpio_set_edge(PIN_IRQ, true, false);
        }
        else
        {
            PiGpio_set_edge(PIN_IRQ, false, true);
        }

#endif
    }

    /* Initial status: If pin is already Active, post an event. */
    if(bgpioVal == bhighOrLow)
    {
        RF_IRQ_Handler();
    }

    while(1)
    {
        /* Block forever for Raising Edge in PIN_IRQ. */
        if(PiGpio_poll(PIN_IRQ, bhighOrLow, -1) != PH_ERR_SUCCESS)
        {
#if 0
            PiGpio_unexport(PIN_IRQ);

            PiGpio_export(PIN_IRQ);

            PiGpio_set_direction(PIN_IRQ, false);

            if(INT_TRIGGER_TYPE ==  PHPLATFORM_PORT_INT_RISING_EDGE)
            {
                PiGpio_set_edge(PIN_IRQ, true, false);
            }
            else
            {
                PiGpio_set_edge(PIN_IRQ, false, true);
            }

#endif
        }


        RF_IRQ_Handler();
    }
}

phStatus_t phPlatform_Port_Host_SetPinConfig(phPlatform_Port_Host_config_t bConfig)
{
    phStatus_t wStatus;
    int ret;

    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_RESET:
        /** Configure Reset . */
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_RESET));

        wStatus = PiGpio_set_direction(PIN_RESET, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_RESET);
            return wStatus;
        }
        break;

    case PHPLATFORM_PORT_PIN_IRQ:
        /** Configure IRQ pin to look for Raising edge. */
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_IRQ));

        wStatus, PiGpio_set_direction(PIN_IRQ, false);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_IRQ);
            return wStatus;
        }

        if(IRQ_PIN_TRIGGER_TYPE ==  PHPLATFORM_PORT_INT_RISING_EDGE)
        {
            wStatus = PiGpio_set_edge(PIN_IRQ, true, false);
        }
        else
        {
            wStatus = PiGpio_set_edge(PIN_IRQ, false, true);
        }
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_IRQ);
            return wStatus;
        }

        phPlatform_Config_FE_Device(gpphPlatform_DataParams);

        gphPiThreadObj.pTaskName = (uint8_t *) "IrqPolling";
        gphPiThreadObj.pStackBuffer = NULL;
        gphPiThreadObj.priority = PHPLATFORM_PORT_PI_IRQ_POLLING_TASK_PRIO;
        gphPiThreadObj.stackSizeInNum = PHPLATFORM_PORT_PI_IRQ_POLLING_TASK_STACK;
        PH_CHECK_SUCCESS_FCT(wStatus, phOsal_ThreadCreate(&gphPiThreadObj.ThreadHandle, &gphPiThreadObj,
            &phPlatform_Port_Host_IrqPolling, NULL));

        break;

    case PHPLATFORM_PORT_PIN_SPI:
        wStatus = PH_ERR_SUCCESS;
        break;

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_I2C:
        wStatus = PH_ERR_SUCCESS;
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_BUSY));

        wStatus = PiGpio_set_direction(PIN_BUSY, false);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_BUSY);
            return wStatus;
        }

        if(BUSY_PIN_TRIGGER_TYPE ==  PHPLATFORM_PORT_INT_RISING_EDGE)
        {
            wStatus = PiGpio_set_edge(PIN_BUSY, true, false);
        }
        else
        {
            wStatus = PiGpio_set_edge(PIN_BUSY, false, true);
        }
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_BUSY);
            return wStatus;
        }
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        wStatus = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_IFSEL0));

        wStatus = PiGpio_set_direction(PIN_IFSEL0, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_IFSEL0);
            return wStatus;
        }
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_IFSEL1));

        wStatus = PiGpio_set_direction(PIN_IFSEL1, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_IFSEL1);
            return wStatus;
        }
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_AD0));

        wStatus = PiGpio_set_direction(PIN_AD0, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_AD0);
            return wStatus;
        }
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_AD1));

        wStatus = PiGpio_set_direction(PIN_AD1, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_AD1);
            return wStatus;
        }
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        PH_CHECK_SUCCESS_FCT(wStatus, PiGpio_export(PIN_AD2));

        wStatus = PiGpio_set_direction(PIN_AD2, true);
        if (wStatus != PH_ERR_SUCCESS)
        {
            PiGpio_unexport(PIN_AD2);
            return wStatus;
        }
        break;
#endif /* NXPBUILD__PHHAL_HW_RC523 */
    default:
        break;
    }

    return wStatus;
}

void phPlatform_Port_Host_SetPinValue(phPlatform_Port_Host_config_t bConfig, uint8_t bVal)
{
    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_RESET:
        (void)PiGpio_Write(PIN_RESET, bVal);
        break;

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        (void)PiGpio_Write(PIN_IFSEL0, bVal);
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
        (void)PiGpio_Write(PIN_IFSEL1, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        break;
#endif /* NXPBUILD__PHHAL_HW_RC523 */
#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_DWL:
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
    default:
        break;
    }
}

bool phPlatform_Port_Host_GetPinValue(phPlatform_Port_Host_config_t bConfig)
{
    uint8_t bStatus = false;

    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_IRQ:
        (void)PiGpio_read(PIN_IRQ, &bStatus);
        break;
#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        (void)PiGpio_read(PIN_BUSY, &bStatus);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */
    default:
        break;
    }

    return (bool)(bStatus);
}

void phPlatform_Port_Host_ClearInt()
{
    return;
}

phStatus_t phPlatform_Port_Host_HwTimer_Init(void)
{
    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Create(uint32_t dwUnits, uint8_t bTimerId, void** ppTimerHandle)
{
    struct sigevent sSigEvent;

    piTimer_config_t *pTimerCfg;

    /* Allocate piTimer_config_t */
    pTimerCfg = malloc( sizeof(piTimer_config_t) );
    if( pTimerCfg == NULL )
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    memset(&sSigEvent, 0, sizeof(sSigEvent));
    sSigEvent.sigev_notify  = SIGEV_THREAD;
    sSigEvent.sigev_value.sival_int = bTimerId;
    sSigEvent.sigev_notify_function = phPlatform_Port_TimerCallBack;
    sSigEvent.sigev_notify_attributes = NULL;

    if(timer_create(CLOCK_REALTIME, &sSigEvent, &pTimerCfg->pPiTimer) !=0)
    {
        *ppTimerHandle = NULL;
        free(pTimerCfg);
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    pTimerCfg->dwTimePeriod = 0;
    pTimerCfg->dwUnits = dwUnits;
    *ppTimerHandle = pTimerCfg;

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Configure(void *pTimerHandle, uint32_t dwUnits, uint32_t dwTimePeriod)
{
    if ((pTimerHandle == NULL) || (((piTimer_config_t *)pTimerHandle)->dwUnits != dwUnits))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    ((piTimer_config_t *)pTimerHandle)->dwTimePeriod = dwTimePeriod;

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Start(void * pTimerHandle, uint16_t wOption)
{
    /* Disarm the Timer. */
    struct itimerspec xTimerVal = {{0, 0}, {0, 0}};
    piTimer_config_t *pTimerCfg = pTimerHandle;


    if(pTimerHandle == NULL){
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    if(pTimerCfg->dwUnits == PH_PLATFORM_TIMER_UNIT_S)
    {
        xTimerVal.it_value.tv_sec = pTimerCfg->dwTimePeriod;
    }
    else
    {
        /* convert milli/micro to nano secs. */
        xTimerVal.it_value.tv_nsec = (pTimerCfg->dwUnits == PH_PLATFORM_TIMER_UNIT_MS)?
                pTimerCfg->dwTimePeriod * 1000000: pTimerCfg->dwTimePeriod * 1000;
    }

    /* Setup timer to restart when match occurs, Free Running mode. */
    if(wOption & PH_PLATFORM_TIMER_FREE_RUN)
    {
        xTimerVal.it_interval.tv_sec = xTimerVal.it_value.tv_sec;
        xTimerVal.it_interval.tv_nsec = xTimerVal.it_value.tv_nsec;
    }

    if(timer_settime(pTimerCfg->pPiTimer, 0, &xTimerVal, NULL) !=0)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_GetElapsedDelay(void * pTimerHandle, uint32_t dwUnits, uint32_t *pdwGetElapsedDelay)
{
    struct itimerspec xTimerVal = {{0, 0}, {0, 0}};
    piTimer_config_t *pTimerCfg = pTimerHandle;

    if((pTimerCfg->dwUnits != dwUnits) || (pTimerHandle == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    if(timer_gettime(pTimerCfg->pPiTimer, &xTimerVal) !=0)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    /* check Currently timer is Disarmed. */
    if((xTimerVal.it_value.tv_nsec == 0) && (xTimerVal.it_value.tv_sec == 0)){
        *pdwGetElapsedDelay = xTimerVal.it_value.tv_nsec;
        return PH_ERR_SUCCESS;
    }

    if(dwUnits == PH_PLATFORM_TIMER_UNIT_S)
    {
        *pdwGetElapsedDelay = (long)pTimerCfg->dwTimePeriod - xTimerVal.it_value.tv_sec;
    }
    else
    {
        /* Convert nano to millli/micro secs. */
        xTimerVal.it_value.tv_nsec = (dwUnits == PH_PLATFORM_TIMER_UNIT_MS)? xTimerVal.it_value.tv_nsec/1000000
                : xTimerVal.it_value.tv_nsec/1000;

        *pdwGetElapsedDelay = (long)pTimerCfg->dwTimePeriod - xTimerVal.it_value.tv_nsec;
    }
    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Stop(void * pTimerHandle)
{
    /* Disarm the Timer. */
    struct itimerspec xTimerVal = {{0, 0}, {0, 0}};

    if(pTimerHandle == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    if(timer_settime(((piTimer_config_t *)pTimerHandle)->pPiTimer, 0, &xTimerVal, NULL) !=0)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Reset(void * pTimerHandle)
{
    /* Disarm the Timer. */
    struct itimerspec xTimerVal = {{0, 0}, {0, 0}};
    piTimer_config_t * pTimerCfg = pTimerHandle;

    if (pTimerHandle == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    pTimerCfg->dwTimePeriod = 0;

    if(timer_settime(pTimerCfg->pPiTimer, 0, &xTimerVal, NULL) !=0)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phPlatform_Port_Host_HwTimer_Delete(void * pTimerHandle)
{
    piTimer_config_t *pTimerCfg = pTimerHandle;

    if(pTimerHandle == NULL){
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PLATFORM);
    }

    if(timer_delete(pTimerCfg->pPiTimer) != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_PLATFORM);
    }

    free(pTimerCfg);

    return PH_ERR_SUCCESS;
}

void phPlatform_Port_Host_HwTimer_DeInit(void)
{
    return ;
}

static void phPlatform_Port_TimerCallBack(union sigval uTimerSignal)
{
    phPlatform_Timer_IsrHandler(uTimerSignal.sival_int);
}
#endif /* NXPBUILD__PH_RASPBERRY_PI */
