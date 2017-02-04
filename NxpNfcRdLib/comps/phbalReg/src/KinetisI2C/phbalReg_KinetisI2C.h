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
* $Revision: 5822 $ (v4.040.05.011646)
* $Date: 2016-08-30 23:01:47 +0530 (Tue, 30 Aug 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHBALREG_KINETISI2C_H
#define PHBALREG_KINETISI2C_H

#include <ph_Status.h>
#include <phbalReg.h>

phStatus_t phbalReg_KinetisI2C_GetPortList(
                                           phbalReg_KinetisI2C_DataParams_t * pDataParams
                                           );

phStatus_t phbalReg_KinetisI2C_SetPort(
                                       phbalReg_KinetisI2C_DataParams_t * pDataParams
                                       );

phStatus_t phbalReg_KinetisI2C_OpenPort(
                                        phbalReg_KinetisI2C_DataParams_t * pDataParams
                                        );

phStatus_t phbalReg_KinetisI2C_ClosePort(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams
                                         );

phStatus_t phbalReg_KinetisI2C_Exchange(
                                        phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength,
                                        uint16_t wRxBufSize,
                                        uint8_t * pRxBuffer,
                                        uint16_t * pRxLength
                                        );

phStatus_t phbalReg_KinetisI2C_SetConfig(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t wValue
                                         );

phStatus_t phbalReg_KinetisI2C_GetConfig(
                                         phbalReg_KinetisI2C_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint16_t * pValue
                                         );

#endif /* PHBALREG_KINETISI2C_H */
