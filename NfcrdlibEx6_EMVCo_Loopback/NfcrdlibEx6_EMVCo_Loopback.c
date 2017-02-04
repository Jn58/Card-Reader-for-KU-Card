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
* Example Source for NfcrdlibEx6_EMVCo_Loopback.
* This application will configure Reader Library as per Emvco specification and start Emvco polling.
* This loop back application will send SELECT_PPSE command and is used to test Emvco2.3.1a(L1)
* digital compliance.
* Please refer Readme.txt file  for  Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Purnank G (ing05193) $
* $Revision: 6114 $ (v4.040.05.011646)
* $Date: 2016-09-22 16:56:53 +0530 (Thu, 22 Sep 2016) $
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

/* Local headers */
#include "NfcrdlibEx6_EMVCo_Loopback.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

/*HAL variables*/
uint8_t                            bHalBufferTx[PHAC_EMVCO_MAX_BUFFSIZE];          /* HAL TX buffer. Size 256 - Based on maximum FSL */
uint8_t                            bHalBufferRx[PHAC_EMVCO_MAX_BUFFSIZE];          /* HAL RX buffer. Size 256 - Based on maximum FSL */

uint8_t                            command_buffer[PHAC_EMVCO_MAX_BUFFSIZE];
uint8_t                            *response_buffer;

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aEmvcoLoopBackTaskBuffer[EMVCO_LOOPBACK_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aEmvcoLoopBackTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Static Defines
*******************************************************************************/
/* EMVCo: Select PPSE Command */
static uint8_t PPSE_SELECT_APDU[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59,
        0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };

/*******************************************************************************
**   Function Declaration
*******************************************************************************/
void Emvco_LoopBack(void * pHalParams);
static phStatus_t LoadEmvcoSettings();
static phStatus_t EmvcoRfReset(uint32_t delayInMs);
static phStatus_t EmvcoDataLoopBack(phacDiscLoop_Sw_DataParams_t * pDataParams);
static phStatus_t EmvcoDataExchange(uint8_t * com_buffer, uint8_t cmdsize, uint8_t ** resp_buffer, uint32_t * wRxLength);

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
        phStatus_t status = PH_ERR_INTERNAL_ERROR;

        /* Initialize the Controller */
        phPlatform_Controller_Init();

        DEBUG_PRINTF("\n Emvco compliance example: ");

        phOsal_Init();

        /* Perform Platform Init */
        status = phPlatform_Init(&sPlatform, bHalBufferTx, sizeof(bHalBufferTx), bHalBufferRx, sizeof(bHalBufferRx));
        CHECK_STATUS(status);
        if(status != PH_ERR_SUCCESS) break;

        /* Initialize Reader Library PAL/AL Components */
        status = phApp_RdLibInit();
        CHECK_STATUS(status);

        if(status != PH_ERR_SUCCESS) break;

        /* Set the generic pointer */
        pHal = &sPlatform.sHal;

#ifndef NXPBUILD__PH_OSAL_NULLOS

        phOsal_ThreadObj_t EmvcoLoopBack;

        EmvcoLoopBack.pTaskName = (uint8_t *) "EmvcoLoopBack";
        EmvcoLoopBack.pStackBuffer = aEmvcoLoopBackTaskBuffer;
        EmvcoLoopBack.priority = EMVCO_LOOPBACK_TASK_PRIO;
        EmvcoLoopBack.stackSizeInNum = EMVCO_LOOPBACK_TASK_STACK;
        phOsal_ThreadCreate(&EmvcoLoopBack.ThreadHandle, &EmvcoLoopBack, &Emvco_LoopBack, &sPlatform.sHal);

        phOsal_StartScheduler();

        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        Emvco_LoopBack(&sPlatform.sHal);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

void Emvco_LoopBack(void * pHalParams)
{
    phStatus_t  status;
    uint16_t    wTagsDetected = 0;
    uint8_t     bCidEnabled;
    uint8_t     bCid;
    uint8_t     bNadSupported;
    uint8_t     bFwi;
    uint8_t     bFsdi;
    uint8_t     bFsci;

    phacDiscLoop_Sw_DataParams_t * pDataParams;

    pDataParams = &sDiscLoop;

    /* Load Emvco Default setting */
    status = LoadEmvcoSettings();
    CHECK_STATUS(status);

    /* Perform RF Reset */
    status = EmvcoRfReset(5100);
    CHECK_STATUS(status);

    /* Disable Emvco Emd Error check */
    status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_SET_EMD, PH_OFF);
    CHECK_STATUS(status);

    status = PHAC_DISCLOOP_NO_TECH_DETECTED;
    while(1)
    {
        do
        {
            /* If no card is been found then RF reset should not be performed. */
            if((status & PH_ERR_MASK) != PHAC_DISCLOOP_NO_TECH_DETECTED)
            {
                /* Perform RF Reset */
                status = EmvcoRfReset(5100);
                CHECK_STATUS(status);
            }

            /* Set discovery loop poll state */
            status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
            CHECK_STATUS(status);

            /* Start Polling, Function will return once card is activated or any other error has occurred */
            status = phacDiscLoop_Run(pDataParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);

        } while((status & PH_ERR_MASK) != PHAC_DISCLOOP_DEVICE_ACTIVATED); /* Exit on Card detection */

        status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
        CHECK_STATUS(status);

        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
        {
            /* Retrieve 14443-4A protocol parameter */
            status = phpalI14443p4a_GetProtocolParams(
                pDataParams->pPal1443p4aDataParams,
                &bCidEnabled,
                &bCid,
                &bNadSupported,
                &bFwi,
                &bFsdi,
                &bFsci);
            CHECK_STATUS(status);

            /* Set 14443-4 protocol parameter */
            status = phpalI14443p4_SetProtocol(
                pDataParams->pPal14443p4DataParams,
                PH_OFF,
                bCid,
                PH_OFF,
                PH_OFF,
                bFwi,
                bFsdi,
                bFsci);
            CHECK_STATUS(status);
        }

        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
        {
            /* Retrieve 14443-3b protocol parameter */
            status = phpalI14443p3b_GetProtocolParams(
                pDataParams->pPal1443p3bDataParams,
                &bCidEnabled,
                &bCid,
                &bNadSupported,
                &bFwi,
                &bFsdi,
                &bFsci);
            CHECK_STATUS(status);

            /* Set 14443-4 protocol parameter */
            status = phpalI14443p4_SetProtocol(
                pDataParams->pPal14443p4DataParams,
                PH_OFF,
                bCid,
                PH_OFF,
                PH_OFF,
                bFwi,
                bFsdi,
                bFsci);
            CHECK_STATUS(status);
        }

        /* Send SELECT_PPSE command and start exchange I-PDU */
        status = EmvcoDataLoopBack(pDataParams);
    }
}

