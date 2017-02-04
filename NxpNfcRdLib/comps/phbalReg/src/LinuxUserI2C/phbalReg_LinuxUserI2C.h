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
 * phbalReg_LinuxUserI2C.h: BAL for linux User Mode
 *
 * Project:  NxpNfcRdLib
 *
 *
 * $Date: 2016-08-25 14:01:00 +0530 (Thu, 25 Aug 2016) $
 * $Author: Ankur Srivastava (nxp79569) $
 * $Revision: 5753 $ (v4.040.05.011646)
 */


#ifndef COMPS_PHBALREG_SRC_LINUXUSERI2C_PHBALREG_LINUXUSERI2C_H_
#define COMPS_PHBALREG_SRC_LINUXUSERI2C_PHBALREG_LINUXUSERI2C_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */
phStatus_t phbalReg_LinuxUserI2C_GetPortList(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    );

phStatus_t phbalReg_LinuxUserI2C_SetPort(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint8_t * pPortName
    );

phStatus_t phbalReg_LinuxUserI2C_OpenPort(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams
    );

phStatus_t phbalReg_LinuxUserI2C_ClosePort(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams
    );

phStatus_t phbalReg_LinuxUserI2C_Exchange(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t * pRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phbalReg_LinuxUserI2C_SetConfig(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    );

phStatus_t phbalReg_LinuxUserI2C_GetConfig(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
    );

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

#endif /* COMPS_PHBALREG_SRC_LINUXUSERI2C_PHBALREG_LINUXUSERI2C_H_ */
