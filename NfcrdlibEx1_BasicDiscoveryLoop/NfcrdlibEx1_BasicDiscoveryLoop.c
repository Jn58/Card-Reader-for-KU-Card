/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Example Source for NfcrdlibEx1_BasicDiscoveryLoop that uses the Discovery loop implementation.
* Uses both POLL and LISTEN modes of discovery loop. Displays detected tag information(like UID, SAK, Product Type)
* and prints information when it gets activated as a target by an external Initiator/reader.
* Whenever multiple technologies are detected, example will select first detected technology to resolve.
* Example will activate device at index zero whenever multiple device is detected.
*
* Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
* execute the project which is present in the same project directory.
* $Author: Purnank G (ing05193) $
* $Revision: 6017 $ (v4.040.05.011646)
* $Date: 2016-09-19 19:12:31 +0530 (Mon, 19 Sep 2016) $
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

#include "NfcrdlibEx1_BasicDiscoveryLoop.h"

/*******************************************************************************
**   Global Defines
*******************************************************************************/

/*HAL variables*/
uint8_t                            bHalBufferTx[256];          /* HAL TX buffer. Size 256 - Based on maximum FSL */
uint8_t                            bHalBufferRx[256];          /* HAL RX buffer. Size 256 - Based on maximum FSL */

/*The below variables needs to be initialized according to example requirements by a customer */

uint8_t  sens_res[2]     = {0x04, 0x00};              /* ATQ bytes - needed for anti-collision */
uint8_t  nfc_id1[3]      = {0xA1, 0xA2, 0xA3};        /* user defined bytes of the UID (one is hardcoded) - needed for anti-collision */
uint8_t  sel_res         = 0x40;
uint8_t  nfc_id3         = 0xFA;                      /* NFC3 byte - required for anti-collision */
uint8_t  poll_res[18]    = {0x01, 0xFE, 0xB2, 0xB3, 0xB4, 0xB5,
                                   0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3,
                                   0xC4, 0xC5, 0xC6, 0xC7, 0x23, 0x45 };


#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aBasicDiscTaskBuffer[BASIC_DISC_DEMO_TASK_STACK];
#else /* uint32_t aBasicDiscTaskBuffer[BASIC_DISC_DEMO_TASK_STACK]; */
#define aBasicDiscTaskBuffer    NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */


/*******************************************************************************
**   Static Defines
*******************************************************************************/

/* This is used to save restore Poll Config.
 * If in case application has update/change PollCfg to resolve Tech
 * when Multiple Tech was detected in previous poll cycle
 */
static uint16_t bSavePollTechCfg;

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
void BasicDiscoveryLoop_Demo(void  *pDataParams);

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
        phOsal_ThreadObj_t BasicDisc;

        /* Initialize the Controller */
        phPlatform_Controller_Init();

        /*Perform OSAL Init first before everything*/
        phOsal_Init();

        DEBUG_PRINTF("\n BasicDiscoveryLoop Example: \n");

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

        BasicDisc.pTaskName = (uint8_t *) "BasicDiscLoop";
        BasicDisc.pStackBuffer = aBasicDiscTaskBuffer;
        BasicDisc.priority = BASIC_DISC_DEMO_TASK_PRIO;
        BasicDisc.stackSizeInNum = BASIC_DISC_DEMO_TASK_STACK;
        phOsal_ThreadCreate(&BasicDisc.ThreadHandle, &BasicDisc, &BasicDiscoveryLoop_Demo, &sDiscLoop);
        phOsal_StartScheduler();
        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        (void)BasicDiscoveryLoop_Demo(&sDiscLoop);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