/**
* \brief Perform RF Reset as per Emvco Specification
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoRfReset(uint32_t delayInMs)
{
    phStatus_t status = PH_ERR_SUCCESS;

    /*RF Field OFF*/
    status = phhalHw_FieldOff(sDiscLoop.pHalDataParams);
    CHECK_STATUS(status);

    status = phhalHw_Wait(sDiscLoop.pHalDataParams,PHHAL_HW_TIME_MICROSECONDS, delayInMs);
    CHECK_STATUS(status);

    /*RF Field ON*/
    status = phhalHw_FieldOn(sDiscLoop.pHalDataParams);
    CHECK_STATUS(status);

    return status;
}

/**
* \brief Exchange Data APDU Packets for EMVCO (ISO14443-4 Exchange)
* This function will Exchange APDU data packets provided by Loop-Back Application
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoDataExchange(uint8_t * com_buffer, uint8_t cmdsize, uint8_t ** resp_buffer, uint32_t * wRxLength)
{
    phStatus_t status;
    uint8_t *ppRxBuffer;
    uint16_t wRxLen = 0;

    status = phpalI14443p4_Exchange(&spalI14443p4, PH_EXCHANGE_DEFAULT, com_buffer, cmdsize, &ppRxBuffer, &wRxLen);
    if (PH_ERR_SUCCESS == status)
    {
        /* set the pointer to the start of the R-APDU */
        *resp_buffer = &ppRxBuffer[0];
    }
    else
    {
        /* Exchange not successful, reset the number of rxd bytes */
        wRxLen = 0x00;
    }

    *wRxLength = wRxLen;

    return status;
}

