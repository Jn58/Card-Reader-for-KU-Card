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
* $Revision: 5076 $ (v4.040.05.011646)
* $Date: 2016-06-13 17:29:09 +0530 (Mon, 13 Jun 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHBALREG_LPCOPENI2C_H
#define PHBALREG_LPCOPENI2C_H

#include <ph_Status.h>
#include <phbalReg.h>

phStatus_t phbalReg_LpcOpenI2c_GetPortList(
                                           phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                           );

phStatus_t phbalReg_LpcOpenI2c_SetPort(
                                       phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                       );

phStatus_t phbalReg_LpcOpenI2c_OpenPort(
                                        phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                        );

phStatus_t phbalReg_LpcOpenI2c_ClosePort(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams
                                         );

phStatus_t phbalReg_LpcOpenI2c_Exchange(
                                        phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        );

phStatus_t phbalReg_LpcOpenI2c_SetConfig(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         );

phStatus_t phbalReg_LpcOpenI2c_GetConfig(
                                         phbalReg_LpcOpenI2c_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         );

#endif /* PHBALREG_LPCOPENI2C_H */
