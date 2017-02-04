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
 *
 * $Date: 2016-08-25 14:01:00 +0530 (Thu, 25 Aug 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 5753 $ (v4.040.05.011646)
 */



/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */
#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C

#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <poll.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "phbalReg_LinuxUserI2C.h"
#include "phPlatform_Port_Host.h"
#include "phPlatform_Port_Pi_RC523.h"
#include "phPlatform_Port_Pi_PN5180.h"
#include "phPlatform_Port_Pi_RC663.h"
/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */
#define PHBAL_REG_LINUX_ERROR                    0xFFFFFFFF

#define PHBAL_REG_LINUX_USER_I2C_CFG_DIR                "/dev/i2c-"
#define PHBAL_REG_LINUX_USER_I2C_CFG_MODE               I2C_MODE_0
#define PHBAL_REG_LINUX_USER_I2C_CFG_BITS_PER_WORD      8

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
phStatus_t phbalReg_LinuxUserI2C_Init(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint16_t wSizeOfDataParams)
{
    phStatus_t status;

    if (sizeof(phbalReg_LinuxUserI2C_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId        = PH_COMP_BAL | PHBAL_REG_LINUX_USER_I2C_ID;
    pDataParams->sBal_Struct.bBalType   = PHBAL_REG_TYPE_I2C;

    pDataParams->sI2CObj.I2CFd          = PHBAL_REG_LINUX_ERROR;
    pDataParams->sI2CObj.I2CNum         = I2C_NUM;

    PH_CHECK_SUCCESS_FCT(status, phbalReg_LinuxUserI2C_OpenPort(pDataParams));

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserI2C_GetPortList(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint16_t wPortBufSize,
    uint8_t * pPortNames, uint16_t * pNumOfPorts)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

phStatus_t phbalReg_LinuxUserI2C_SetPort(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint8_t * pPortName)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

phStatus_t phbalReg_LinuxUserI2C_OpenPort(phbalReg_LinuxUserI2C_DataParams_t * pDataParams)
{
    uint32_t dwStatus;
    char I2C[16];

    snprintf(I2C, sizeof(I2C), PHBAL_REG_LINUX_USER_I2C_CFG_DIR "%lu", pDataParams->sI2CObj.I2CNum);

    pDataParams->sI2CObj.I2CFd = open(I2C, O_RDWR);
    if(pDataParams->sI2CObj.I2CFd == PHBAL_REG_LINUX_ERROR)
    {
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    /* Set Address. */
    dwStatus = ioctl(pDataParams->sI2CObj.I2CFd, I2C_SLAVE, I2C_SLAVE_ADDR_7BIT);
    if(dwStatus == PHBAL_REG_LINUX_ERROR)
    {
        (void)phbalReg_LinuxUserI2C_ClosePort(pDataParams);
        return (PH_ERR_INTERNAL_ERROR | PH_COMP_BAL);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserI2C_ClosePort(phbalReg_LinuxUserI2C_DataParams_t * pDataParams)
{
    if(pDataParams->sI2CObj.I2CFd != PHBAL_REG_LINUX_ERROR)
    {
        close(pDataParams->sI2CObj.I2CFd);
    }

    pDataParams->sI2CObj.I2CFd = PHBAL_REG_LINUX_ERROR;

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserI2C_Exchange(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint16_t wOption,
    uint8_t * pTxBuffer, uint16_t wTxLength, uint16_t wRxBufSize, uint8_t * pRxBuffer, uint16_t * pRxLength)
{
    uint32_t dwStatus;

    if(wTxLength == 0)
    {
        return (PH_ERR_INVALID_PARAMETER | PH_COMP_BAL);
    }

    if (pRxLength != NULL)
    {
        *pRxLength = 0;
    }

    if ((write(pDataParams->sI2CObj.I2CFd, pTxBuffer, wTxLength)) != wTxLength)
    {
        return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    }

    if ((pRxBuffer != NULL) && (pRxLength != NULL) && (wRxBufSize != 0))
    {
        if ((read(pDataParams->sI2CObj.I2CFd, pRxBuffer, wRxBufSize)) != wRxBufSize)
        {
            return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
        }

        *pRxLength = wRxBufSize;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LinuxUserI2C_SetConfig(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t wValue)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

phStatus_t phbalReg_LinuxUserI2C_GetConfig(phbalReg_LinuxUserI2C_DataParams_t * pDataParams, uint16_t wConfig,
    uint16_t * pValue)
{
    return (PH_ERR_UNSUPPORTED_PARAMETER | PH_COMP_BAL);
}

/* *****************************************************************************************************************
 * Private Functions
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */
