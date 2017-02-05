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
* Example Source for Nfcrdlib_SimplifiedAPI_EMVCo.
* This application will configure Reader Library as per Emvco specification and start Emvco polling.
* This loop back application will send SELECT_PPSE command and is used to test Emvco2.3.1a(L1)
* digital compliance.
* Please refer Readme.txt file  for  Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Purnank G (ing05193) $
* $Revision: 5993 $ (v4.040.05.011646)
* $Date: 2016-09-19 13:50:28 +0530 (Mon, 19 Sep 2016) $
*
* History:
* BK: Generated 12. Jun 2014
* PC: Generated 25. Nov 2012
*
*/

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

/* NFCLIB Header */
#include <phNfcLib.h>

/* Local headers */
#include "Nfcrdlib_SimplifiedAPI_EMVCo.h"
#include "postSend.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

uint8_t  response_buffer[PHAC_EMVCO_MAX_BUFFSIZE];
uint16_t respsize = sizeof(response_buffer);

phNfcLib_PeerInfo_t                sNfcLib_Info;                                   /* NFC LIB component holder */

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aEmvcoLoopBackTaskBuffer[EMVCO_LOOPBACK_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aEmvcoLoopBackTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Static Defines
*******************************************************************************/
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x00, 0x05, 0xF2, 0x22, 0x22, 0x22, 0x22};

/*******************************************************************************
**   Function Declaration
*******************************************************************************/
void Emvco_LoopBack(void * pHalParams);
static phStatus_t EmvcoRfReset(uint32_t delayInMs);

/*******************************************************************************
**   Function Definitions
*******************************************************************************/

/*******************************************************************************
**   Main Function
*******************************************************************************/

int main (void)
{
    do
    {
        phNfcLib_Status_t dwStatus;

        /* Initialize the Controller */
        phPlatform_Controller_Init();

        /* Initialize library */
        dwStatus = phNfcLib_Init();
        CHECK_NFCLIB_STATUS(dwStatus);
        if(dwStatus != PH_NFCLIB_STATUS_SUCCESS) break;

        DEBUG_PRINTF("\n Emvco compliance example: ");

#ifndef NXPBUILD__PH_OSAL_NULLOS

        phOsal_ThreadObj_t EmvcoLoopBack;

        EmvcoLoopBack.pTaskName = (uint8_t *) "EmvcoLoopBack";
        EmvcoLoopBack.pStackBuffer = aEmvcoLoopBackTaskBuffer;
        EmvcoLoopBack.priority = EMVCO_LOOPBACK_TASK_PRIO;
        EmvcoLoopBack.stackSizeInNum = EMVCO_LOOPBACK_TASK_STACK;
        phOsal_ThreadCreate(&EmvcoLoopBack.ThreadHandle, &EmvcoLoopBack, &Emvco_LoopBack, NULL);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        Emvco_LoopBack(NULL);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void Emvco_LoopBack(void * pHalParams)
{
    phNfcLib_Status_t   dwStatus;
    uint16_t            wRfTechnos = PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_A | PH_NFCLIB_TECHNOLOGY_INITIATOR_ISO_14443_B;
    uint16_t            cmdsize;
    uint8_t             bEndOfLoopBack = 0;
    uint8_t             bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;
    uint8_t             bEndOfTestAnalog = PH_OFF;

    /* Load Emvco Default setting */
    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_EMVCO);
    CHECK_NFCLIB_STATUS(dwStatus);

    dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_BLOCKING, PH_ON);
    CHECK_NFCLIB_STATUS(dwStatus);

    while (1)
    {
        dwStatus = phNfcLib_Activate(wRfTechnos, &sNfcLib_Info, NULL);
        if (dwStatus == PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
        {
            dwStatus = phNfcLib_Transmit(PPSE_SELECT_APDU, sizeof(PPSE_SELECT_APDU));
            if (dwStatus == PH_NFCLIB_STATUS_SUCCESS)
            {
                respsize = sizeof(response_buffer);
                dwStatus = phNfcLib_Receive(response_buffer, &respsize, NULL);
                if ((dwStatus == PH_NFCLIB_STATUS_SUCCESS) && (respsize > 0))
                {
			DEBUG_PRINTF("\n ");
			int i;
			for(i=0;i<respsize-2;i++)
			{
				DEBUG_PRINTF("%c",response_buffer[i]);
			}

		}
	    }
        }
        dwStatus = phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_DEACTIVATION_BLOCKING, PH_ON);
        CHECK_NFCLIB_STATUS(dwStatus);

        dwStatus = phNfcLib_Deactivate(bRemovalProcedure, NULL);
        CHECK_NFCLIB_STATUS(dwStatus);

        bRemovalProcedure = PH_NFCLIB_DEACTIVATION_MODE_RF_OFF;
    }
}

/**
* \brief Perform RF Reset for the specified milli-seconds duration.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoRfReset(uint32_t delayInMs)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;

    /*RF Field OFF*/
    wStatus = phhalHw_FieldOff(phNfcLib_GetDataParams(PH_COMP_HAL));
    CHECK_STATUS(wStatus);

    wStatus = phhalHw_Wait(phNfcLib_GetDataParams(PH_COMP_HAL), PHHAL_HW_TIME_MILLISECONDS, delayInMs);
    CHECK_STATUS(wStatus);

    /*RF Field ON*/
    wStatus = phhalHw_FieldOn(phNfcLib_GetDataParams(PH_COMP_HAL));
    CHECK_STATUS(wStatus);

    return wStatus;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
