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
 * Example Source for NfcrdlibEx4_MIFAREClassic that uses the MIFARE Classic implementation.
 * This example will load/configure Discovery loop for MIFARE Classic, uses POLL mode to detect the MIFARE Classic card.
 * Displays detected MIFARE Classic card information like UID, ATQA, SAK, Sector Authentication, Block Read and Write status.
 * Example will activate the device at index zero whenever multiple MIFARE Classic cards are detected.

 * Please refer Readme.txt file for Hardware Pin Configuration, Software Configuration and steps to build and
 * execute the project which is present in the same project directory.
 * $Author: Purnank G (ing05193) $
 * $Revision: 6114 $ (v4.040.05.011646)
 * $Date: 2016-09-22 16:56:53 +0530 (Thu, 22 Sep 2016) $
 *
 * History:
 *
 *
 */

/**
* Reader Library Headers
*/
#include <phApp_Init.h>

#include "NfcrdlibEx4_MIFAREClassic.h"

#define NUMBER_OF_KEYENTRIES        2
#define NUMBER_OF_KEYVERSIONPAIRS   2
#define NUMBER_OF_KUCENTRIES        1

#define DATA_BUFFER_LEN             16 /* Buffer length */
#define MFC_BLOCK_DATA_SIZE         16 /* Block Data size - 16 Bytes */

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/

/*HAL variables*/
uint8_t                            bHalBufferTx[128];          /* HAL  TX buffer */
uint8_t                            bHalBufferRx[128];          /* HAL  RX buffer */

/*PAL variables*/
phKeyStore_Sw_KeyEntry_t           sKeyEntries[NUMBER_OF_KEYENTRIES];                                  /* Sw KeyEntry structure */
phKeyStore_Sw_KUCEntry_t           sKUCEntries[NUMBER_OF_KUCENTRIES];                                  /* Sw Key usage counter structure */
phKeyStore_Sw_KeyVersionPair_t     sKeyVersionPairs[NUMBER_OF_KEYVERSIONPAIRS * NUMBER_OF_KEYENTRIES]; /* Sw KeyVersionPair structure */

uint8_t                            bDataBuffer[DATA_BUFFER_LEN];  /* universal data buffer */

uint8_t                            bSak;                      /* SAK  card type information */
uint16_t                           wAtqa;                     /* ATQA card type information */

/* Set the key for the MIFARE (R) Classic cards. */
uint8_t Key[6] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

/* Don't change the following line */
uint8_t Original_Key[6] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
uint32_t aMfClscTaskBuffer[MIFARECLASSIC_TASK_STACK];
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define aMfClscTaskBuffer       NULL
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

/*******************************************************************************
**   Function Declarations
*******************************************************************************/
void NfcrdlibEx4_MIFAREClassic(void *pParams);
static phStatus_t Ex4_NfcRdLibInit(void);

/*******************************************************************************
**   Function Definitions
*******************************************************************************/

/*******************************************************************************
**   Main Function
*******************************************************************************/
int main (void)
{
    do{
        phStatus_t status = PH_ERR_INTERNAL_ERROR;

        /* Initialize the Controller */
        phPlatform_Controller_Init();

        /* Print Example application name */
        DEBUG_PRINTF("\n *** MIFARE Classic Example *** \n");

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

        phOsal_ThreadObj_t MfClsc;

        MfClsc.pTaskName = (uint8_t *) "MfClsc";
        MfClsc.pStackBuffer = aMfClscTaskBuffer;
        MfClsc.priority = MIFARECLASSIC_TASK_PRIO;
        MfClsc.stackSizeInNum = MIFARECLASSIC_TASK_STACK;
        phOsal_ThreadCreate(&MfClsc.ThreadHandle, &MfClsc, &NfcrdlibEx4_MIFAREClassic, NULL);

        phOsal_StartScheduler();

        /* Print Error info */
        DEBUG_PRINTF("RTOS Error : Scheduler exited. \n");

#else
        (void)NfcrdlibEx4_MIFAREClassic(&sDiscLoop);
#endif
    } while(0);

    while(1); //Comes here if initialization failure or scheduler exit due to error

    return 0;
}

/***********************************************************************************************
 * \brief   This function demonstrates the MIFARE Classic card detection, Block authentication,
 *          Block Read and Block Write operation.
 * \param   *pParams
 * \return  This function will never return
 **********************************************************************************************/
