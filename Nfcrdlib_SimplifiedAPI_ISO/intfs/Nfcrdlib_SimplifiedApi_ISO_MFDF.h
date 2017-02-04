/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Simplified API ISO MFDF.
* $Author: Ashish Pal (nxp79566) $
* $Revision: 4423 $ (v4.040.05.011646)
* $Date: 2016-16-09 11:50:54 +0530 (Fri, 22 Apr 2016) $
*
* History:
*
*
*/

#ifndef EX11_SIMPLIFIEDAPI_ISO_MFDF_H
#define EX11_SIMPLIFIEDAPI_ISO_MFDF_H

/* MFDF_CREATE_APPLICATION takes the option with which to create the application, application identifier, key settings for the application */
/* The optional File ID, optional DF Name and the size of the DF name */
/* bOption can be 0x00 - 0x03,
* \li 01 meaning wISOFileId is supplied \n
* \li 02 meaning pISODFName is present \n
* \li 03 meaning both wISOFileId and pISODFName are present \n
* \li 00 meaning both not present
 * pAid is pointer to 3 byte Application ID, bKeysettings1 and bKeysettings2 is the application access settings please refer Desfire data sheet */
/* pISOFileId is pointer to 2 byte File ID, pISODFName can be pointer to max 16 bytes name and bISODFNamelen is size of same */
#define MFDF_CREATE_APPLICATION(bOption, pAid, bKeysettings1, bKeysettings2, pISOFileID, pISODFname, bISODFNamelen) { \
		            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.bOptions = bOption; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.bKeySettings1 = bKeysettings1; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.bKeySettings2 = bKeysettings2; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.pApplicationIdentifier = pAid; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.pISOFileId = pISOFileID; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.pISODFName = pISODFname; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_Application.bISODFNameLen = bISODFNamelen; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Create_Application; \
                    }


/* MFDF_SELECT_APPLICATION takes the ID of the application to be selected */
/* pAid is pointer to a 3 byte application Id */
#define MFDF_SELECT_APPLICATION(pAid) { \
            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_Application.pApplicationIdentifier = pAid; \
            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Select_Application; \
            }

/* MFDF_GET_VERSION takes no parameter return the version of the Desfire card */
#define MFDF_GET_VERSION() { \
		                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Get_Version; \
                            }

/* MFDF_CREATE_STDDATAFILE takes the option to tell whether ISO file ID is present or not,File no and ISO file id for the file to be created */
/* Communication settings and access rights with which the data to be exchanged with the file and finally the size of the file */
/* bOpt can be 0 or 1 indicating presence of ISO file, bFileNumber can be any valid single byte no, pISOFileID is a pointer to 2 byte File ID */
/* bCommSettings is restricted to PHAL_MFDF_COMMUNICATION_PLAIN for current release, pAccessRight is a pointer to 2 byte access rights*/
/* pISOFileSize is pointer to 3 bytes of File size (LSB first)*/
#define MFDF_CREATE_STDDATAFILE(bOpt, bFileNumber, pISOFileID, bCommSettings, pAccessRight, pISOFileSize) {\
		                phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bOption = bOpt; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bFileNo = bFileNumber; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pISOFileId = pISOFileID; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.bCommSett = bCommSettings; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pAccessRights = pAccessRight; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Create_StdDataFile.pFileSize = pISOFileSize; \
                        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Create_StdDataFile; \
		                           }

/* MFDF_WRITE_DATA takes the communication settings, file no, offset from where to write as well as the data and length of the data */
/* bCommOpt can be PHAL_MFDF_COMMUNICATION_PLAIN for this current release, bFileNumber is the no of file already created to which to write */
/* pOffSet is pointer to 3 bytes offset from where to write, pData is the pointer to the data to write */
/* pDataLen is pointer ot 3 bytes of length of data (LSB first)  */
#define MFDF_WRITE_DATA(bCommOpt, bFileNumber, pOffSet, pData, pDataLen) { \
		phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Write_Data.bCommOption = bCommOpt; \
        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Write_Data.bFileNo = bFileNumber; \
        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Write_Data.pOffset = pOffSet; \
        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Write_Data.pTxData = pData; \
        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Write_Data.pTxDataLen = pDataLen; \
        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Write_Data; \
        }

/* MFDF_ISO_SELECTFILE takes the option, selection method, file id, file df name and length of the name of the file to be selected */
/* bOpt can be 0/C, bSELector can be 0/2/4, pFID is a pointer to 2 byte file id, pDFName is pointer to df file name and bDFNameLen is size of it */
#define MFDF_ISO_SELECTFILE(bOpt, bSELector, pFID, pDFName, bDFNameLen) { \
		                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_File.bOption = bOpt; \
                            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_File.bSelector = bSELector; \
                            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_File.pFid = pFID; \
                            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_File.pDFname = pDFName; \
                            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Select_File.bDFnameLen = bDFNameLen; \
                            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Iso_SelectFile; \
		                    }

/* MFDF_ISO_READBINARY takes the option, Offset from where to read, short file ID and no of bytes to read */
/* wOpt can be #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_RXCHAINING, bOffSet can be any reasonable value, bSFID should have bit 7 as 1*/
/* bBytestoRead can be 0(read entire file) or any valid value */
#define MFDF_ISO_READBINARY(wOpt, bOffSet, bSFID, bBytestoRead) { \
		            phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Read_Binary.wOption = wOpt; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Read_Binary.bOffset = bOffSet; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Read_Binary.bSfid = bSFID; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Read_Binary.bBytesToRead = bBytestoRead; \
                    phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Iso_ReadBinary; \
		                    }

/* MFDF_ISO_UPDATEBINARY takes the Offset from where to write, short file ID and data and data len to write */
/* bOffSet can be any reasonable value, bSFID should have bit 7 as 1, pTxData is pointer to data and bTxDataLen is no of bytes to write */
#define MFDF_ISO_UPDATEBINARY(bOffSet, bSFID, pTxData, bTxDataLen)  {\
		        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Update_Binary.bOffset = bOffSet; \
		        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Update_Binary.bSfid = bSFID; \
		        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Update_Binary.pData = pTxData; \
		        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.Operation.Update_Binary.bDataLen = bTxDataLen; \
		        phNfcLib_TransmitInput.phNfcLib_MifareDesfire.bCommand = MFDF_Iso_UpdateBinary; \
                   }


#endif /* EX11_SIMPLIFIEDAPI_ISO_MFDF_H */
