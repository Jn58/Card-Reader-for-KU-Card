/*
 * phPlatform_Port_Pi_RC523.h
 *
 *  Created on: May 24, 2016
 *      Author: nxp69678
 */

#ifndef PHPLATFORM_PORT_PI_PN5180_H
#define PHPLATFORM_PORT_PI_PN5180_H

#include <ph_Status.h>


#if defined (NXPBUILD__PHHAL_HW_PN5180) && \
    defined (NXPBUILD__PH_RASPBERRY_PI)

#define PIN_RESET                   7   /**< "/sys/class/gpio/gpio7/" */
#define PIN_IRQ                     23  /**< "/sys/class/gpio/gpio23/" */
#define PIN_BUSY                    25  /**< "/sys/class/gpio/gpio25/" */

#define SPI_BUS                     0   /**< "/dev/spidev0.0" */
#define SPI_CS                      0   /**< "/dev/spidev0.0" */
#define SPI_FREQ                    5000000 /**< 5 MHz. */

#define RESET_POWERDOWN_LEVEL       PH_PLATFORM_SET_LOW
#define RESET_POWERUP_LEVEL         PH_PLATFORM_SET_HIGH

#define RF_IRQ_Handler              CLIF_IRQHandler
#define IRQ_PIN_TRIGGER_TYPE        PHPLATFORM_PORT_INT_RISING_EDGE
#define BUSY_PIN_TRIGGER_TYPE       PHPLATFORM_PORT_INT_FALLING_EDGE

/* Interface select pin configuration */
#define PORT_IFSEL
#define PIN_IFSEL0
#define PIN_IFSEL1

/* I2C Address pin configuration */
#define PORT_AD0                    /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PORT_AD1                    /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PIN_AD0                                    /* I2C address pin1 from Blueboard Pn512 v1.5 connection to LPC1769. */
#define PIN_AD1                                    /* I2C address pin2 from Blueboard Pn512 v1.5 connection to LPC1769. */
#define PORT_AD2                    /* Port of LPC1769 used to connect I2C address pin from Blueboard Pn512 v1.5. */
#define PIN_AD2                                    /* I2C address pin3 from Blueboard Pn512 v1.5 connection to LPC1769. */

#define EINT_IRQn                         /* External interrupt that needs to be enabled on controller to receive interrupts from FE. */
#define EINT_PRIORITY                               /* Priority of the external interrupt. */


/* I2C Configuration */
#define LPC_I2C
#define LPC_DIS_I2C_IRQ

#define PORT_SDA
#define PIN_SDA

#define PORT_SCL
#define PIN_SCL

#endif /* NXPBUILD__PHHAL_HW_PN5180 && NXPBUILD__PH_RASPBERRY_PI */

#endif /* PHPLATFORM_PORT_PI_PN5180_H */
