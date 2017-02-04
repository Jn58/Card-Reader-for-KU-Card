/*
 *                    Copyright (c), NXP Semiconductors
 *
 *                       (C) NXP Semiconductors 2016
 *
 *         All rights are reserved. Reproduction in whole or in part is
 *        prohibited without the written consent of the copyright owner.
 *    NXP reserves the right to make changes without notice at any time.
 *   NXP makes no warranty, expressed, implied or statutory, including but
 *   not limited to any implied warranty of merchantability or fitness for any
 *  particular purpose, or that the use will not infringe any third party patent,
 *   copyright or trademark. NXP must not be liable for any loss or damage
 *                            arising from its use.
 */

/** @file
 *
 * phbalReg_LinuxKernelSpi.c:  <The purpose and scope of this file>
 *
 * $Date: 2016-09-16 18:00:07 +0530 (Fri, 16 Sep 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 5955 $ (v4.040.05.011646)
 */

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI

#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "phbalReg_LinuxKernelSpi.h"
#include "phPlatform_Port_Host.h"
#include "phPlatform_Port_Pi_RC523.h"
#include "phPlatform_Port_Pi_PN5180.h"
#include "phPlatform_Port_Pi_RC663.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHBAL_REG_LINUX_KERNEL_SPI_CFG_DIR          "/dev/bal"

#define PHBAL_REG_LINUX_ERROR                       0xFFFFFFFF

/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phbalReg_LinuxKernelSpi_Init(
                                    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
                                    uint16_t wSizeOfDataParams
                                    )
{
    phStatus_t status;

    if (sizeof(phbalReg_LinuxKernelSpi_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId        = PH_COMP_BAL | PHBAL_REG_LINUX_KERNEL_SPI_ID;
    pDataParams->sBal_Struct.bBalType   = PHBAL_REG_TYPE_SPI;

    PH_CHECK_SUCCESS_FCT(status, phbalReg_LinuxKernelSpi_OpenPort(pDataParams));

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_GetPortList(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, uint16_t wPortBufSize,
    uint8_t * pPortNames, uint16_t * pNumOfPorts)
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_SetPort(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, uint8_t * pPortName)
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_OpenPort(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams)
{

    pDataParams->sSpiObj.spiFd = open(PHBAL_REG_LINUX_KERNEL_SPI_CFG_DIR, O_RDWR);
    if(pDataParams->sSpiObj.spiFd == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_ClosePort(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams)
{
    if(pDataParams->sSpiObj.spiFd != PHBAL_REG_LINUX_ERROR)
    {
        close(pDataParams->sSpiObj.spiFd);
    }

    pDataParams->sSpiObj.spiFd = PHBAL_REG_LINUX_ERROR;

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_Exchange(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, uint16_t wOption,
    uint8_t * pTxBuffer, uint16_t wTxLength, uint16_t wRxBufSize, uint8_t * pRxBuffer, uint16_t * pRxLength)
{
    uint16_t sNumBytesWritten;

    if ((pTxBuffer == NULL) && (pRxBuffer == NULL))
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_BAL);
    }

    sNumBytesWritten = write(pDataParams->sSpiObj.spiFd, pTxBuffer, wTxLength);

    if (sNumBytesWritten != wTxLength)
    {
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    if ((pRxBuffer != NULL) && (wRxBufSize))
    {
        sNumBytesWritten = read(pDataParams->sSpiObj.spiFd, pRxBuffer, wRxBufSize);
    }

    if (pRxLength != NULL)
    {
        *pRxLength = sNumBytesWritten;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxKernelSpi_SetConfig(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t wValue)
{
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_BAL);
}

phStatus_t phbalReg_LinuxKernelSpi_GetConfig(phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t * pValue)
{
    return (PH_ERR_UNSUPPORTED_COMMAND | PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

