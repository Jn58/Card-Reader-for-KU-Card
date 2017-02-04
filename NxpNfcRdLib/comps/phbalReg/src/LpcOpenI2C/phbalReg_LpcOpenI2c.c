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
* $Revision: 5316 $ (v4.040.05.011646)
* $Date: 2016-06-29 12:19:39 +0530 (Wed, 29 Jun 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
#include "phPlatform.h"
#include "phbalReg_LpcOpenI2c.h"

phStatus_t phbalReg_LpcOpenI2c_Init(
                                    phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                    uint16_t wSizeOfDataParams
                                    )
{
    volatile uint32_t delay;

    /* EMVCo: High Speed SPI require for EMVCo */
    uint32_t ClockRate = 1000000;

    if (sizeof(phbalReg_LpcOpenI2c_DataParams_t) != wSizeOfDataParams)
    {
        return (PH_ERR_INVALID_DATA_PARAMS | PH_COMP_BAL);
    }

    PH_ASSERT_NULL (pDataParams);

    pDataParams->sBal_Struct.wId      = PH_COMP_BAL | PHBAL_REG_LPCOPEN_I2C_ID;
    pDataParams->sBal_Struct.bBalType = PHBAL_REG_TYPE_I2C;

    phPlatform_Port_Host_SetPinConfig(PHPLATFORM_PORT_PIN_I2C);

    Chip_I2C_Init(LPC_I2C);

    Chip_I2C_SetClockRate(LPC_I2C, ClockRate);

    NVIC_DisableIRQ(LPC_DIS_I2C_IRQ);
    Chip_I2C_SetMasterEventHandler(LPC_I2C, Chip_I2C_EventHandlerPolling);

    for(delay=0;delay<10000;delay++){}

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_GetPortList(
                                           phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                           )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_SetPort(
                                       phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                       )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_OpenPort(
                                        phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                        )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_ClosePort(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_Exchange(
                                        phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    I2C_XFER_T xfer;

    /* Only 7Bits of slave address should be initialized as Last bit(R/W) will be
     * appended automatically based on the operation done. */
    xfer.slaveAddr  =   0x28;
    xfer.txBuff     =   pTxBuffer;
    xfer.txSz       =   wTxLength;
    xfer.rxBuff     =   pRxBuffer;
    xfer.rxSz       =   wRxBufSize;

    if (I2C_STATUS_DONE != Chip_I2C_MasterTransfer(LPC_I2C, &xfer))
    {
        return (PH_ERR_INTERFACE_ERROR | PH_COMP_BAL);
    }

    if(pRxLength != NULL)
    {
        *pRxLength = (uint16_t)(wRxBufSize - xfer.rxSz);
    }

    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_SetConfig(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         )
{
    return PH_ERR_SUCCESS;
}

phStatus_t phbalReg_LpcOpenI2c_GetConfig(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         )
{
    return PH_ERR_SUCCESS;
}

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */
