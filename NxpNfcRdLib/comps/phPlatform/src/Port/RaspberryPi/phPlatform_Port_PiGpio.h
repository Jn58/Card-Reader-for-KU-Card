/*
 * phbalReg_Stub_Utils.h
 *
 *  Created on: 26 Jan 2016
 *      Author: donatien
 */

#ifndef PHPLATFORM_PORT_PIGPIO_H_
#define PHPLATFORM_PORT_PIGPIO_H_

#include <ph_Status.h>

#ifdef NXPBUILD__PH_RASPBERRY_PI

#include <stddef.h>
#include <stdbool.h>

phStatus_t PiGpio_is_exported(size_t gpio);
phStatus_t PiGpio_export(size_t gpio);
phStatus_t PiGpio_unexport(size_t gpio);
phStatus_t PiGpio_set_direction(size_t gpio, bool output);
phStatus_t PiGpio_set_edge(size_t gpio, bool rising, bool falling);
phStatus_t PiGpio_Write(size_t gpio, int value);
phStatus_t PiGpio_read(size_t gpio, uint8_t *pGpioVal);
phStatus_t PiGpio_poll(size_t gpio, int highOrLow, int timeOutms);

#endif /* NXPBUILD__PH_RASPBERRY_PI */

#endif /* PHPLATFORM_PORT_PIGPIO_H_ */
