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
* Software Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 5316 $ (v4.040.05.011646)
* $Date: 2016-06-29 12:19:39 +0530 (Wed, 29 Jun 2016) $
*/

#include <ph_TypeDefs.h>
#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phacDiscLoop.h>
#include <phalT1T.h>
#include <phalTop.h>

#ifdef NXPBUILD__PHAL_TOP_SW

#include "phalTop_Sw.h"
#include "phalTop_Sw_Int_T1T.h"
#include "phalTop_Sw_Int_T2T.h"
#include "phalTop_Sw_Int_T3T.h"
#include "phalTop_Sw_Int_T4T.h"
#include "phalTop_Sw_Int_T5T.h"

static const pphalTop_Sw_CheckNdef pfphalTop_Sw_CheckNdef[PHAL_TOP_MAX_TAGTYPE_SUPPORTED] = {
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_CheckNdef,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_CheckNdef,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_FELICA_SW)
    &phalTop_Sw_Int_T3T_CheckNdef,
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if ! defined(NXPBUILD__PHAL_FELICA_SW)
    NULL,
#endif /* NXPBUILD__PHAL_FELICA_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    &phalTop_Sw_Int_T4T_CheckNdef,
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if ! defined(NXPBUILD__PHAL_MFDF_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_CheckNdef
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

static const pphalTop_Sw_FormatNdef pfphalTop_Sw_FormatNdef[PHAL_TOP_MAX_TAGTYPE_SUPPORTED] = {
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_FormatNdef,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_FormatNdef,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_FELICA_SW)
    &phalTop_Sw_Int_T3T_FormatNdef,
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if ! defined(NXPBUILD__PHAL_FELICA_SW)
    NULL,
#endif /* NXPBUILD__PHAL_FELICA_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    &phalTop_Sw_Int_T4T_FormatNdef,
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if ! defined(NXPBUILD__PHAL_MFDF_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_FormatNdef
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

static const pphalTop_Sw_EraseNdef pfphalTop_Sw_EraseNdef[PHAL_TOP_MAX_TAGTYPE_SUPPORTED] = {
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_EraseNdef,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_EraseNdef,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_FELICA_SW)
    &phalTop_Sw_Int_T3T_EraseNdef,
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if ! defined(NXPBUILD__PHAL_FELICA_SW)
    NULL,
#endif /* NXPBUILD__PHAL_FELICA_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    &phalTop_Sw_Int_T4T_EraseNdef,
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if ! defined(NXPBUILD__PHAL_MFDF_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_EraseNdef
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

static const pphalTop_Sw_ReadNdef pfphalTop_Sw_ReadNdef[PHAL_TOP_MAX_TAGTYPE_SUPPORTED] = {
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_ReadNdef,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_ReadNdef,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_FELICA_SW)
    &phalTop_Sw_Int_T3T_ReadNdef,
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if ! defined(NXPBUILD__PHAL_FELICA_SW)
    NULL,
#endif /* NXPBUILD__PHAL_FELICA_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    &phalTop_Sw_Int_T4T_ReadNdef,
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if ! defined(NXPBUILD__PHAL_MFDF_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_ReadNdef
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

static const pphalTop_Sw_WriteNdef pfphalTop_Sw_WriteNdef[PHAL_TOP_MAX_TAGTYPE_SUPPORTED] = {
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_WriteNdef,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_WriteNdef,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_FELICA_SW)
    &phalTop_Sw_Int_T3T_WriteNdef,
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if ! defined(NXPBUILD__PHAL_FELICA_SW)
    NULL,
#endif /* NXPBUILD__PHAL_FELICA_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    &phalTop_Sw_Int_T4T_WriteNdef,
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if ! defined(NXPBUILD__PHAL_MFDF_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_WriteNdef
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

static const pphalTop_Sw_Int_SetReadOnly pfphalTop_Sw_Int_SetReadOnly[PHAL_TOP_MAX_TAGTYPE_SUPPORTED]={
#if defined(NXPBUILD__PHAL_T1T_SW)
    &phalTop_Sw_Int_T1T_SetReadOnly,
#endif /* NXPBUILD__PHAL_T1T_SW */
#if ! defined(NXPBUILD__PHAL_T1T_SW)
    NULL,
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    &phalTop_Sw_Int_T2T_SetReadOnly,
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if ! defined(NXPBUILD__PHAL_MFUL_SW)
    NULL,
#endif /* NXPBUILD__PHAL_MFUL_SW */
    NULL,
    NULL,
#if defined(NXPBUILD__PHAL_I15693_SW)
    &phalTop_Sw_Int_T5T_SetReadOnly
#endif /* NXPBUILD__PHAL_I15693_SW */
#if ! defined(NXPBUILD__PHAL_I15693_SW)
    NULL
#endif /* NXPBUILD__PHAL_I15693_SW */
};

phStatus_t phalTop_Sw_Init(
                           phalTop_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams,
                           void * pAlT1T,
                           void * pAlMful,
                           void * pAlFelica,
                           void * pAlMfdf,
                           void * pAl15693
                           )
{
    if (sizeof(phalTop_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_TOP);
    }

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAlT1T);
    PH_ASSERT_NULL (pAlMful);
    PH_ASSERT_NULL (pAlFelica);
    PH_ASSERT_NULL (pAlMfdf);
    PH_ASSERT_NULL (pAl15693);

    pDataParams->wId = PH_COMP_AL_TOP | PHAL_TOP_SW_ID;
    pDataParams->pTopTagsDataParams[0] = pAlT1T;
    pDataParams->pTopTagsDataParams[1] = pAlMful;
    pDataParams->pTopTagsDataParams[2] = pAlFelica;
    pDataParams->pTopTagsDataParams[3] = pAlMfdf;
    pDataParams->pTopTagsDataParams[4] = pAl15693;

    return phalTop_Sw_Reset(pDataParams);
}

phStatus_t phalTop_Sw_CheckNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pTagState
                                )
{
    /* Validate tag type */
    if((pDataParams->bTagType < PHAL_TOP_TAG_TYPE_T1T_TAG) ||
       (pDataParams->bTagType > PHAL_TOP_TAG_TYPE_T5T_TAG))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if the tag is enabled in build */
    if(pfphalTop_Sw_CheckNdef[pDataParams->bTagType - 1] != NULL)
    {
        return pfphalTop_Sw_CheckNdef[pDataParams->bTagType - 1](
            pDataParams,
            pTagState);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }
}

phStatus_t phalTop_Sw_ReadNdef(
                               phalTop_Sw_DataParams_t * pDataParams,
                               uint8_t * pData,
                               uint16_t * pLength
                               )
{
    /* Validate tag type */
    if((pDataParams->bTagType < PHAL_TOP_TAG_TYPE_T1T_TAG) ||
       (pDataParams->bTagType > PHAL_TOP_TAG_TYPE_T5T_TAG))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if the tag is enabled in build */
    if(pfphalTop_Sw_ReadNdef[pDataParams->bTagType - 1] != NULL)
    {
        return pfphalTop_Sw_ReadNdef[pDataParams->bTagType - 1](
            pDataParams,
            pData,
            pLength);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }
}

phStatus_t phalTop_Sw_WriteNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pData,
                                uint16_t wLength
                                )
{
    /* Validate tag type */
    if((pDataParams->bTagType < PHAL_TOP_TAG_TYPE_T1T_TAG) ||
       (pDataParams->bTagType > PHAL_TOP_TAG_TYPE_T5T_TAG))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if the tag is enabled in build */
    if(pfphalTop_Sw_WriteNdef[pDataParams->bTagType - 1] != NULL)
    {
        return pfphalTop_Sw_WriteNdef[pDataParams->bTagType - 1](
            pDataParams,
            pData,
            wLength);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }
}

phStatus_t phalTop_Sw_EraseNdef(
                                phalTop_Sw_DataParams_t * pDataParams
                                )
{
    /* Validate tag type */
    if((pDataParams->bTagType < PHAL_TOP_TAG_TYPE_T1T_TAG) ||
       (pDataParams->bTagType > PHAL_TOP_TAG_TYPE_T5T_TAG))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if the tag is enabled in build */
    if(pfphalTop_Sw_EraseNdef[pDataParams->bTagType - 1] != NULL)
    {
        return pfphalTop_Sw_EraseNdef[pDataParams->bTagType - 1](pDataParams);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }
}

phStatus_t phalTop_Sw_FormatNdef(
                                 phalTop_Sw_DataParams_t * pDataParams
                                 )
{
    /* Validate tag type */
    if((pDataParams->bTagType < PHAL_TOP_TAG_TYPE_T1T_TAG) ||
       (pDataParams->bTagType > PHAL_TOP_TAG_TYPE_T5T_TAG))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    /* Check if the tag is enabled in build */
    if(pfphalTop_Sw_FormatNdef[pDataParams->bTagType - 1] != NULL)
    {
        return pfphalTop_Sw_FormatNdef[pDataParams->bTagType - 1](pDataParams);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }
}

phStatus_t phalTop_Sw_Reset(
                            phalTop_Sw_DataParams_t * pDataParams
                            )
{

    /* Reset common variables */
    pDataParams->bTagState = 0;
    pDataParams->bTagType = 0;
    pDataParams->bVno = 0;
    pDataParams->wNdefLength = 0;
    pDataParams->wMaxNdefLength = 0;

    memset(&pDataParams->ualTop, 0x00, sizeof(pDataParams->ualTop));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_TOP);
}

phStatus_t phalTop_Sw_SetConfig(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint16_t wConfig,
                                uint16_t wValue
                                )
{
    switch(wConfig)
    {
    case PHAL_TOP_CONFIG_TAG_TYPE:
        if((wValue < PHAL_TOP_TAG_TYPE_T1T_TAG) || (wValue > PHAL_TOP_TAG_TYPE_T5T_TAG))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        pDataParams->bTagType = (uint8_t)wValue;
        break;

    case PHAL_TOP_CONFIG_TAG_STATE:
        /* This shall be used only to set tag to read-only state. */
        if(wValue != PHAL_TOP_STATE_READONLY)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        /* Set to read only is supported only by T1T, T2T and T5T */
        if((pDataParams->bTagType != PHAL_TOP_TAG_TYPE_T1T_TAG) &&
           (pDataParams->bTagType != PHAL_TOP_TAG_TYPE_T2T_TAG) &&
           (pDataParams->bTagType != PHAL_TOP_TAG_TYPE_T5T_TAG))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        /* Check if the tag is enabled in build */
        if(pfphalTop_Sw_Int_SetReadOnly[pDataParams->bTagType - 1] != NULL)
        {
            return pfphalTop_Sw_Int_SetReadOnly[pDataParams->bTagType - 1](
                pDataParams);
        }
        else
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }
        break;

#if defined(NXPBUILD__PHAL_T1T_SW)
    case PHAL_TOP_CONFIG_T1T_TMS:
        if((wValue > 0x800) || (wValue < 0x0E))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T1T_t *)(&pDataParams->ualTop.salTop_T1T))->bTms = (uint8_t)((wValue / 8) - 1);
        break;

    case PHAL_TOP_CONFIG_T1T_TERMINATOR_TLV:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T1T_t *)(&pDataParams->ualTop.salTop_T1T))->bTerminatorTlvPresence = (uint8_t)(wValue);
        break;
