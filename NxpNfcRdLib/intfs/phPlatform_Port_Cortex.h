/*
 * phPlatform_LpcPort.h
 *
 *  Created on: Apr 11, 2016
 *      Author: nxp69678
 */

#ifndef PHPLATFORM_PORT_CORTEX_H
#define PHPLATFORM_PORT_CORTEX_H

#ifdef __GNUC__
    #define __ENABLE_IRQ() __asm volatile ("cpsie i")
    #define __DISABLE_IRQ() __asm volatile ("cpsid i")
    #define __WFE() __asm volatile ("wfe")
    #define __SEV() __asm volatile ("sev")
#endif /* __GNUC__ */

#ifdef __ARMCC_VERSION
    #define __ENABLE_IRQ __enable_irq
    #define __DISABLE_IRQ __disable_irq
    #define __WFE __wfe
    #define __SEV __sev
#endif /* __ARMCC_VERSION */

#ifdef __ICCARM__
#   include "intrinsics.h"
#   define __NOP             __no_operation
#   define __ENABLE_IRQ      __enable_interrupt
#   define __DISABLE_IRQ     __disable_interrupt
#endif

#if defined (NXPBUILD__PH_LPC1769)     || \
    defined (NXPBUILD__PH_LPC11U68)    || \
    defined (NXPBUILD__PH_PN7462AU)    || \
    defined (NXPBUILD__PH_KINETIS)

#if defined (NXPBUILD__PH_LPC1769)
#   define PH_PLATFORM_PORT_CORTEX_NVIC_VECTACTIVE       0x000001FFU      /* for cortex m3. */
#elif defined (NXPBUILD__PH_LPC11U68) || defined (NXPBUILD__PH_PN7462AU)
#   define PH_PLATFORM_PORT_CORTEX_NVIC_VECTACTIVE       0x0000003FU      /* for cortex m0. */
#elif defined (NXPBUILD__PH_KINETIS)
#   define PH_PLATFORM_PORT_CORTEX_NVIC_VECTACTIVE       0x000001FFU      /* for cortex m4. */
#else
#   error "Either of NXPBUILD__PH_LPC1769 or NXPBUILD__PH_LPC11U68 or NXPBUILD__PH_PN7462AU or NXPBUILD__PH_KINETIS should be defined"
#endif

/*Interrupt Control and State Register*/
#define PH_PLATFORM_PORT_CORTEX_NVIC_INT_CTRL         ( (volatile uint32_t *) 0xe000ed04 )

#define phPlatform_EnterCriticalSection()       __DISABLE_IRQ()

#define phPlatform_ExitCriticalSection()       __ENABLE_IRQ()

#define phPlatform_Sleep()                     __WFE()

#define phPlatform_WakeUp()                    __SEV()

/**
 * Returns non zero value if the controller is in Isr context/Handler Mode.
 */
#define phPlatform_Is_Irq_Context()           ((*(PH_PLATFORM_PORT_CORTEX_NVIC_INT_CTRL) & PH_PLATFORM_PORT_CORTEX_NVIC_VECTACTIVE))

#endif
#endif /* PHPLATFORM_PORT_CORTEX_H */