/**
* \brief EMVCo Loop-Back function
* This Loop-Back function converts each received R-APDU into the next C-APDU (by stripping the
* status words), and sends this C-APDU back to the card simulator.
* Also this function send SELECT_PPSE command after card activation.
* Loop-Back Function exist when EOT (End Of Test) Command is received from the card simulator.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
static phStatus_t EmvcoDataLoopBack(phacDiscLoop_Sw_DataParams_t * pDataParams)
{

    uint32_t cmdsize, respsize;
    phStatus_t status;
    uint8_t bEndOfLoopBack = 0;
    uint8_t bRemovalProcedure = PH_OFF;
    uint8_t bEndOfTestAnalog = PH_OFF;
    cmdsize = sizeof(PPSE_SELECT_APDU);
    status = EmvcoDataExchange(PPSE_SELECT_APDU, cmdsize, &response_buffer, &respsize);

    while (!bEndOfLoopBack)
    {
        if (respsize > 0)
        {
            if (respsize >= MiN_VALID_DATA_SIZE)
            {
                /* EOT (End Of Test) Command. Exit the loop */
                if (eEmdRes_EOT == response_buffer[1])
                {
                    /* Second byte = 0x70, stop the loopback */
                    bEndOfLoopBack = 1;
                    bRemovalProcedure = PH_ON;
                }
                /* EOT (End Of Test) Command used for analog tests. Exit the loop, perform long rf reset. */
                else if (eEmdRes_EOT_AnalogTests == response_buffer[1])
                {
                    /* Second byte = 0x72, stop the loopback */
                    bEndOfLoopBack = 1;
                    bEndOfTestAnalog = PH_ON;
                }
                else if (eEmdRes_SW_0 == response_buffer[respsize - 2])
                {
                    /* Format the card response into a new command without the status word 0x90 0x00 */
                    cmdsize = respsize - 2;  /* To Remove two bytes of status word */
                    memcpy(command_buffer, response_buffer, cmdsize);

                    /* Send back(Command) : Received Response - Status_Word */
                    status = EmvcoDataExchange(command_buffer, cmdsize, &response_buffer, &respsize);
                }
                else
                {
                    /* error Abort Loopback */
                    bEndOfLoopBack = 1;
                }
            }/*if (respsize >= 6)*/
            else
            {
                /* re-send the select appli APDU */
                status = EmvcoDataExchange(PPSE_SELECT_APDU, cmdsize, &response_buffer, &respsize);
                if (respsize == 0)
                {
                    bEndOfLoopBack = 1;
                }
            }
        }/*if(respsize > 0)*/
        else
        {
            bEndOfLoopBack = 1;
        }
    }/*while (!bEndOfLoopBack)*/

    if (bEndOfTestAnalog == PH_ON)
    {
        /* analog tests - end of tests. long rf reset is required. */
        EmvcoRfReset(25000);
    }

    if(bRemovalProcedure == PH_ON)
    {
        /* Set Poll state to perform Tag removal procedure*/
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_REMOVAL);
        CHECK_STATUS(status);

        status = phacDiscLoop_Run(pDataParams, PHAC_DISCLOOP_ENTRY_POINT_POLL);
        bRemovalProcedure = PH_OFF;
    }
    return status;
}

/**
* This function will load/configure Discovery loop and 14443 PAL with default values to support Emvco2.3.1a(L1) digital compliance.
 * Application can read these values from EEPROM area and load/configure Discovery loop via SetConfig
* \param   bProfile      Reader Library Profile
* \note    Values used below are default and is for demonstration purpose.
*/
static phStatus_t LoadEmvcoSettings()
{
    phStatus_t status = PH_ERR_SUCCESS;

    /* Passive Bailout bitmap config. */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, 0x00);
    CHECK_STATUS(status);

    /* Passive CON_DEVICE limit. */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 1);
    CHECK_STATUS(status);

    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DEVICE_LIMIT, 1);
    CHECK_STATUS(status);

#ifdef NXPBUILD__PHHAL_HW_PN5180
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_GTA_VALUE_US, 4000);
    CHECK_STATUS(status);

    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_GTB_VALUE_US, 4000);
    CHECK_STATUS(status);
#else
    /* Passive polling Tx Guard times in micro seconds. */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_GTA_VALUE_US, 5100);
    CHECK_STATUS(status);

    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_GTB_VALUE_US, 5100);
    CHECK_STATUS(status);
#endif

    /* Configure FSDI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI, 0x08);
    CHECK_STATUS(status);

    /* Configure CID for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_CID, 0x00);
    CHECK_STATUS(status);

    /* Configure DRI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DRI, 0x00);
    CHECK_STATUS(status);

    /* Configure DSI for the 14443P4A tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_DSI, 0x00);
    CHECK_STATUS(status);

    /* Configure AFI for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ, 0x00);
    CHECK_STATUS(status);

    /* Configure FSDI for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_FSDI, 0x08);
    CHECK_STATUS(status);

    /* Configure CID for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_CID, 0x00);
    CHECK_STATUS(status);

    /* Configure DRI for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DRI, 0x00);
    CHECK_STATUS(status);

    /* Configure DSI for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_DSI, 0x00);
    CHECK_STATUS(status);

    /* Configure Extended ATQB support for the type B tags */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB, 0x00);
    CHECK_STATUS(status);

    /* Configure reader library mode */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_OPE_MODE, RD_LIB_MODE_EMVCO);
    CHECK_STATUS(status);

    return status;
}

/* Stubs, in case the phApp_Init.c expects these implementations */
#ifdef NXPBUILD__PHPAL_I14443P4MC_SW
/*
 * WTX Callback called from WTX timer of 14443p3mC PAL.
 */
void pWtoxCallBck(uint8_t bTimerId)
{
  /* Dummy */
}

uint8_t aAppHCEBuf[32];
uint16_t wAppHCEBuffSize = sizeof(aAppHCEBuf);
#endif /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_I18092MT_SW
void pRtoxCallBck(uint8_t bTimerId)
{
  /* Dummy */
}
#endif /* NXPBUILD__PHPAL_I18092MT_SW */

#ifdef NXPBUILD__PHHAL_HW_TARGET
/* Stubbed definitions in case TARGET is enabled */
uint8_t  sens_res[2]     = {0x04, 0x00};
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};
uint8_t  sel_res         = 0x40;
uint8_t  nfc_id3         = 0xFA;
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                                   0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                                   0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45 };
#endif /* NXPBUILD__PHHAL_HW_TARGET */

/******************************************************************************
**                            End Of File
******************************************************************************/
