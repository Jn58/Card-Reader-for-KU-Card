/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Platform file abstracting HOST and front-end.
* $Author: Sundaresan Swaminathan (nxp36013) $
* $Revision: 5329 $ (v4.040.05.011646)
* $Date: 2016-06-30 10:23:42 +0530 (Thu, 30 Jun 2016) $
*
*/
#ifndef PHPLATFORM_PORT_LPC1769_RC663_H
#define PHPLATFORM_PORT_LPC1769_RC663_H

#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_RC663) && \
    defined (NXPBUILD__PH_LPC1769)

#include "board.h"



/* Reset pin configuration */
#define PORT_RESET         PH_PLATFORM_PORT0         /* Port of LPC1769 used to connect reset pin from Blueboard CLEV663B. */
#define PIN_RESET          5                         /* Reset pin for Blueboard CLEV663B. */

#define RESET_POWERDOWN_LEVEL PH_PLATFORM_SET_HIGH
#define RESET_POWERUP_LEVEL   PH_PLATFORM_SET_LOW

/* Interface select pin configuration */
#define PORT_IFSEL         PH_PLATFORM_PORT2         /* Port of LPC1769 used to connect interface select pins from Blueboard CLEV663B. */
#define PIN_IFSEL0         4                         /* Interface select pin to define com. interface */
#define PIN_IFSEL1         5                         /* Interface select pin to define com. interface */

/* I2C Address pin configuration */
#define PORT_AD0           PH_PLATFORM_PORT0         /* Port of LPC1769 used to connect I2C address pin from Blueboard CLEV663B. */
#define PORT_AD1           PH_PLATFORM_PORT0         /* Port of LPC1769 used to connect I2C address pin from Blueboard CLEV663B. */
#define PIN_AD0            17                        /* I2C address pin from Blueboard CLEV663B connection to LPC1769. */
#define PIN_AD1            15                        /* I2C address pin from Blueboard CLEV663B connection to LPC1769. */

/* Interrupt Configuration */
#define PORT_INTERRUPT     PH_PLATFORM_PORT2         /* Port of LPC1769 used to connect interrupt Pin from Blueboard CLEV663B. */
#define PIN_NO             12                        /* Interrupt pin for Blueboard CLEV663B connection to LPC1769. */
#define PIN_INTERRUPT      1 << PIN_NO               /* Position of Interrupt pin for Blueboard CLEV663B connection to LPC1769. */
#define EINT_IRQn          EINT3_IRQn                /* External interrupt that needs to be enabled on controller to receive interrupts from FE. */
#define EINT_PRIORITY      5                         /* Priority of the external interrupt. */
#define RF_IRQ_Handler     CLIF_IRQHandler
#define INT_TRIGGER_TYPE   PHPLATFORM_PORT_INT_FALLING_EDGE

/* I2C Configuration */
#define LPC_I2C            I2C2
#define LPC_DIS_I2C_IRQ    I2C2_IRQn

#define PORT_SDA           PH_PLATFORM_PORT0
#define PIN_SDA            10

#define PORT_SCL           PH_PLATFORM_PORT0
#define PIN_SCL            11

#define IOCON_DIGMODE_EN   0

/* SPI Configuration */
#define LPC_SSP            LPC_SSP1
#define SSP_CLOCKRATE      5000000
#define PORT_SSP           PH_PLATFORM_PORT0
#define PORT_SSP_SCK       PH_PLATFORM_PORT0
#define MOSI_PINNUM_SSP    9
#define MISO_PINNUM_SSP    8
#define SCK_PINNUM_SSP     7
#define SSEL_PINNUM_SSP    10

#endif

#endif /* PHPLATFORM_PORT_LPC1769_RC663_H */
