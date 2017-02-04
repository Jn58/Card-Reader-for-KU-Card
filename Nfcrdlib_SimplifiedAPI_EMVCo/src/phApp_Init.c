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
* Example Source abstracting component data structure and code initialization and code specific to HW used in the examples
* This file shall be present in all examples. A customer does not need to touch/modify this file. This file
* purely depends on the phNxpBuild_Lpc.h or phNxpBuild_App.h
* The phAppInit.h externs the component data structures initialized here that is in turn included by the core examples.
* The core example shall not use any other variable defined here except the RdLib component data structures(as explained above)
* The RdLib component initialization requires some user defined data and function pointers.
* These are defined in the respective examples and externed here.
*
* Keystore and Crypto initialization needs to be handled by application.
*
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v4.040.05.011646)
* $Date: 2016-01-22 18:04:59 +0530 (Fri, 22 Jan 2016) $
*
* History:
* BK: Generated 25. Sept 2014
*
*/

#include "phApp_Init.h"

#ifdef NXPBUILD__PHHAL_HW_PN5180
/* HAL specific headers */
#include <phhalHw_Pn5180_Instr.h>
#endif

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/

/* Print technology being resolved */
void phApp_PrintTech(uint8_t TechType)
{
    switch(TechType)
    {
    case PHAC_DISCLOOP_POS_BIT_MASK_A:
        DEBUG_PRINTF ("\tResolving Type A... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_B:
        DEBUG_PRINTF ("\tResolving Type B... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F212:
        DEBUG_PRINTF ("\tResolving Type F with baud rate 212... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F424:
        DEBUG_PRINTF ("\tResolving Type F with baud rate 424... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_V:
        DEBUG_PRINTF ("\tResolving Type V... \n");
        break;

    default:
        break;
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

/**
* This function will print Tag information
* \param   pDataParams      The discovery loop data parameters
* \param   wNumberOfTags    Total number of tags detected
* \param   wTagsDetected    Technology Detected
*/
void phApp_PrintTagInfo(phacDiscLoop_Sw_DataParams_t *pDataParams, uint16_t wNumberOfTags, uint16_t wTagsDetected)
{
    uint8_t bIndex;
    uint8_t bTagType;

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        if(pDataParams->sTypeATargetInfo.bT1TFlag)
        {
            DEBUG_PRINTF("\tTechnology  : Type A");
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
            DEBUG_PRINTF ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            DEBUG_PRINTF ("\n\t\tType: Type 1 Tag\n");
        }
        else
        {
            DEBUG_PRINTF("\tTechnology  : Type A");
            for(bIndex = 0; bIndex < wNumberOfTags; bIndex++)
            {
                DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
                DEBUG_PRINTF ("\n\t\tUID :");
                phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aUid,
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].bUidSize);
                DEBUG_PRINTF ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak);

                if ((pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & (uint8_t) ~0xFB) == 0)
                {
                    /* Bit b3 is set to zero, [Digital] 4.8.2 */
                    /* Mask out all other bits except for b7 and b6 */
                    bTagType = (pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & 0x60);
                    bTagType = bTagType >> 5;

                    switch(bTagType)
                    {
                    case PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type 2 Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type 4A Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: P2P\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type NFC_DEP and  4A Tag\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
    {
        DEBUG_PRINTF("\tTechnology  : Type B");
        /* Loop through all the Type B tags detected and print the Pupi */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            /* PUPI Length is always 4 bytes */
            phApp_Print_Buff( pDataParams->sTypeBTargetInfo.aTypeB_I3P3[bIndex].aPupi, 0x04);
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    if( PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212) ||
        PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
    {
        DEBUG_PRINTF("\tTechnology  : Type F");

        /* Loop through all the type F tags and print the IDm */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm,
                        PHAC_DISCLOOP_FELICA_IDM_LENGTH );
            if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[0] == 0x01) &&
                (pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[1] == 0xFE))
            {
                /* This is Type F tag with P2P capabilities */
                DEBUG_PRINTF ("\n\t\tType: P2P");
            }
            else
            {
                /* This is Type F T3T tag */
                DEBUG_PRINTF ("\n\t\tType: Type 3 Tag");
            }

            if(pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].bBaud != PHAC_DISCLOOP_CON_BITR_212)
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 424\n");
            }
            else
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 212\n");
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
    {
        DEBUG_PRINTF("\tTechnology  : Type V / ISO 15693 / T5T");
        /* Loop through all the Type V tags detected and print the UIDs */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeVTargetInfo.aTypeV[bIndex].aUid, 0x08);
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
    {
        DEBUG_PRINTF("\tTechnology  : ISO 18000p3m3 / EPC Gen2");
        /* Loop through all the 18000p3m3 tags detected and print the UII */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF("\n\t\tUII :");
            phApp_Print_Buff(
                pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].aUii,
                (pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].wUiiLength / 8));
        }
        DEBUG_PRINTF("\n");
    }
#endif
}

/**
* This function will print Error information received from Reader Lib
* \param   wStatus      Error status
*/
void PrintErrorInfo(phStatus_t wStatus)
{
    DEBUG_PRINTF("\n ErrorInfo Comp:");

    switch(wStatus & 0xFF00)
    {
    case PH_COMP_BAL:
        DEBUG_PRINTF("\t PH_COMP_BAL");
        break;
    case PH_COMP_HAL:
        DEBUG_PRINTF("\t PH_COMP_HAL");
        break;
    case PH_COMP_PAL_ISO14443P3A:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P3A");
        break;
    case PH_COMP_PAL_ISO14443P3B:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P3B");
        break;
    case PH_COMP_PAL_ISO14443P4A:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P4A");
        break;
    case PH_COMP_PAL_ISO14443P4:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P4");
        break;
    case PH_COMP_PAL_FELICA:
        DEBUG_PRINTF("\t PH_COMP_PAL_FELICA");
        break;
    case PH_COMP_PAL_EPCUID:
        DEBUG_PRINTF("\t PH_COMP_PAL_EPCUID");
        break;
    case PH_COMP_PAL_SLI15693:
        DEBUG_PRINTF("\t PH_COMP_PAL_SLI15693");
        break;
    case PH_COMP_PAL_I18000P3M3:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18000P3M3");
        break;
    case PH_COMP_PAL_I18092MPI:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18092MPI");
        break;
    case PH_COMP_PAL_I18092MT:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18092MT");
        break;
    case PH_COMP_PAL_GENERALTARGET:
        DEBUG_PRINTF("\t PH_COMP_PAL_GENERALTARGET");
        break;
    case PH_COMP_PAL_I14443P4MC:
        DEBUG_PRINTF("\t PH_COMP_PAL_I14443P4MC");
        break;
    case PH_COMP_AC_DISCLOOP:
        DEBUG_PRINTF("\t PH_COMP_AC_DISCLOOP");
        break;
    case PH_COMP_OSAL:
        DEBUG_PRINTF("\t PH_COMP_OSAL");
        break;
    default:
        DEBUG_PRINTF("\t 0x%x",(wStatus & PH_COMPID_MASK));
        break;
    }

    DEBUG_PRINTF("\t type:");

    switch(wStatus & PH_ERR_MASK)
    {
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        DEBUG_PRINTF("\t PH_ERR_SUCCESS_INCOMPLETE_BYTE");
        break;
    case PH_ERR_IO_TIMEOUT:
        DEBUG_PRINTF("\t PH_ERR_IO_TIMEOUT");
        break;
    case PH_ERR_INTEGRITY_ERROR:
        DEBUG_PRINTF("\t PH_ERR_INTEGRITY_ERROR");
        break;
    case PH_ERR_COLLISION_ERROR:
        DEBUG_PRINTF("\t PH_ERR_COLLISION_ERROR");
        break;
    case PH_ERR_BUFFER_OVERFLOW:
        DEBUG_PRINTF("\t PH_ERR_BUFFER_OVERFLOW");
        break;
    case PH_ERR_FRAMING_ERROR:
        DEBUG_PRINTF("\t PH_ERR_FRAMING_ERROR");
        break;
    case PH_ERR_PROTOCOL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_PROTOCOL_ERROR");
        break;
    case PH_ERR_RF_ERROR:
        DEBUG_PRINTF("\t PH_ERR_RF_ERROR");
        break;
    case PH_ERR_EXT_RF_ERROR:
        DEBUG_PRINTF("\t PH_ERR_EXT_RF_ERROR");
        break;
    case PH_ERR_NOISE_ERROR:
        DEBUG_PRINTF("\t PH_ERR_NOISE_ERROR");
        break;
    case PH_ERR_ABORTED:
        DEBUG_PRINTF("\t PH_ERR_ABORTED");
        break;
    case PH_ERR_INTERNAL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_INTERNAL_ERROR");
        break;
    case PH_ERR_INVALID_DATA_PARAMS:
        DEBUG_PRINTF("\t PH_ERR_INVALID_DATA_PARAMS");
        break;
    case PH_ERR_INVALID_PARAMETER:
        DEBUG_PRINTF("\t PH_ERR_INVALID_PARAMETER");
        break;
    case PH_ERR_PARAMETER_OVERFLOW:
        DEBUG_PRINTF("\t PH_ERR_PARAMETER_OVERFLOW");
        break;
    case PH_ERR_UNSUPPORTED_PARAMETER:
        DEBUG_PRINTF("\t PH_ERR_UNSUPPORTED_PARAMETER");
        break;
    case PH_ERR_OSAL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_OSAL_ERROR");
        break;
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED:
        DEBUG_PRINTF("\t PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED");
        break;
    case PHAC_DISCLOOP_COLLISION_PENDING:
        DEBUG_PRINTF("\t PHAC_DISCLOOP_COLLISION_PENDING");
        break;
    default:
        DEBUG_PRINTF("\t 0x%x",(wStatus & PH_ERR_MASK));
        break;
    }
}


/******************************************************************************
**                            End Of File
******************************************************************************/
