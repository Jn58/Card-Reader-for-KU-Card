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
 * phbalReg_LinuxKernel.h:  BAL for linux Kernel Mode
 *
 * Project:  NxpNfcRdLib
 *
 * $Date: 2016-10-10 22:12:05 +0530 (Mon, 10 Oct 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 6352 $ (v4.040.05.011646)
 */


#ifndef COMPS_PHBALREG_SRC_LINUXKERNELSPI_PHBALREG_LINUXKERNELSPI_H_
#define COMPS_PHBALREG_SRC_LINUXKERNELSPI_PHBALREG_LINUXKERNELSPI_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include <ph_Status.h>

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
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
phStatus_t phbalReg_LinuxKernelSpi_GetPortList(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    );

phStatus_t phbalReg_LinuxKernelSpi_SetPort(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
    uint8_t * pPortName
    );

phStatus_t phbalReg_LinuxKernelSpi_OpenPort(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams
    );

phStatus_t phbalReg_LinuxKernelSpi_ClosePort(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams
    );

phStatus_t phbalReg_LinuxKernelSpi_Exchange(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t * pRxBuffer,
    uint16_t * pRxLength
    );

phStatus_t phbalReg_LinuxKernelSpi_SetConfig(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    );

phStatus_t phbalReg_LinuxKernelSpi_GetConfig(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t * pValue
    );

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */
#endif /* COMPS_PHBALREG_SRC_LINUXKERNELSPI_PHBALREG_LINUXKERNELSPI_H_ */
