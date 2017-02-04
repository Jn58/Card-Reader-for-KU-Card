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
 * phOsal_Config.h: Osal Configuration
 *
 * Project:  NxpNfcRdLib
 *
 *
 * $Date: 2016-10-10 22:12:05 +0530 (Mon, 10 Oct 2016) $
 * $Author: Purnank G (ing05193) $
 * $Revision: 6352 $ (v4.040.05.011646)
 */


#ifndef PHOSAL_CONFIG_H
#define PHOSAL_CONFIG_H

#include "ph_Status.h"

#ifdef NXPBUILD__PH_OSAL
/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */
#ifdef NXPBUILD__PH_OSAL_FREERTOS

#define PH_OSAL_CONFIG_FREERTOS_MAX_NUM_TASKS       3
#define PH_OSAL_CONFIG_FREERTOS_MAX_NUM_EVENTS      3
#define PH_OSAL_CONFIG_FREERTOS_MAX_NUM_SEMAPHORE   8
#define PH_OSAL_CONFIG_FREERTOS_MAX_NUM_MUTEX       2
#define PH_OSAL_CONFIG_FREERTOS_MAX_NUM_TIMERS      2

#endif /* NXPBUILD__PH_OSAL_FREERTOS */

#ifdef NXPBUILD__PH_OSAL_NULLOS

#define PH_OSAL_CONFIG_NULLOS_MAX_NUM_EVENTS        5
#define PH_OSAL_CONFIG_NULLOS_MAX_NUM_TIMERS        5

#endif /* NXPBUILD__PH_OSAL_FREERTOS */


#ifdef NXPBUILD__PH_OSAL_LINUX

#define PH_OSAL_CONFIG_LINUX_MAX_NUM_TASKS          5
#define PH_OSAL_CONFIG_LINUX_MAX_NUM_EVENTS         5

#endif /* NXPBUILD__PH_OSAL_LINUX */
/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

#endif /* NXPBUILD__PH_OSAL */
#endif /* PHOSAL_CONFIG_H */
