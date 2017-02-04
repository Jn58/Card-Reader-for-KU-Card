/*
 * phPlatform_Port_LPC1769_PN5180.h
 *
 *  Created on: Apr 14, 2016
 *      Author: nxp69678
 */

#ifndef PHPLATFORM_PORT_LPC1769_PN5180_H
#define PHPLATFORM_PORT_LPC1769_PN5180_H


#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_PN5180) && \
    defined (NXPBUILD__PH_LPC1769)

#include "board.h"



#define PORT_RESET          PH_PLATFORM_PORT2
#define PIN_RESET           5

#define PORT_DWL_REQ        PH_PLATFORM_PORT0
#define PIN_DWL_REQ         21

#define PORT_BUSY           PH_PLATFORM_PORT2
#define PIN_BUSY            11

#define PORT_INTERRUPT      PH_PLATFORM_PORT2
#define PIN_NO              12
#define PIN_INTERRUPT       (1 << PIN_NO)
#define EINT_IRQn           EINT3_IRQn
#define EINT_PRIORITY       5
#define RF_IRQ_Handler     CLIF_IRQHandler
#define INT_TRIGGER_TYPE   PHPLATFORM_PORT_INT_RISING_EDGE

#define RESET_POWERDOWN_LEVEL PH_PLATFORM_SET_LOW
#define RESET_POWERUP_LEVEL   PH_PLATFORM_SET_HIGH

/* SPI Configuration */
#define LPC_SSP             LPC_SSP0
#define SSP_CLOCKRATE       5000000
#define PORT_SSP            PH_PLATFORM_PORT0
#define PORT_SSP_SCK        PH_PLATFORM_PORT0
#define MOSI_PINNUM_SSP     18
#define MISO_PINNUM_SSP     17
#define SCK_PINNUM_SSP      15
#define SSEL_PINNUM_SSP     16

#endif

#endif /* PHPLATFORM_PORT_LPC1769_PN5180_H */
