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
#ifndef PHPLATFORM_PORT_LPC11U68_RC663_H
#define PHPLATFORM_PORT_LPC11U68_RC663_H

#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_RC663) && \
    defined (NXPBUILD__PH_LPC11U68)

#include "board.h"



#define PORT_RESET         PH_PLATFORM_PORT1         /* Reset pin for Blueboard CLEV663B */
#define PIN_RESET          12                        /* Reset pin for Blueboard CLEV663B */

#define RESET_POWERDOWN_LEVEL PH_PLATFORM_SET_HIGH
#define RESET_POWERUP_LEVEL   PH_PLATFORM_SET_LOW

#define PORT_IFSEL         PH_PLATFORM_PORT2         /*  */
#define PIN_IFSEL0         2                         /* Select pin to define com. interface */
#define PIN_IFSEL1         7                         /* Select pin to define com. interface */

#define PORT_AD0           PH_PLATFORM_PORT0
#define PORT_AD1           PH_PLATFORM_PORT1
#define PIN_AD0            17
#define PIN_AD1            0


#define PORT_INTERRUPT     PH_PLATFORM_PORT1         /* Port of LPC1769 used to connection interrupt Pin from Blueboard CLEV663B. */
#define PIN_NO             6                         /* Interrupt pin for Blueboard CLEV663B connection to LPC1769. */
#define EINT_IRQn          PIN_INT0_IRQn
#define EINT_PRIORITY      5
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
#define SSEL_PINNUM_SSP    5

#endif

#endif    /* PHPLATFORM_PORT_LPC11U68_RC663_H */
