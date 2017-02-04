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
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 5458 $ (v4.040.05.011646)
 * $Date: 2016-09-01 19:11:09 +0530 (Thu, 01 Sept 2016) $
 *
 * History:
 *
 *
 */

/**
* Reader Library Headers
*/
#include <Nfcrdlib_SimplifiedApi_ISO.h>


/*******************************************************************************
**   Global Defines
*******************************************************************************/
#define DATA_BUFFER_LEN             256 /* Buffer length */

uint8_t bMoreDataAvailable = 0;
uint16_t wNumberofBytes = 256;
phNfcLib_Transmit_t phNfcLib_TransmitInput;
phNfcLib_PeerInfo_t PeerInfo = {0};
uint8_t             bDataBuffer[DATA_BUFFER_LEN];  /* universal data buffer */
void SimplifiedApiDemo (void* pParams);

/*******************************************************************************
**   Function Declarations
*******************************************************************************/


/*******************************************************************************
**   Function Definitions
*******************************************************************************/

/*******************************************************************************
**   Main Function
*******************************************************************************/
#ifdef NXPBUILD__PH_RASPBERRY_PI
#define SIMPLIFIED_ISO_PRIO     0
#define SIMPLIFIED_ISO_STACK    0x20000
#else
#define SIMPLIFIED_ISO_PRIO     4
#define SIMPLIFIED_ISO_STACK    2000
#endif

int main (void)
{
    phOsal_ThreadObj_t SimplifiedApi;
    SimplifiedApi.pTaskName = (uint8_t *) "SimplifiedApi";
    SimplifiedApi.pStackBuffer = NULL;
    SimplifiedApi.priority = SIMPLIFIED_ISO_PRIO;
    SimplifiedApi.stackSizeInNum = SIMPLIFIED_ISO_STACK;
    phOsal_ThreadCreate(&SimplifiedApi.ThreadHandle, &SimplifiedApi, &SimplifiedApiDemo, NULL);
    phOsal_StartScheduler();

    while(1);
    return 0;
}
void SimplifiedApiDemo (void* pParams)
{
    uint32_t dwStatus = 0;
    uint16_t wTechnologyMask = 0x1B;
    while(1)
    {
        do
        {
            /* Initialised the NfcRdLib */
            (void)phNfcLib_Init();
            if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
            {
                break;
            }

            /* Configuring the activation profile as ISO */
            phNfcLib_SetConfig_Value(PH_NFCLIB_CONFIG_ACTIVATION_PROFILE, PH_NFCLIB_ACTIVATION_PROFILE_ISO);
            if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
            {
                break;
            }

            /* Activating the card with blocking activation mode */
            dwStatus = phNfcLib_Activate(wTechnologyMask, &PeerInfo, NULL);
            if(dwStatus != PH_NFCLIB_STATUS_PEER_ACTIVATION_DONE)
            {
                break;
            }

            switch(PeerInfo.dwActivatedType)
            {
                /* Reference application for the particular tech type will divert form here */
            case E_PH_NFCLIB_MIFARE_CLASSIC_1K:
                DEBUG_PRINTF (" \nMifare Classic 1K detected... \n");
                dwStatus = NfcLib_MifareClassic_1k_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_CLASSIC_4K:
                DEBUG_PRINTF (" \nMifare Classic 4K detected... \n");
                dwStatus = NfcLib_MifareClassic_1k_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_ULTRALIGHT:
                DEBUG_PRINTF (" \nMifare UltraLight detected... \n");
                dwStatus = NfcLib_MifareUltralight_Reference_app();
                break;

            case E_PH_NFCLIB_MIFARE_DESFIRE:
                DEBUG_PRINTF (" \nMifare Desfire detected... \n");
                dwStatus = NfcLib_MifareDesfire_Reference_app();
                break;

            case E_PH_NFCLIB_TYPEB_LAYER3:
                DEBUG_PRINTF (" \nType B Layer 3 card detected... \n");
                DEBUG_PRINTF (" \nThis application contains nothing for only layer 3 Type B card... \n");
                break;

            case E_PH_NFCLIB_TYPEA_LAYER3:
                DEBUG_PRINTF (" \nType A Layer 3 card or Mifare Classic/ultralight with merged atqa detected... \n");
                DEBUG_PRINTF (" \nThis application contains nothing for only layer 3 Type A card... \n");
                break;

            case E_PH_NFCLIB_TYPEA_LAYER4:
                DEBUG_PRINTF (" \nType A Layer 4 card or Mifare desfire with merged atqa detected... \n");
                dwStatus = NfcLib_Layer4TypeA_Reference_app();
                break;

            case E_PH_NFCLIB_TYPEB_LAYER4:
                DEBUG_PRINTF (" \nType B Layer 4 card detected... \n");
                dwStatus = NfcLib_TypeB_Reference_app();
                break;

            case E_PH_NFCLIB_ISO15693:
                DEBUG_PRINTF (" \nType ISO 15693 compatible card detected... \n");
                dwStatus = NfcLib_ISO15693_Reference_app();
                break;

            case E_PH_NFCLIB_ISO18000p3m3:
                DEBUG_PRINTF (" \nType ISO 18000 compatible card detected... \n");
                dwStatus = NfcLib_ISO18000p3m3_Reference_app();
                break;

            default:
                break;
            }

            dwStatus = phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RELEASE,
                                            &PeerInfo
                                            );
            if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
            {
                DEBUG_PRINTF (" \nDeactivate with Release Mode failed, card was removed from vicinity... \n");
                DEBUG_PRINTF (" \n Thus Performing Deactivate with RF OFF mode... \n");
                dwStatus = phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF,
                                                &PeerInfo
                                                );
            }

        } while(0);

        (void)phNfcLib_DeInit();
    }

}


/**
* This function will print buffer content
* \param   *pBuff   Buffer Reference
* \param   num      data size to be print
*/
void phApp_Print_Buff(uint8_t *pBuff, uint8_t num)
{
    uint32_t    i;

    for(i = 0; i < num; i++)
    {
        DEBUG_PRINTF(" %02X",pBuff[i]);
    }
}
/***********************************************************************************************
 *                            End Of File
 **********************************************************************************************/