/**
* This function demonstrates the usage of discovery loop
* It detects and reports the NFC technology type
* \param   pDataParams      The discovery loop data parameters
* \note    This function will never return
*/
void BasicDiscoveryLoop_Demo(void  *pDataParams)
{
    phStatus_t    status = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;
    uint16_t      wTagsDetected = 0;
    uint16_t      wNumberOfTags = 0;
    uint16_t      wEntryPoint;
    uint16_t      wValue;
    uint8_t       bIndex;

    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);

    /* Get Poll Configuration */
    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, &bSavePollTechCfg);
    CHECK_STATUS(status);

    /* Start in poll mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
    status = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;

    while(1)
    {
#ifdef PH_EXAMPLE1_LPCD_ENABLE
        /* Configure LPCD */
        if((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
        {
            status = phApp_ConfigureLPCD();
            CHECK_STATUS(status);
        }

        /* Bool to enable LPCD feature. */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
        CHECK_STATUS(status);
#endif /* PH_EXAMPLE1_LPCD_ENABLE */

        /* Set Discovery Poll State to Detection */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(status);

        /* Set Poll Configuration */
        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, bSavePollTechCfg);
        CHECK_STATUS(status);

        /* Switch off RF field */
        status = phhalHw_FieldOff(pHal);
        CHECK_STATUS(status);

        /* Start discovery loop */
        status = phacDiscLoop_Run(pDataParams, wEntryPoint);

        if(wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
        {
            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED)
            {
                DEBUG_PRINTF (" \n Multiple technology detected: \n");

                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
                {
                    DEBUG_PRINTF (" \tType A detected... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
                {
                    DEBUG_PRINTF (" \tType B detected... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212))
                {
                    DEBUG_PRINTF (" \tType F detected with baud rate 212... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
                {
                    DEBUG_PRINTF (" \tType F detected with baud rate 424... \n");
                }
                if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
                {
                    DEBUG_PRINTF(" \tType V / ISO 15693 / T5T detected... \n");
                }

                /* Select 1st Detected Technology to Resolve*/
                for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                {
                    if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                    {
                        /* Configure for one of the detected technology */
                        status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, (1 << bIndex));
                        CHECK_STATUS(status);
                        break;
                    }
                }

                /* Print the technology resolved */
                phApp_PrintTech((1 << bIndex));

                /* Set Discovery Poll State to collision resolution */
                status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION);
                CHECK_STATUS(status);

                /* Restart discovery loop in poll mode from collision resolution phase */
                status = phacDiscLoop_Run(pDataParams, wEntryPoint);
            }

            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
            {
                /* Get Detected Technology Type */
                status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                /* Get number of tags detected */
                status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_STATUS(status);

                DEBUG_PRINTF (" \n Multiple cards resolved: %d cards \n",wNumberOfTags);
                phApp_PrintTagInfo(pDataParams, wNumberOfTags, wTagsDetected);

                if(wNumberOfTags > 1)
                {
                    /* Get 1st Detected Tag and Activate device at index 0 */
                    for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                    {
                        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                        {
                            DEBUG_PRINTF("\t Activating one card...\n");
                            status = phacDiscLoop_ActivateCard(pDataParams, bIndex, 0);
                            break;
                        }
                    }

                    if(((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND))
                    {
                        /* Get Detected Technology Type */
                        status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                        CHECK_STATUS(status);

                        phApp_PrintTagInfo(pDataParams, 0x01, wTagsDetected);
                    }
                    else
                    {
                        PRINT_INFO("\t\tCard activation failed...\n");
                    }
                }
                /* Switch to LISTEN mode after POLL mode */
            }
            else if (((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
                    ((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED))
            {
                /* Switch to LISTEN mode after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFON)
            {
                /*
                 * If external RF is detected during POLL, return back so that the application
                 * can restart the loop in LISTEN mode
                 */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND)
            {
                DEBUG_PRINTF (" \n Device having T4T and NFC-DEP support detected... \n");

                /* Get Detected Technology Type */
                status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                phApp_PrintTagInfo(pDataParams, 1, wTagsDetected);

                /* Switch to LISTEN mode after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
            {
                DEBUG_PRINTF (" \n Card detected and activated successfully... \n");
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_STATUS(status);

                /* Get Detected Technology Type */
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                phApp_PrintTagInfo(pDataParams, wNumberOfTags, wTagsDetected);

                /* Switch to LISTEN mode after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
            {
                DEBUG_PRINTF (" \n Active target detected... \n");
                /* Switch to LISTEN mode after POLL mode */
            }
            else if((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
            {
                DEBUG_PRINTF (" \n Passive target detected... \n");

                /* Get Detected Technology Type */
                status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                phApp_PrintTagInfo(pDataParams, 1, wTagsDetected);

                /* Switch to LISTEN mode after POLL mode */
            }
            else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
            {
                /* LPCD is succeed but no tag is detected. */
            }
            else
            {
                if((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    CHECK_STATUS(status);
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                }
                else
                {
                    DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                }
            }

            /* Update the Entry point to LISTEN mode. */
            wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
        }
        else
        {
            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFOFF)
            {
                /*
                 * Enters here if in the target/card mode and external RF is not available
                 * Wait for LISTEN timeout till an external RF is detected.
                 * Application may choose to go into standby at this point.
                 */
                status = phhalHw_EventConsume(pHal);
                CHECK_STATUS(status);

                status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_RFON_INTERRUPT, PH_ON);
                CHECK_STATUS(status);

                status = phhalHw_EventWait(pHal, LISTEN_PHASE_TIME_MS);
                if((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                {
                    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
                }
                else
                {
                    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
                }
            }
            else
            {
                if((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVATED_BY_PEER)
                {
                    DEBUG_PRINTF (" \n Device activated in listen mode... \n");
                }
                else if ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
                {
                    /* In case of Front end used is RC663, then listen mode is not supported.
                     * Switch from listen mode to poll mode. */
                }
                else
                {
                    if((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                    {
                        status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                        CHECK_STATUS(status);
                        DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                    }
                    else
                    {
                        DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                    }
                }

                /* On successful activated by Peer, switch to LISTEN mode */
                wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
            }
        }
    }
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


/******************************************************************************
**                            End Of File
******************************************************************************/
