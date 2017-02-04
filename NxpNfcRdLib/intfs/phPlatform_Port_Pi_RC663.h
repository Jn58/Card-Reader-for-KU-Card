/*
 * phPlatform_Port_Pi_RC523.h
 *
 *  Created on: May 24, 2016
 *      Author: nxp69678
 */

#ifndef PHPLATFORM_PORT_PI_RC663_H
#define PHPLATFORM_PORT_PI_RC663_H

#include <ph_Status.h>


#if defined (NXPBUILD__PHHAL_HW_RC663) && \
    defined (NXPBUILD__PH_RASPBERRY_PI)

#define PIN_RESET                   24   /**< "/sys/class/gpio/gpio24/" */
#define PIN_IRQ                     23  /**< "/sys/class/gpio/gpio23/" */
/* #define PIN_BUSY                 Busy Pin not required in Rc663 */

#define SPI_BUS                     0   /**< "/dev/spidev0.0" */
#define SPI_CS                      0   /**< "/dev/spidev0.0" */
#define SPI_FREQ                    5000000 /**< 5 MHz. */

#define I2C_NUM                     1   /**< "/dev/i2c-1" */
#define I2C_SLAVE_ADDR_7BIT         0x28

#define RESET_POWERDOWN_LEVEL       PH_PLATFORM_SET_HIGH
#define RESET_POWERUP_LEVEL         PH_PLATFORM_SET_LOW

#define RF_IRQ_Handler              CLIF_IRQHandler
#define IRQ_PIN_TRIGGER_TYPE        PHPLATFORM_PORT_INT_RISING_EDGE
/* #define BUSY_PIN_TRIGGER_TYPE    Busy Pin not there in Rc663 */

#define PIN_IFSEL0          27
#define PIN_IFSEL1          22

#define PIN_AD0             4
#define PIN_AD1             17
/* AD2 is not required in RC663
#define PIN_AD2             18
*/

#endif /* NXPBUILD__PHHAL_HW_RC663 && NXPBUILD__PH_RASPBERRY_PI */

#endif /* PHPLATFORM_PORT_PI_RC663_H */