#endif /* NXPBUILD__PHAL_T1T_SW */

#if defined(NXPBUILD__PHAL_MFUL_SW)
    case PHAL_TOP_CONFIG_T2T_TMS:
        if(wValue < 0x30)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T2T_t *)(&pDataParams->ualTop.salTop_T2T))->bTms = (uint8_t)(wValue / 8);
        break;
#endif /* NXPBUILD__PHAL_MFUL_SW */

#if defined(NXPBUILD__PHAL_MFDF_SW)
    case PHAL_TOP_CONFIG_T4T_NDEF_FILE_ID:
        if((wValue == 0x0000) || (wValue == 0xE102) || (wValue == 0xE103) ||
           (wValue == 0x3F00) || (wValue == 0x3FFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T4T_t *)(&pDataParams->ualTop.salTop_T4T))->aNdefFileID[0] = (uint8_t)(wValue & 0x00FF);
        ((phalTop_T4T_t *)(&pDataParams->ualTop.salTop_T4T))->aNdefFileID[1] = (uint8_t)((wValue & 0xFF00) >> 8);
        break;

    case PHAL_TOP_CONFIG_T4T_NDEF_FILE_SIZE:
        if((wValue < 0x0005) || (wValue > 0x80FE))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T4T_t *)(&pDataParams->ualTop.salTop_T4T))->wMaxFileSize = wValue;
        break;

    case PHAL_TOP_CONFIG_T4T_MLE:
        if(wValue < 0x000F)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T4T_t *)(&pDataParams->ualTop.salTop_T4T))->wMLe = wValue;
        break;

    case PHAL_TOP_CONFIG_T4T_MLC:
        if(wValue < 0x0001)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T4T_t *)(&pDataParams->ualTop.salTop_T4T))->wMLc = wValue;
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#if defined(NXPBUILD__PHAL_I15693_SW)
    case PHAL_TOP_CONFIG_T5T_MLEN:
        if(wValue < 24)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->wMlen = (wValue / 8);
        break;

    case PHAL_TOP_CONFIG_T5T_MBREAD:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->bMbRead = (uint8_t)(wValue);
        break;

    case PHAL_TOP_CONFIG_T5T_LOCKBLOCK:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->bLockBlock = (uint8_t)(wValue);
        break;

    case PHAL_TOP_CONFIG_T5T_SPL_FRM:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->bSplFrm = (uint8_t)(wValue);
        break;

    case PHAL_TOP_CONFIG_T5T_OPTION_FLAG:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->bOptionFlag = (uint8_t)(wValue);
        break;

    case PHAL_TOP_CONFIG_T5T_TERMINATOR_TLV:
        if((wValue != PH_ON) && (wValue != PH_OFF))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
        }

        ((phalTop_T5T_t *)(&pDataParams->ualTop.salTop_T5T))->bTerminatorTlvPresence = (uint8_t)(wValue);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_TOP);
}

