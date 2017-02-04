/*
 * phPlatform_Port_K8X_PN5180.h
 *
 *  Created on: Apr 14, 2016
 *      Author: nxp69678
 */

#ifndef NXPRDLIB_INTFS_PHPLATFORM_PORT_K82_PN5180_H_
#define NXPRDLIB_INTFS_PHPLATFORM_PORT_K82_PN5180_H_


#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_PN5180) && \
    defined (NXPBUILD__PH_KINETIS_K82)

//#include "board.h"
//#include "pin_mux.h"
//#include "clock_config.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_dspi.h"

#define RESET_POWERDOWN_LEVEL 	PH_PLATFORM_SET_LOW
#define RESET_POWERUP_LEVEL   	PH_PLATFORM_SET_HIGH

#define PORT_RESET            	PORTA
#define GPIO_RESET            	GPIOA
#define PIN_RESET             	13

#define PORT_INTERRUPT        	PORTC
#define GPIO_INTERRUPT        	GPIOC
#define PIN_NO                	7

#define PORT_BUSY             	PORTA
#define GPIO_BUSY             	GPIOA
#define PIN_BUSY              	5

#define PORT_DWL_REQ          	PORTA
#define GPIO_DWL_REQ          	GPIOA
#define PIN_DWL_REQ           	12

#define PORT_IFSEL0_REQ			PORTB
#define GPIO_IFSEL0_REQ			GPIOB
#define PIN_IFSEL0_REQ			16

#define PORT_IFSEL1_REQ			PORTB
#define GPIO_IFSEL1_REQ			GPIOB
#define PIN_IFSEL1_REQ			17

#define PORT_AD0_REQ			PORTB
#define GPIO_AD0_REQ			GPIOB
#define PIN_AD0_REQ				20

#define PORT_AD1_REQ			PORTB
#define GPIO_AD1_REQ			GPIOB
#define PIN_AD1_REQ				21

#define PORT_AD2_REQ			PORTB
#define GPIO_AD2_REQ			GPIOB
#define PIN_AD2_REQ				22

#define PIN_INTERRUPT         (1 << PIN_NO)
#define EINT_IRQn             PORTC_IRQn
#define EINT_PRIORITY         8
#define RF_IRQ_Handler        PORTC_IRQHandler

#define INT_TRIGGER_TYPE      kPORT_InterruptRisingEdge

/* SPI Configuration */
#define DSPI_DATA_RATE        5000000U
#define DSPI_MASTER_CLOCK_SRC DSPI0_CLK_SRC
#define DSPI_IRQ              SPI0_IRQn
#define DSPI_IRQ_PRIORITY     7

#define KINETIS_DSPI_MASTER      DSPI0

#define PORT_SSP_1            PORTA
#define FIRST_PINNUM_SSP      15

#define PORT_SSP_2            PORTA
#define SECOND_PINNUM_SSP     16

#define PORT_SSP_3            PORTA
#define THIRD_PINNUM_SSP      17

#define PORT_SSP_SSEL         PORTA
#define GPIO_SSP_SSEL         GPIOA
#define SSEL_PINNUM_SSP       14

#endif

#endif /* NXPRDLIB_INTFS_PHPLATFORM_PORT_K82_PN5180_H_ */