void NfcrdlibEx4_MIFAREClassic(void *pParams)
{
    phStatus_t  status = 0;
    uint16_t    wTagsDetected = 0;
    uint8_t     bUid[PHAC_DISCLOOP_I3P3A_MAX_UID_LENGTH];
    uint8_t     bUidSize;

    /* Initialize components required by this example. */
    status = Ex4_NfcRdLibInit();

    while(1)    /* Continuous loop */
    {
        DEBUG_PRINTF("\nReady to detect\n");

        do
        {
            /* Field OFF */
            status = phhalHw_FieldOff(pHal);
            CHECK_STATUS(status);

            status = phhalHw_Wait(sDiscLoop.pHalDataParams,PHHAL_HW_TIME_MICROSECONDS, 5100);
            CHECK_STATUS(status);

            /* Configure Discovery loop for Poll Mode */
            status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
            CHECK_STATUS(status);

            /* Run Discovery loop */
            status = phacDiscLoop_Run(&sDiscLoop, PHAC_DISCLOOP_ENTRY_POINT_POLL);

        }while((status & PH_ERR_MASK) != PHAC_DISCLOOP_DEVICE_ACTIVATED); /* Exit on Card detection */

        /* Card detected */
        /* Get the tag types detected info */
        status = phacDiscLoop_GetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);

        /* Check for Status */
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Check for Type A tag detection */
            if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
            {
                /* Check for MIFARE Classic */
                if (0x08 == (sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aSak & 0x08))
                {
                    do
                    {
                        /* Print UID */
                        DEBUG_PRINTF ("\nUID: ");
                        phApp_Print_Buff(sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                                sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);

                        /* Print ATQA  and SAK */
                        DEBUG_PRINTF("\nATQA:");
                        phApp_Print_Buff(sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aAtqa, 2);
                        DEBUG_PRINTF ("\nSAK: 0x%x",sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aSak);

                        /* Print Product type */
                        DEBUG_PRINTF("\nProduct: MIFARE Classic \n");

                        bUidSize = sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].bUidSize;
                        memcpy(bUid, sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aUid, bUidSize);

                        /* Authenticate with the Key
                         * We can authenticate at any block of a sector and we will get the access to all blocks of the same sector
                         * For example authenticating at block 5, we will get the access to blocks 4, 5, 6 and 7.
                         */
                        /* Send authentication for block 6 */
                        status = phalMfc_Authenticate(&salMFC, 6, PHHAL_HW_MFC_KEYA, 1, 0, bUid, bUidSize);

                        /* Check for Status */
                        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
                        {
                            /* Print Error info */
                            DEBUG_PRINTF("\nAuthentication Failed!!!");
                            DEBUG_PRINTF("\nPlease correct the used key");
                            DEBUG_PRINTF("\nExecution aborted!!!\n");
                            break;
                        }

                        DEBUG_PRINTF("\nAuthentication Successful");

                        /* Empty the bDataBuffer */
                        memset(bDataBuffer, '\0', DATA_BUFFER_LEN);

                        DEBUG_PRINTF("\nRead data from Block 4");

                        /* Read data from block 4 */
                        status = phalMfc_Read(&salMFC, 4, bDataBuffer);

                        /* Check for Status */
                        if (status != PH_ERR_SUCCESS)
                        {
                            /* Print Error info */
                            DEBUG_PRINTF("\nRead operation failed!!!\n");
                            DEBUG_PRINTF("\nExecution aborted!!!\n\n");
                            break; /* Break from the loop*/
                        }

                        DEBUG_PRINTF("\nRead Success");
                        DEBUG_PRINTF("\nThe content of Block 4 is:\n");

                        phApp_Print_Buff(&bDataBuffer[0], MFC_BLOCK_DATA_SIZE);
                        DEBUG_PRINTF("\n\n --- End of Read Operation --- \n");

                        DEBUG_PRINTF("\nWrite data to Block 4");

                        /* Write data to block 4 */
                        status = phalMfc_Write(&salMFC, 4, bDataBuffer);

                        /* Check for Status */
                        if (status != PH_ERR_SUCCESS)
                        {
                            /* Print Error info */
                            DEBUG_PRINTF("\nWrite operation failed!!!\n");
                            DEBUG_PRINTF("\nExecution aborted!!!\n");
                            break; /* Break from the loop*/
                        }

                        DEBUG_PRINTF("\nWrite Success");
                        DEBUG_PRINTF("\n\n --- End of Write Operation --- ");

                        /* End of example */
                        DEBUG_PRINTF("\n\n --- End of Example --- \n\n");

                    }while(0);

                    DEBUG_PRINTF("\nPlease Remove the Card\n\n");

                    /* Field RESET */
                    status = phhalHw_FieldReset(pHal);
                    CHECK_STATUS(status);

                    /* Make sure that example application is not detecting the same card continuously */
                    do
                    {
                        /* Send WakeUpA */
                        status = phpalI14443p3a_WakeUpA(sDiscLoop.pPal1443p3aDataParams,
                                                        sDiscLoop.sTypeATargetInfo.aTypeA_I3P3[0].aAtqa);

                        /* Check for Status */
                        if (status != PH_ERR_SUCCESS)
                        {
                            break; /* Card Removed, break from the loop */
                        }

                        /* Send HaltA */
                        status = phpalI14443p3a_HaltA(sDiscLoop.pPal1443p3aDataParams);
                        CHECK_STATUS(status);

                        /* Delay - 5 milli seconds*/
                        status = phhalHw_Wait(sDiscLoop.pHalDataParams, PHHAL_HW_TIME_MILLISECONDS, 5);
                        CHECK_STATUS(status);

                    }while(1);
                }
            }
        }
    }
}

