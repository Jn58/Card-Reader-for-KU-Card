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
#ifndef PHPLATFORM_PORT_LPC11U68_RC523_H
#define PHPLATFORM_PORT_LPC11U68_RC523_H

#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_RC523) && \
    defined (NXPBUILD__PH_LPC11U68)

#include "board.h"



/* Reset pin configuration */
#define PORT_RESET         PH_PLATFORM_PORT1         /* Port of LPC1769 used to connect reset pin from Blueboard Pn512 v1.5. */
#define PIN_RESET          3                         /* Reset pin for Blueboard Pn512 v1.5. */

#define RESET_POWERDOWN_LEVEL PH_PLATFORM_SET_LOW
#define RESET_POWERUP_LEVEL   PH_PLATFORM_SET_HIGH

/* I2C Address pin configuration */
#define PORT_AD0           PH_PLATFORM_PORT1         /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PORT_AD1           PH_PLATFORM_PORT0         /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PIN_AD0            9                        /* I2C address pin1 from Blueboard Pn512 v1.5 connection to LPC1769. */
#define PIN_AD1            23                        /* I2C address pin2 from Blueboard Pn512 v1.5 connection to LPC1769. */
#define PORT_AD2           PH_PLATFORM_PORT0         /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PIN_AD2            14                        /* I2C address pin3 from Blueboard Pn512 v1.5 connection to LPC1769. */

/* Interrupt Configuration */
#define PORT_INTERRUPT     PH_PLATFORM_PORT1         /* Port of LPC1769 used to connect interrupt Pin from Blueboard Pn512 v1.5. */
#define PIN_NO             4                         /* Interrupt pin for Blueboard Pn512 v1.5 connection to LPC1769. */
#define EINT_IRQn          PIN_INT0_IRQn             /* External interrupt that needs to be enabled on controller to receive interrupts from FE. */
#define EINT_PRIORITY      5                         /* Priority of the external interrupt. */
#define EINT_CHANNEL       PININTCH0
#define RF_IRQ_Handler     CLIF_IRQHandler

/* I2C Configuration */
#define LPC_I2C            I2C0
#define LPC_DIS_I2C_IRQ    I2C0_IRQn
#define LPC_RESET_I2C      RESET_I2C0
#define PORT_SDA           PH_PLATFORM_PORT0
#define PIN_SDA            5

#define PORT_SCL           PH_PLATFORM_PORT0
#define PIN_SCL            4

/* SPI Configuration */
#define LPC_SSP            LPC_SSP0
#define SSP_CLOCKRATE      5000000
#define PORT_SSP           PH_PLATFORM_PORT0
#define PORT_SSP_SCK       PH_PLATFORM_PORT1
#define MOSI_PINNUM_SSP    9
#define MISO_PINNUM_SSP    8
#define SCK_PINNUM_SSP     29
#define SSEL_PINNUM_SSP    2

#endif

#endif /* PHPLATFORM_PORT_LPC11U68_RC523_H */
