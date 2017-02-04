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
* BAL Stub Component of Reader Library Framework.
* $Author: Purnank G (ing05193) $
* $Revision: 6139 $ (v4.040.05.011646)
* $Date: 2016-09-23 15:43:50 +0530 (Fri, 23 Sep 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
#include "phPlatform.h"
#include "phbalReg_LpcOpenSpi.h"

phStatus_t phbalReg_LpcOpenSpi_Init(
                                    phbalReg_LpcOpenSpi_DataParams_t * pDataParams,
                                    uint16_t wSizeOfDataParams
                                    )
{
    SSP_ConfigFormat ssp_format;
    volatile uint32_t delay;

    if (sizeof(phbalReg_LpcOpenSpi_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId      = PH_COMP_BAL | PHBAL_REG_LPCOPEN_SPI_ID;
    pDataParams->sBal_Struct.bBalType = PHBAL_REG_TYPE_SPI;

    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_SPI);

    Chip_SSP_Init(LPC_SSP);
    Chip_SSP_SetBitRate(LPC_SSP, SSP_CLOCKRATE);

    ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
    ssp_format.bits = SSP_BITS_8;
    ssp_format.clockMode = SSP_CLOCK_MODE0;

    Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
    Chip_SSP_SetMaster(LPC_SSP, 1 /*Master*/);
    Chip_SSP_Enable(LPC_SSP);

    /* Wait Startup time */
    for(delay=0; delay<10000; delay++){}

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_GetPortList(
                                           phbalReg_LpcOpenSpi_DataParams_t * pDataParams
                                           )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_SetPort(
                                       phbalReg_LpcOpenSpi_DataParams_t * pDataParams
                                       )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_OpenPort(
                                        phbalReg_LpcOpenSpi_DataParams_t * pDataParams
                                        )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_ClosePort(
                                         phbalReg_LpcOpenSpi_DataParams_t * pDataParams
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_Exchange(
                                        phbalReg_LpcOpenSpi_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    uint16_t              xferLen;
    Chip_SSP_DATA_SETUP_T xferConfig;

    xferConfig.length  = wTxLength;
    xferConfig.rx_data = pRxBuffer;
    xferConfig.tx_data = pTxBuffer;
    xferConfig.rx_cnt  = 0;
    xferConfig.tx_cnt  = 0;

    /* Enable chip select of reader IC by pulling NSS low. */
    Chip_GPIO_SetPinState(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP, PH_PLATFORM_SET_LOW);

    /* data exchange */
    if ( wRxBufSize == 0 )
    {
        Chip_SSP_WriteFrames_Blocking(LPC_SSP, pTxBuffer, wTxLength);
        xferLen = wTxLength; /* Fake as a full duplex */
    }
    else
    {
        if ( wTxLength == 0 )
        {
            xferLen = Chip_SSP_ReadFrames_Blocking(LPC_SSP, pRxBuffer, wRxBufSize);
        }
        else
        {
            xferLen = Chip_SSP_RWFrames_Blocking(LPC_SSP, &xferConfig);
        }
    }

    /* Disable chip select of reader IC by pulling NSS high. */
    Chip_GPIO_SetPinState(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP, PH_PLATFORM_SET_HIGH);

    if ( wTxLength !=0 && wRxBufSize != 0 )
    {
        if (xferLen != wTxLength)
        {
            return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
        }
    }
    if ( pRxLength != NULL)
    	*pRxLength = xferLen;

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_SetConfig(
                                         phbalReg_LpcOpenSpi_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenSpi_GetConfig(
                                         phbalReg_LpcOpenSpi_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         )
{
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */
