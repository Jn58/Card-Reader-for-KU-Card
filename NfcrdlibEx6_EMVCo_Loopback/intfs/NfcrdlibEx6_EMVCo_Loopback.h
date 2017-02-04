/*
 * NfcrdlibEx6_EMVCo_Loopback.h
 *
 *  Created on: May 19, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX6_EMVCO_LOOPBACK_H_
#define INTFS_NFCRDLIBEX6_EMVCO_LOOPBACK_H_
#include <ph_Status.h>

#define PRETTY_PRINTING                                         /**< Enable pretty printing */
#define MiN_VALID_DATA_SIZE                     6
#define PHAC_EMVCO_MAX_BUFFSIZE               600               /**< Maximum buffer size for Emvco. */

typedef enum{
    eEmdRes_EOT = 0x70,
    eEmdRes_EOT_AnalogTests = 0x72,
    eEmdRes_SW_0 = 0x90,
    eEmdRes_SW_1 = 0x00,
}eEmvcoRespByte;

#ifdef NXPBUILD__PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define EMVCO_LOOPBACK_TASK_STACK              (2000/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define EMVCO_LOOPBACK_TASK_STACK              (2000/4)
#else /*  __PN74XXXX__*/
#define EMVCO_LOOPBACK_TASK_STACK              (2000)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define EMVCO_LOOPBACK_TASK_PRIO                4
#endif /* NXPBUILD__PH_OSAL_FREERTOS */

#ifdef NXPBUILD__PH_OSAL_LINUX
#define EMVCO_LOOPBACK_TASK_STACK              0x20000
#define EMVCO_LOOPBACK_TASK_PRIO               0
#endif /* NXPBUILD__PH_OSAL_LINUX */



#endif /* INTFS_NFCRDLIBEX6_EMVCO_LOOPBACK_H_ */
