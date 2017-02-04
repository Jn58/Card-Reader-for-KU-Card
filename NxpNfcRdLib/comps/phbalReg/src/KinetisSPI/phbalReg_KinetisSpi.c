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
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v4.040.05.011646)
* $Date: 2016-01-22 18:04:59 +0530 (Fri, 22 Jan 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI

#include "phPlatform.h"
#include "phbalReg_KinetisSpi.h"

#ifdef NXPBUILD__PH_KINETIS_K81
dspi_rtos_handle_t g_masterHandle;
#endif

phStatus_t phbalReg_KinetisSpi_Init(
                                    phbalReg_KinetisSpi_DataParams_t * pDataParams,
                                    uint16_t wSizeOfDataParams
                                    )
{
	dspi_master_config_t g_masterConfig;

    if (sizeof(phbalReg_KinetisSpi_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }
    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId      = PH_COMP_BAL | PHBAL_REG_KINETIS_SPI_ID;
    pDataParams->sBal_Struct.bBalType = PHBAL_REG_TYPE_SPI;

    memset(&g_masterConfig, 0, sizeof(dspi_master_config_t));

    g_masterConfig.whichCtar = kDSPI_Ctar0;
    g_masterConfig.ctarConfig.baudRate = DSPI_DATA_RATE;
    g_masterConfig.ctarConfig.bitsPerFrame = 8U;
    g_masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    g_masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    g_masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    g_masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / DSPI_DATA_RATE;
    g_masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / DSPI_DATA_RATE;
    g_masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / DSPI_DATA_RATE;
    g_masterConfig.whichPcs = kDSPI_Pcs0;
    g_masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;
    g_masterConfig.enableContinuousSCK = false;
    g_masterConfig.enableRxFifoOverWrite = false;
    g_masterConfig.enableModifiedTimingFormat = false;
    g_masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_SPI);

#ifdef NXPBUILD__PH_KINETIS_K81
    DSPI_RTOS_Init(&g_masterHandle, DSPI_MASTER_BASE, &g_masterConfig, CLOCK_GetFreq(DSPI_MASTER_CLOCK_SRC));
#else
    /* Initialize the DSPI peripheral */
    DSPI_MasterInit(KINETIS_DSPI_MASTER, &g_masterConfig, CLOCK_GetFreq(DSPI_MASTER_CLOCK_SRC));
#endif

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_GetPortList(
                                           phbalReg_KinetisSpi_DataParams_t * pDataParams
                                           )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_SetPort(
                                       phbalReg_KinetisSpi_DataParams_t * pDataParams
                                       )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_OpenPort(
                                        phbalReg_KinetisSpi_DataParams_t * pDataParams
                                        )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_ClosePort(
                                         phbalReg_KinetisSpi_DataParams_t * pDataParams
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_Exchange(
                                        phbalReg_KinetisSpi_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t            status;
    uint8_t * pRxBuf;
    status_t dspiStatus;
    dspi_transfer_t g_masterXfer;
    uint8_t g_dummyBuffer[260];

    memset(&g_masterXfer, 0, sizeof(dspi_transfer_t));

    status = PH_ERR_SUCCESS;

    if(pRxBuffer == NULL)
    {
        pRxBuf = g_dummyBuffer;
    }
    else
    {
        pRxBuf = pRxBuffer;
    }

    /* chip select reader IC */
    GPIO_WritePinOutput(GPIO_SSP_SSEL, SSEL_PINNUM_SSP, PH_PLATFORM_SET_LOW);

    /* Set up the transfer */
    g_masterXfer.txData = pTxBuffer;
    g_masterXfer.rxData = pRxBuf;
    g_masterXfer.dataSize = wTxLength;
    g_masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    /* Start transfer */
#ifdef NXPBUILD__PH_KINETIS_K81
    dspiStatus = DSPI_RTOS_Transfer(&g_masterHandle, &g_masterXfer);
#else
    dspiStatus = DSPI_MasterTransferBlocking(KINETIS_DSPI_MASTER, &g_masterXfer);
#endif
    if (dspiStatus != kStatus_Success)
    {
        return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    }

    GPIO_WritePinOutput(GPIO_SSP_SSEL, SSEL_PINNUM_SSP, PH_PLATFORM_SET_HIGH);

    if (pRxLength != NULL)
    {
    	*pRxLength = wTxLength;
    }

    return status;
}

phStatus_t phbalReg_KinetisSpi_SetConfig(
                                         phbalReg_KinetisSpi_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisSpi_GetConfig(
                                         phbalReg_KinetisSpi_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         )
{
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */
