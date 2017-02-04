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
* Generic BAL Component of Reader Library Framework.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 5953 $ (v4.040.05.011646)
* $Date: 2016-09-16 17:57:28 +0530 (Fri, 16 Sep 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>










#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
#include "LpcOpenSPI/phbalReg_LpcOpenSpi.h"
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
#include "LpcOpenI2c/phbalReg_LpcOpenI2c.h"
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */


#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
#include "KinetisSPI/phbalReg_KinetisSpi.h"
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
#include "KinetisI2C/phbalReg_KinetisI2C.h"
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
#include "LinuxUserSpi/phbalReg_LinuxUserSpi.h"
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
#include "LinuxKernelSpi/phbalReg_LinuxKernelSpi.h"
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
#include "LinuxUserI2C/phbalReg_LinuxUserI2C.h"
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


#ifdef NXPBUILD__PHBAL_REG

#ifndef NXPRDLIB_REM_GEN_INTFS

phStatus_t phbalReg_GetPortList(
                                void * pDataParams,
                                uint16_t wPortBufSize,
                                uint8_t * pPortNames,
                                uint16_t * pNumOfPorts
                                )
{
    phStatus_t PH_MEMLOC_REM status;
#ifdef NXPBUILD__PH_LOG
    uint16_t PH_MEMLOC_REM wCnt;
    uint16_t PH_MEMLOC_REM wLen;
#endif /* NXPBUILD__PH_LOG */

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_GetPortList");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wPortBufSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPortNames);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumOfPorts);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wPortBufSize_log, &wPortBufSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPortNames);
    PH_ASSERT_NULL (pNumOfPorts);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {









#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_GetPortList((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_GetPortList((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_GetPortList((phbalReg_KinetisSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_GetPortList((phbalReg_KinetisI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_GetPortList((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_GetPortList((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_GetPortList((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        for (wCnt = 0; wCnt < *pNumOfPorts; wCnt++)
        {
            wLen = (uint16_t)strlen((char*)pPortNames);
            PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPortNames_log, pPortNames, wLen);
            pPortNames += wLen;
        }
    }
#endif /* NXPBUILD__PH_LOG */
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pNumOfPorts_log, pNumOfPorts);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phbalReg_SetPort(
                            void * pDataParams,
                            uint8_t * pPortName
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_SetPort");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPortName);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPortName_log, pPortName, (uint16_t)strlen((char*)pPortName));
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPortName);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {










#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_SetPort((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_SetPort((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_SetPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_SetPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_SetPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, pPortName);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_SetPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, pPortName);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_SetPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, pPortName);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phbalReg_OpenPort(
                             void * pDataParams
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_OpenPort");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {










#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_OpenPort((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_OpenPort((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_OpenPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_OpenPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_OpenPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_OpenPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_OpenPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phbalReg_ClosePort(
                              void * pDataParams
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_ClosePort");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {










#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_ClosePort((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_ClosePort((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_ClosePort((phbalReg_KinetisSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_ClosePort((phbalReg_KinetisI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_ClosePort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_ClosePort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_ClosePort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phbalReg_Exchange(
                             void * pDataParams,
                             uint16_t wOption,
                             uint8_t * pTxBuffer,
                             uint16_t wTxLength,
                             uint16_t wRxBufSize,
                             uint8_t * pRxBuffer,
                             uint16_t * pRxLength
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_Exchange");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wRxBufSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wRxBufSize_log, &wRxBufSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (wTxLength) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {










#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_Exchange((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_Exchange((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_Exchange((phbalReg_KinetisSpi_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_Exchange((phbalReg_KinetisI2C_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_Exchange((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_Exchange((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_Exchange((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
        ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE))
        &&
        (pRxBuffer != NULL) &&
        (pRxLength != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRxBuffer_log, pRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phbalReg_SetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t wValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_SetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {









#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_SetConfig((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_SetConfig((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */


#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_SetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_SetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_SetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_SetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_SetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return PH_ADD_COMPCODE(status, PH_COMP_BAL);
}

phStatus_t phbalReg_GetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t * pValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phbalReg_GetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_BAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
    {









#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI
    case PHBAL_REG_LPCOPEN_SPI_ID:
        status = phbalReg_LpcOpenSpi_GetConfig((phbalReg_LpcOpenSpi_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C
    case PHBAL_REG_LPCOPEN_I2C_ID:
        status = phbalReg_LpcOpenI2c_GetConfig((phbalReg_LpcOpenI2c_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI
    case PHBAL_REG_KINETIS_SPI_ID:
        status = phbalReg_KinetisSpi_GetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C
    case PHBAL_REG_KINETIS_I2C_ID:
        status = phbalReg_KinetisI2C_GetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI
    case PHBAL_REG_LINUX_USER_SPI_ID:
        status = phbalReg_LinuxUserSpi_GetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI
    case PHBAL_REG_LINUX_KERNEL_SPI_ID:
        status = phbalReg_LinuxKernelSpi_GetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C
    case PHBAL_REG_LINUX_USER_I2C_ID:
        status = phbalReg_LinuxUserI2C_GetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
            break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return PH_ADD_COMPCODE(status, PH_COMP_BAL);
}

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHBAL_REG */