/***********************************************************************************************
 * \brief   Initializes the discovery loop and keystore components required by Example-4
 * \param   none
 * \return  status  Returns the function status
 **********************************************************************************************/
static phStatus_t Ex4_NfcRdLibInit(void)
{
    phStatus_t status;

    /* Device limit for Type A */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_TYPEA_DEVICE_LIMIT, 1);
    CHECK_STATUS(status);

    /* Bailout on Type A detect */
    status = phacDiscLoop_SetConfig(&sDiscLoop, PHAC_DISCLOOP_CONFIG_BAIL_OUT, PHAC_DISCLOOP_POS_BIT_MASK_A);
    CHECK_STATUS(status);

    /* Initialize the keystore component */
    status = phKeyStore_Sw_Init(
        &sPlatform.sKeyStore,
        sizeof(phKeyStore_Sw_DataParams_t),
        &sKeyEntries[0],
        NUMBER_OF_KEYENTRIES,
        &sKeyVersionPairs[0],
        NUMBER_OF_KEYVERSIONPAIRS,
        &sKUCEntries[0],
        NUMBER_OF_KUCENTRIES
        );
    CHECK_STATUS(status);

    /* load a Key to the Store */
    /* Note: If You use Key number 0x00, be aware that in SAM
      this Key is the 'Host authentication key' !!! */
    status = phKeyStore_FormatKeyEntry(&sPlatform.sKeyStore, 1, PH_KEYSTORE_KEY_TYPE_MIFARE);
    CHECK_STATUS(status);

    /* Set Key Store */
    status = phKeyStore_SetKey(&sPlatform.sKeyStore, 1, 0, PH_KEYSTORE_KEY_TYPE_MIFARE, &Key[0], 0);
    CHECK_STATUS(status);

    /* Read the version of the reader IC */
#if defined NXPBUILD__PHHAL_HW_RC523
    status = phhalHw_Rc523_ReadRegister(&sPlatform.sHal, PHHAL_HW_RC523_REG_VERSION, &bDataBuffer[0]);
    CHECK_STATUS(status);
    DEBUG_PRINTF("\nReader chip PN512: 0x%02x\n", bDataBuffer[0]);
#endif
#if defined NXPBUILD__PHHAL_HW_RC663
    status = phhalHw_Rc663_ReadRegister(&sPlatform.sHal, PHHAL_HW_RC663_REG_VERSION, &bDataBuffer[0]);
    CHECK_STATUS(status);
    DEBUG_PRINTF("\nReader chip RC663: 0x%02x\n", bDataBuffer[0]);
#endif

    /* Return Success */
    return PH_ERR_SUCCESS;
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

/***********************************************************************************************
 *                            End Of File
 **********************************************************************************************/
