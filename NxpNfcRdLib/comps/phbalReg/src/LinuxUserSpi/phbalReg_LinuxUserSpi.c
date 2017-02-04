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
 * phbalReg_LinuxUserSpi.c:  BAL for linux User Mode
 *
 * Project:  NxpNfcRdLib
 *
 * $Date: 2016-10-10 22:12:05 +0530 (Mon, 10 Oct 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 6352 $ (v4.040.05.011646)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI

#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <poll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "phbalReg_LinuxUserSpi.h"
#include "phPlatform_Port_Host.h"
#include "phPlatform_Port_Pi_RC523.h"
#include "phPlatform_Port_Pi_PN5180.h"
#include "phPlatform_Port_Pi_RC663.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHBAL_REG_LINUX_ERROR                    0xFFFFFFFF

#define PHBAL_REG_LINUX_USER_SPI_CFG_DIR                "/dev/spidev"
#define PHBAL_REG_LINUX_USER_SPI_CFG_MODE               SPI_MODE_0
#define PHBAL_REG_LINUX_USER_SPI_CFG_BITS_PER_WORD      8

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
phStatus_t phbalReg_LinuxUserSpi_Init(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint16_t wSizeOfDataParams)
{
    phStatus_t status;

    if (sizeof(phbalReg_LinuxUserSpi_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId        = PH_COMP_BAL | PHBAL_REG_LINUX_USER_SPI_ID;
    pDataParams->sBal_Struct.bBalType   = PHBAL_REG_TYPE_SPI;

    pDataParams->sSpiObj.spiFd          = PHBAL_REG_LINUX_ERROR;

    /* Set default bus/pin numbers */
    pDataParams->sSpiObj.spiBus         = SPI_BUS;
    pDataParams->sSpiObj.spiCsPinNum    = SPI_CS;

    PH_CHECK_SUCCESS_FCT(status, phbalReg_LinuxUserSpi_OpenPort(pDataParams));

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_GetPortList(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint16_t wPortBufSize,
    uint8_t * pPortNames, uint16_t * pNumOfPorts)
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_SetPort(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint8_t * pPortName)
{
    uint32_t dwNumItemFilled;
    uint32_t dwSpiBus;
    uint32_t dwCSPin;
    uint32_t dwBusyPin;

    dwNumItemFilled = sscanf(pPortName, "%lu:%lu:%lu", &dwSpiBus, &dwCSPin, &dwBusyPin);
    if(dwNumItemFilled != 3)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    pDataParams->sSpiObj.spiBus = dwSpiBus;
    pDataParams->sSpiObj.spiCsPinNum = dwCSPin;

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_OpenPort(phbalReg_LinuxUserSpi_DataParams_t * pDataParams)
{
    uint32_t dwStatus;
    char spi[16];

    uint32_t mode = PHBAL_REG_LINUX_USER_SPI_CFG_MODE;
    uint8_t bits = PHBAL_REG_LINUX_USER_SPI_CFG_BITS_PER_WORD;
    uint32_t speed = SPI_FREQ;

    snprintf(spi, sizeof(spi), PHBAL_REG_LINUX_USER_SPI_CFG_DIR "%lu.%lu", pDataParams->sSpiObj.spiBus, pDataParams->sSpiObj.spiCsPinNum);

    pDataParams->sSpiObj.spiFd = open(spi, O_RDWR);
    if(pDataParams->sSpiObj.spiFd == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    /* Set mode */
    dwStatus = ioctl(pDataParams->sSpiObj.spiFd, SPI_IOC_WR_MODE, &mode);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(pDataParams);
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    /* Set Bits per Word */
    dwStatus = ioctl(pDataParams->sSpiObj.spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(pDataParams);
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    /* Set Speed. */
    dwStatus = ioctl(pDataParams->sSpiObj.spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserSpi_ClosePort(pDataParams);
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_ClosePort(phbalReg_LinuxUserSpi_DataParams_t * pDataParams)
{
    if(pDataParams->sSpiObj.spiFd != PHBAL_REG_LINUX_ERROR)
    {
        close(pDataParams->sSpiObj.spiFd);
    }

    pDataParams->sSpiObj.spiFd = PHBAL_REG_LINUX_ERROR;

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_Exchange(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint16_t wOption,
    uint8_t * pTxBuffer, uint16_t wTxLength, uint16_t wRxBufSize, uint8_t * pRxBuffer, uint16_t * pRxLength)
{
    uint32_t dwStatus;
    struct spi_ioc_transfer sSpiTransfer = {0};

    if(wTxLength == 0)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_BAL);
    }

    sSpiTransfer.tx_buf = (uintptr_t)pTxBuffer;
    sSpiTransfer.rx_buf = (uintptr_t)pRxBuffer;
    sSpiTransfer.len = wTxLength;
    sSpiTransfer.speed_hz = SPI_FREQ;
    sSpiTransfer.bits_per_word = PHBAL_REG_LINUX_USER_SPI_CFG_BITS_PER_WORD;
    sSpiTransfer.delay_usecs = 0;

    dwStatus = ioctl(pDataParams->sSpiObj.spiFd, SPI_IOC_MESSAGE(1), &sSpiTransfer);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    }

    if( pRxLength != NULL )
    {
        *pRxLength = sSpiTransfer.len;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserSpi_SetConfig(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t wValue)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

phStatus_t phbalReg_LinuxUserSpi_GetConfig(phbalReg_LinuxUserSpi_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t * pValue)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */
