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
* $Revision: 6267 $ (v4.040.05.011646)
* $Date: 2016-10-04 16:44:49 +0530 (Tue, 04 Oct 2016) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHBALREG_H
#define PHBALREG_H

#include <ph_Status.h>
#include <phOsal.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHBAL_REG

/** \defgroup phbalReg Bus Abstraction Layer
* @{
*/

/**
* \brief BAL type structure
*/
typedef struct
{
    uint16_t       wId;              /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t        bBalType;         /**< BAL type used by HAL to configure the BAL configured at runtime. */
} phbalReg_Type_t;
/** @} */
#endif /* NXPBUILD__PHBAL_REG */












#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI

/** \defgroup phbalReg_LpcOpenSpi Component : LpcOpenSpi
* \brief Component abstracting the functionality of LPC Open stack SPI interface.
* @{
*/

#define PHBAL_REG_LPCOPEN_SPI_ID               0x0DU       /**< ID for LPC Open SPI BAL component */

/**
* \brief LPC Open SPI BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_LpcOpenSpi_DataParams_t;

/**
* \brief Initialize the LPC Open SPI BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LpcOpenSpi_Init(
                                    phbalReg_LpcOpenSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C

/** \defgroup phbalReg_LpcOpenI2c Component : LpcOpenI2c
* \brief Component abstracting the functionality of LPC Open stack I2C interface
* @{
*/

#define PHBAL_REG_LPCOPEN_I2C_ID               0x0EU       /**< ID for LPC Open I2C BAL component */

/**
* \brief LPC Open I2C BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_LpcOpenI2c_DataParams_t;


/**
* \brief Initialize the LPC Open I2C BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LpcOpenI2c_Init(
                                    phbalReg_LpcOpenI2c_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI

/** \defgroup phbalReg_KinetisSpi Component : KinetisSpi
* \brief Component abstracting the functionality of Kinetis KSDK 2.0 SPI interface.
* @{
*/

#define PHBAL_REG_KINETIS_SPI_ID               0x0FU       /**< ID for Kinetis SPI BAL component */

/**
* \brief Kinetis SPI BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_KinetisSpi_DataParams_t;

/**
* \brief Initialize the Kinetis SPI BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_KinetisSpi_Init(
                                    phbalReg_KinetisSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */


#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C

/** \defgroup phbalReg_KinetisI2C Component : KinetisI2C
* \brief Component abstracting the functionality of Kinetis KSDK 2.0 I2C interface.
* @{
*/

#define PHBAL_REG_KINETIS_I2C_ID               0x10U       /**< ID for Kinetis I2C BAL component */

/**
* \brief Kinetis I2C BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_KinetisI2C_DataParams_t;

/**
* \brief Initialize the Kinetis I2C BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_KinetisI2C_Init(
                                    phbalReg_KinetisI2C_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI

/** \defgroup phbalReg_LinuxKernelSpi Component : Linux Kernel Spi
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_KERNEL_SPI_ID               0x11U       /**< ID for Linux Kernel Spi BAL component */


typedef struct
{
    /* File descriptors */
    uint32_t spiFd;
}phbalReg_LinuxKernelSpiObj_t;

/**
* \brief BAL Linux Kernel Spi parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxKernelSpiObj_t sSpiObj;
} phbalReg_LinuxKernelSpi_DataParams_t;

/**
* \brief Initialise the BAL Linux Kernel Spi layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxKernelSpi_Init(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams                          /**< [In] Specifies the size of the data parameter structure. */
);

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */


#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI

/** \defgroup phbalReg_LinuxUserSpi Component : Linux User Spi
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_USER_SPI_ID               0x12U       /**< ID for Linux User Spi BAL component */

typedef struct
{
    /* Pin/bus numbers */
    uint32_t spiBus;
    uint32_t spiCsPinNum;

    /* File descriptors */
    uint32_t spiFd;
}phbalReg_LinuxUserSpiObj_t;

/**
* \brief BAL Linux User Spi parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxUserSpiObj_t sSpiObj;
} phbalReg_LinuxUserSpi_DataParams_t;

/**
* \brief Initialise the BAL Linux User Spi layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxUserSpi_Init(
    phbalReg_LinuxUserSpi_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams
    );


/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C

/** \defgroup phbalReg_LinuxUserI2C Component : Linux User I2C
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_USER_I2C_ID               0x13U       /**< ID for Linux User I2C BAL component */

typedef struct
{
    /* Pin number */
    uint32_t I2CNum;

    /* File descriptors */
    uint32_t I2CFd;
}phbalReg_LinuxUserI2CObj_t;

/**
* \brief BAL Linux User I2C parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxUserI2CObj_t sI2CObj;
} phbalReg_LinuxUserI2C_DataParams_t;

/**
* \brief Initialise the BAL Linux User I2C layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxUserI2C_Init(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams
    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */


#ifdef NXPBUILD__PHBAL_REG

/** \defgroup phbalReg Bus Abstraction Layer
* \brief These components implement the interfaces between physical host-device and physical reader-device.
* @{
*/

/**
* \name Generic BAL configuration parameters
*/
/*@{*/
#define PHBAL_REG_CONFIG_WRITE_TIMEOUT_MS   0x0000U /**< Configure transmission timeout in milliseconds. */
#define PHBAL_REG_CONFIG_READ_TIMEOUT_MS    0x0001U /**< Configure reception timeout in milliseconds. */

/**
* \name BAL types
*/
/*@{*/
#define PHBAL_REG_TYPE_SPI                  0x0001U /**< Configure transmission timeout in milliseconds. */
#define PHBAL_REG_TYPE_I2C                  0x0002U /**< Configure reception timeout in milliseconds. */
#define PHBAL_REG_TYPE_SERIAL_WIN           0x0003U /**< Configure reception timeout in milliseconds. */

/*@}*/

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LPCOPEN_SPI)

