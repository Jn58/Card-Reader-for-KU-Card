/*
 * phPlatform_Port_K81_PN5180.h
 *
 *  Created on: Apr 14, 2016
 *      Author: nxp69678
 */

#ifndef NXPRDLIB_INTFS_PHPLATFORM_PORT_K8X_PN5180_H_
#define NXPRDLIB_INTFS_PHPLATFORM_PORT_K8X_PN5180_H_


#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_PN5180) && \
    defined (NXPBUILD__PH_KINETIS_K81)

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_dspi.h"
#ifdef NXPBUILD__PH_OSAL_FREERTOS
#include <fsl_dspi_freertos.h>
#endif

#define PORT_RESET            PORTB
#define GPIO_RESET            GPIOB
#define PIN_RESET             9
#define RESET_POWERDOWN_LEVEL PH_PLATFORM_SET_LOW
#define RESET_POWERUP_LEVEL   PH_PLATFORM_SET_HIGH

#define PORT_DWL_REQ          PORTC
#define GPIO_DWL_REQ          GPIOC
#define PIN_DWL_REQ           13

#define PORT_BUSY             PORTC
#define GPIO_BUSY             GPIOC
#define PIN_BUSY              7

/* Interrupt Configuration */
#define PORT_INTERRUPT        PORTB
#define GPIO_INTERRUPT        GPIOB
#define PIN_NO                22
#define PIN_INTERRUPT         (1 << PIN_NO)
#define EINT_IRQn             PORTB_IRQn
#define EINT_PRIORITY         8
#define RF_IRQ_Handler        PORTB_IRQHandler

#define INT_TRIGGER_TYPE      kPORT_InterruptRisingEdge

/* SPI Configuration */
#define DSPI_DATA_RATE        5000000U
#define DSPI_MASTER_CLOCK_SRC DSPI1_CLK_SRC
#define DSPI_IRQ              SPI1_IRQn
#define DSPI_IRQ_PRIORITY     7

#define DSPI_MASTER_BASE      DSPI1

//TODO: Unaware of SPI pin schmatics
#define PORT_SSP_1            PORTB
#define FIRST_PINNUM_SSP      11

#define PORT_SSP_2            PORTB
#define SECOND_PINNUM_SSP     16

#define PORT_SSP_3            PORTB
#define THIRD_PINNUM_SSP      17

#define PORT_SSP_SSEL         PORTB
#define GPIO_SSP_SSEL         GPIOB
#define SSEL_PINNUM_SSP       10

#endif

#endif /* NXPRDLIB_INTFS_PHPLATFORM_PORT_K8X_PN5180_H_ */