phStatus_t phalTop_Sw_GetConfig(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint16_t wConfig,
                                uint16_t  * wValue
                                )
{
    switch(wConfig)
    {
    case PHAL_TOP_CONFIG_TAG_TYPE:
        *wValue = pDataParams->bTagType;
        break;

    case PHAL_TOP_CONFIG_TAG_STATE:
        *wValue = pDataParams->bTagState;
        break;

    case PHAL_TOP_CONFIG_NDEF_LENGTH:
        *wValue = pDataParams->wNdefLength;
        break;

    case PHAL_TOP_CONFIG_MAX_NDEF_LENGTH:
        *wValue = pDataParams->wMaxNdefLength;
        break;

    case PHAL_TOP_CONFIG_NDEF_VERSION:
        *wValue = pDataParams->bVno;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_TOP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_TOP);
}

#if defined(__DEBUG)
/* This API would be available only for debugging purpose */
phStatus_t phalTop_Sw_SetPtr(
                             phalTop_Sw_DataParams_t * pDataParams,
                             void * pT1T,
                             void * pT2T,
                             void * pT3T,
                             void * pT4T,
                             void * pT5T
                             )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;;

#if defined(NXPBUILD__PHAL_T1T_SW)
    pDataParams->pTopTagsDataParams[0] = pT1T;
#endif /* NXPBUILD__PHAL_T1T_SW */
#if defined(NXPBUILD__PHAL_MFUL_SW)
    pDataParams->pTopTagsDataParams[1] = pT2T;
#endif /* NXPBUILD__PHAL_MFUL_SW */
#if defined(NXPBUILD__PHAL_FELICA_SW)
    pDataParams->pTopTagsDataParams[2] = pT3T;
#endif /* NXPBUILD__PHAL_FELICA_SW */
#if defined(NXPBUILD__PHAL_MFDF_SW)
    pDataParams->pTopTagsDataParams[3] = pT4T;
#endif /* NXPBUILD__PHAL_MFDF_SW */
#if defined(NXPBUILD__PHAL_I15693_SW)
    pDataParams->pTopTagsDataParams[4] = pT5T;
#endif /* NXPBUILD__PHAL_I15693_SW */
    return status;
}
#endif /* __DEBUG */

#endif /* NXPBUILD__PHAL_TOP_SW */
