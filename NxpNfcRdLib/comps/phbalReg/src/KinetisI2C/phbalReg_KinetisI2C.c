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
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 5832 $ (v4.040.05.011646)
* $Date: 2016-09-05 14:45:47 +0530 (Mon, 05 Sep 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C

#include "phPlatform.h"
#include "phbalReg_KinetisI2C.h"
#include <fsl_i2c.h>

phStatus_t phbalReg_KinetisI2C_Init(
                                    phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                    uint16_t wSizeOfDataParams
                                    )
{
    i2c_master_config_t masterConfig;

    if (sizeof(phbalReg_KinetisI2C_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }
    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId      = PH_COMP_BAL | PHBAL_REG_KINETIS_I2C_ID;
    pDataParams->sBal_Struct.bBalType = PHBAL_REG_TYPE_I2C;

    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_I2C);

    /*
     * masterConfig.baudRate_Bps = 100000U;
     * masterConfig.enableHighDrive = false;
     * masterConfig.enableStopHold = false;
     * masterConfig.glitchFilterWidth = 0U;
     * masterConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = I2C_BAUD_RATE;

    I2C_MasterInit(KINETIS_I2C_MASTER, &masterConfig, CLOCK_GetFreq(I2C_MASTER_CLK_SRC));

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_GetPortList(
                                           phbalReg_KinetisI2C_DataParams_t * pDataParams
                                           )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_SetPort(
                                       phbalReg_KinetisI2C_DataParams_t * pDataParams
                                       )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_OpenPort(
                                        phbalReg_KinetisI2C_DataParams_t * pDataParams
                                        )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_ClosePort(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_Exchange(
                                        phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    status_t dI2CStatus;
    i2c_master_transfer_t masterXfer;

    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = I2C_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = pTxBuffer;
    masterXfer.dataSize = wTxLength;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    /* Write to the Slave. */
    dI2CStatus = I2C_MasterTransferBlocking(KINETIS_I2C_MASTER, &masterXfer);
    if (dI2CStatus != kStatus_Success)
    {
        return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    }

    if ((wRxBufSize) && (pRxBuffer))
    {
    	/* Read from the Slave. */
    	masterXfer.direction = kI2C_Read;
    	masterXfer.data = pRxBuffer;
    	masterXfer.dataSize = wRxBufSize;
    	dI2CStatus = I2C_MasterTransferBlocking(KINETIS_I2C_MASTER, &masterXfer);
    	if (dI2CStatus != kStatus_Success)
    	{
    		return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    	}
    }

    if (pRxLength)
    {
    	*pRxLength = wRxBufSize;
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_SetConfig(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_KinetisI2C_GetConfig(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         )
{
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */
