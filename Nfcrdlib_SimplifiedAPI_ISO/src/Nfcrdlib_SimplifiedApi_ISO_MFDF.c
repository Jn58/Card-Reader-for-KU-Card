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
 * Reference application file for MFDF interface of Simplified API
 * $Author: Ashish Pal (nxp79566) $
 * $Revision: 5458 $ (v4.040.05.011646)
 * $Date: 2016-09-01 19:11:09 +0530 (Thu, 01 Sept 2016) $
 *
 * History:
 *
 *
 */

#include <Nfcrdlib_SimplifiedApi_ISO.h>

extern phNfcLib_Transmit_t phNfcLib_TransmitInput;
extern phNfcLib_PeerInfo_t PeerInfo;

extern uint8_t bMoreDataAvailable;
extern uint16_t wNumberofBytes;
extern uint8_t  bDataBuffer[256];

#define NFCLIB_MFDF_COMMUNICATION_PLAIN   0x00U   /**< Plain mode of communication. */
#define NFCLIB_MFDF_COMMUNICATION_MACD    0x10U   /**< MAC mode of communication. */
#define NFCLIB_MFDF_COMMUNICATION_ENC     0x30U   /**< Enciphered mode of communication. */

uint32_t NfcLib_MifareDesfire_Reference_app()
{

    uint32_t dwStatus;
    uint8_t bData[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    uint8_t aData[6] = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
    uint8_t pAid[3] = {0xA5, 0x15, 0xA5};
    uint8_t pISOId[2] = {0x34, 0x12};
    uint8_t pISOFileId[2] = {0xEF, 0x01};
    uint8_t pAccessRights[2] = {0xEE, 0xEE};
    uint8_t pFileSize[3] = {0x10, 0x00, 0x00};
    uint8_t pOffset[3] = {0x00, 0x00, 0x00};
    uint8_t pDataLen[3] = {0x06, 0x00, 0x00};

    do
    {
    /*************************************************************************************************************************
     *********************************************CREATE APPLICATION**********************************************************
     *************************************************************************************************************************/
    /* Create Application with the option 0x01 which means only iso file id will be provided and no Df name */
    /* AID is A515A5 with 1234 as ISO name and will support 4 keys with AES and will have confgurable master key and free access */
    MFDF_CREATE_APPLICATION(0x01, &pAid[0], 0x0F, 0xA4, &pISOId[0], NULL, 0)

    /* Finally the command has to be sent, the command for MFDF creates the application of the AID name */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Creation of application failed please use the factory default card... \n");
        break;
    }

    /*************************************************************************************************************************
     *********************************************SELECT APPLICATION**********************************************************
     *************************************************************************************************************************/
    /* Selecting the above created app */
    MFDF_SELECT_APPLICATION(&pAid[0]);

    /* Finally the command has to be sent to select the above created app */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Selection of application failed... \n");
        break;
    }

    /*************************************************************************************************************************
     *********************************************GET VERSION****************************************************************
     *************************************************************************************************************************/
    /* Get version returns the information about the card please refer Desfire Data sheet to map the field */
    MFDF_GET_VERSION()

    /* Finally the command has to be sent, the command for MFDF gets the version */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Get Version for Desfire Failed.. \n");
        break;
    }


    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The data received over the above command can be retrieved by calling the receive */
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    /* wNumberofBytes will vary depending upon the card layout*/
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        break;
    }
    DEBUG_PRINTF("\nVersion of the Mifare Desfire Card is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);

    /*************************************************************************************************************************
     *******************************************CREATE STD DATA FILE**********************************************************
     *************************************************************************************************************************/

    /* Creating Standard Data file with option as 0x01 which means Iso File ID is present, the file to be taged as #1 */
    /* ISO file id is 01EF and only Plain communication with free access rights to be performed on this file of size 0x100000 */
    MFDF_CREATE_STDDATAFILE(0x01, 0x01, &pISOFileId[0], NFCLIB_MFDF_COMMUNICATION_PLAIN, &pAccessRights[0], &pFileSize[0])

    /* Finally the command has to be sent, the command for MFDF TO Create Std Data File */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    /* The status should be success, if not break from the application */
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Creation of Data File Failed.. \n");
        break;
    }


    /*************************************************************************************************************************
     ***************************************************WRITE DATA************************************************************
     *************************************************************************************************************************/
    /* Peform Write in Plain Mode on File #1 with no offset and 6 bytes data */
    MFDF_WRITE_DATA(NFCLIB_MFDF_COMMUNICATION_PLAIN, 0x01, &pOffset[0], &bData[0], &pDataLen[0])

    /* Finally the command has to be sent, the command for MFDF writes the data to the MFDF */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Writing Data to the Created File Failed.. \n");
        break;
    }


    /*************************************************************************************************************************
     *********************************************ISO SELECT FILE************************************************************
     *************************************************************************************************************************/

    /**
     * Due to limitation of Desfire EV1 in one session the card will not act in both native and iso mode
     * thus there is a need to perform RF-reset here. Desfire EV2 has overcome this limitation
     */

    DEBUG_PRINTF (" \n Performing Reset due to Desfire EV1 Limitation.. \n");
    /* Deactivate is being perfomed with rf mode off */
   (void)phNfcLib_Deactivate(PH_NFCLIB_DEACTIVATION_MODE_RF_OFF, &PeerInfo);

   /* Perform the activate again */
    (void)phNfcLib_Activate(0x01, &PeerInfo, NULL);

    /* Performing the select file with option as 0x0C which means no FCI is returned, bSELector is kept as 0 because selection
     * is performed by file id i.e. 0x3412 and DF Name will not be provided */
    MFDF_ISO_SELECTFILE(0x0C, 0x00, &pISOId[0], NULL, 0x00)

     /* Finally the command has to be sent, the command for MFDF selects the ISO file */
     dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                 0x0  /* No data to be send apart from command*/
                                 );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Selection of ISO File Failed.. \n");
        break;
    }

    /* Performing the select file with option as 0x0C which means no FCI is returned, bSELector is kept as 0 because selection
     * is performed by file id i.e. 0xEF01 and DF Name will not be provided */
    MFDF_ISO_SELECTFILE(0x0C, 0x00, &pISOFileId[0], NULL, 0x00)

    /* Finally the command has to be sent, the command for MFDF selects the ISO file */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* This length paramter is used only when apart from the command, there is some data to be send*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Selection of ISO File Failed.. \n");
        break;
    }

    /*************************************************************************************************************************
     *********************************************ISO UPDATE BINARY**********************************************************
     *************************************************************************************************************************/
    /* Perform Update Binary on the created ISO file with no offset, not proving any short file Id, and 6 bytes data */
    MFDF_ISO_UPDATEBINARY(0x00, 0x00, &aData[0], 0x06)

    /* Finally the command has to be sent, the command for MFDF writes the data to the iso file */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command, data is part of command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Update oF ISO Binary Failed.. \n");
        break;
    }
    /*************************************************************************************************************************
     *********************************************ISO READ BINARY***************************************************************
     *************************************************************************************************************************/

    /* Performing Read Binary of the created binary with default option, no offset and not providing Short fiLE ID and read 6 bytes */
    MFDF_ISO_READBINARY(0x00, 0x00, 0x00, 6)

    /* Finally the command has to be sent, the command for MFDF reads the data from the file */
    dwStatus = phNfcLib_Transmit(&phNfcLib_TransmitInput,
                                0x0  /* No data to be send apart from command*/
                                );
    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        DEBUG_PRINTF (" \n Read oF ISO Binary Failed.. \n");
        break;
    }

    /* This parameter has to be reset before every receive */
    wNumberofBytes = 256;

    /*The data received over the above command can be retrieved by calling the receive, should be the same data as update binary*/
    /* wNumberofBytes first as input tells the max supported recieve size and then as out tells the actual number of data bytes received */
    dwStatus = phNfcLib_Receive(&bDataBuffer[0],
                                &wNumberofBytes,
                                &bMoreDataAvailable
                                );
    DEBUG_PRINTF("\nData Read from the ISO File is");
    phApp_Print_Buff(&bDataBuffer[0], wNumberofBytes);
    /*************************************************************************************************************************/
    }while(0);

    return dwStatus;
}