#include "../comps/phbalReg/src/LpcOpenSPI/phbalReg_LpcOpenSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
        phbalReg_LpcOpenSpi_GetPortList((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LpcOpenSpi_SetPort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LpcOpenSpi_OpenPort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LpcOpenSpi_ClosePort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LpcOpenSpi_Exchange((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LpcOpenSpi_SetConfig((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LpcOpenSpi_GetConfig((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LPCOPEN_I2C)

#include "../comps/phbalReg/src/LpcOpenI2C/phbalReg_LpcOpenI2c.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
        phbalReg_LpcOpenI2c_GetPortList((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LpcOpenI2c_SetPort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LpcOpenI2c_OpenPort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LpcOpenI2c_ClosePort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LpcOpenI2c_Exchange((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LpcOpenI2c_SetConfig((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LpcOpenI2c_GetConfig((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_USER_SPI)

#include "../comps/phbalReg/src/LinuxUserSpi/phbalReg_LinuxUserSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxUserSpi_GetPortList((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxUserSpi_SetPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxUserSpi_OpenPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxUserSpi_ClosePort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxUserSpi_Exchange((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxUserSpi_SetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxUserSpi_GetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI)

#include "../comps/phbalReg/src/LinuxKernelSpi/phbalReg_LinuxKernelSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxKernelSpi_GetPortList((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxKernelSpi_SetPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxKernelSpi_OpenPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxKernelSpi_ClosePort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxKernelSpi_Exchange((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxKernelSpi_SetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxKernelSpi_GetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_USER_I2C)

#include "../comps/phbalReg/src/LinuxUserI2C/phbalReg_LinuxUserI2C.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxUserI2C_GetPortList((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxUserI2C_SetPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxUserI2C_OpenPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxUserI2C_ClosePort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxUserI2C_Exchange((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxUserI2C_SetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxUserI2C_GetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_KINETIS_SPI)

#include "../comps/phbalReg/src/KinetisSPI/phbalReg_KinetisSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
		phbalReg_KinetisSpi_GetPortList((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_SetPort(pDataParams, pPortName) \
		phbalReg_KinetisSpi_SetPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_OpenPort(pDataParams) \
		phbalReg_KinetisSpi_OpenPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
		phbalReg_KinetisSpi_ClosePort((phbalReg_KinetisSpi_DataParams_t *) pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
		phbalReg_KinetisSpi_Exchange((phbalReg_KinetisSpi_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize,pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
		phbalReg_KinetisSpi_SetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
		phbalReg_KinetisSpi_GetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_KINETIS_I2C)

#include "../comps/phbalReg/src/KinetisI2C/phbalReg_KinetisI2C.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
		phbalReg_KinetisI2C_GetPortList((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_SetPort(pDataParams, pPortName) \
		phbalReg_KinetisI2C_SetPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_OpenPort(pDataParams) \
		phbalReg_KinetisI2C_OpenPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
		phbalReg_KinetisI2C_ClosePort((phbalReg_KinetisI2C_DataParams_t *) pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
		phbalReg_KinetisI2C_Exchange((phbalReg_KinetisI2C_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize,pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
		phbalReg_KinetisI2C_SetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
		phbalReg_KinetisI2C_GetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifndef NXPRDLIB_REM_GEN_INTFS

/**
* \brief List all available ports.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_BUFFER_OVERFLOW Given enumeration buffer is too small.
* \retval #PH_ERR_INTERFACE_ERROR Error while enumerating devices.
*/
phStatus_t phbalReg_GetPortList(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wPortBufSize,  /**< [In] Buffer Size of Port Name String. */
                                uint8_t * pPortNames,   /**< [Out] Port Name as Multi-String. */
                                uint16_t * pNumOfPorts  /**< [Out] Number of found port strings. */
                                );

/**
* \brief Select port to be used.
*
* <em>Example SerialWin:</em> The caller has to ensure that \c pPortName
* is valid throughout the whole lifetime of \c pDataParams.\n
* Furthermore, the caller is responsible for prepending "\\.\" if COM ports above
* COM9 need to be accessed.\n\n
* \b Example:
\code
strcpy(pPortName, "COM9");         <-- correct
strcpy(pPortName, "\\\\.\\COM9");  <-- correct
strcpy(pPortName, "\\\\.\\COM10"); <-- correct
strcpy(pPortName, "COM10");        <-- wrong
\endcode
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phbalReg_SetPort(
                            void * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                            uint8_t * pPortName /**< [In] Port Name as String. */
                            );

/**
* \brief Open communication port.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Communication port is already open.
* \retval #PH_ERR_INTERFACE_ERROR Error while opening port.
*/
phStatus_t phbalReg_OpenPort(
                             void * pDataParams /**< [In] Pointer to this layer's parameter structure. */
                             );

/**
* \brief Close communication port.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Communication port is not open.
* \retval #PH_ERR_INTERFACE_ERROR Error while closing port.
*/
phStatus_t phbalReg_ClosePort(
                              void * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                              );

/**
* \brief Perform data exchange on the bus. This API is used for command exchange between front-end CLIF
* and HOST.
* SPI : This API will also perform required flow control for particular front-end.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
* \retval #PH_ERR_IO_TIMEOUT No response received within given time frame.
* \retval #PH_ERR_BUFFER_OVERFLOW Response is too big for either given receive buffer or internal buffer.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_Exchange(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint16_t wOption,      /**< [In] Option parameter. */
                             uint8_t * pTxBuffer,   /**< [In] Data to transmit. */
                             uint16_t wTxLength,    /**< [In] Number of bytes to transmit. */
                             uint16_t wRxBufSize,   /**< [In] Size of receive buffer / Number of bytes to receive (depending on implementation). */
                             uint8_t * pRxBuffer,   /**< [Out] Received data. */
                             uint16_t * pRxLength   /**< [Out] Number of received data bytes. */
                             );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_SetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wConfig,     /**< [In] Configuration identifier. */
                              uint16_t wValue       /**< [In] Configuration value. */
                              );
/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_GetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wConfig,     /**< [In] Configuration identifier. */
                              uint16_t * pValue     /**< [Out] Configuration value. */
                              );

/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHBAL_REG */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHBALREG_H */
