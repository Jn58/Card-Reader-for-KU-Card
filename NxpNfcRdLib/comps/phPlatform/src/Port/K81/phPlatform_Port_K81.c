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
* Platform file abstracting HOST features.
* $Author: Ankur Srivastava (nxp79569) $
* $Revision: 6231 $ (v4.040.05.011646)
* $Date: 2016-09-29 19:27:47 +0530 (Thu, 29 Sep 2016) $
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PH_KINETIS_K81

#include <phPlatform.h>
#include <phPlatform_Port_Host.h>

void phPlatform_Controller_Init(void)
{
	return ;
}
phStatus_t phPlatform_Port_Host_SetPinConfig(phPlatform_Port_Host_config_t bConfig)
{
    gpio_pin_config_t pnK81_gpio_config;
    port_pin_config_t pinConfig =
    {
        .pullSelect = kPORT_PullDisable,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAsGpio,
        .lockRegister = kPORT_UnlockRegister
    };

    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_RESET:
        pnK81_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK81_gpio_config.outputLogic  = 1;

        /* Configure Kinetis pin to Output which is connected to Reset/PowerDown pin of FE. */
        PORT_SetPinConfig(PORT_RESET, PIN_RESET, &pinConfig);
        GPIO_PinInit(GPIO_RESET, PIN_RESET, &pnK81_gpio_config);
        break;

    case PHPLATFORM_PORT_PIN_IRQ:
        pnK81_gpio_config.pinDirection = kGPIO_DigitalInput;
        pnK81_gpio_config.outputLogic  = 0;

        /* Interrupt Configuration */
        NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
        /* Clear interrupt flag. */
        GPIO_ClearPinsInterruptFlags(GPIO_INTERRUPT, PIN_NO);

        /* Initialize the interrupt pin */
        PORT_SetPinConfig(PORT_INTERRUPT, PIN_NO, &pinConfig);
        PORT_SetPinInterruptConfig(PORT_INTERRUPT, PIN_NO, INT_TRIGGER_TYPE);
        GPIO_PinInit(GPIO_INTERRUPT, PIN_NO, &pnK81_gpio_config);

        /* Enable interrupt in the NVIC. */
        NVIC_ClearPendingIRQ(EINT_IRQn);
        EnableIRQ(EINT_IRQn);
        break;

    case PHPLATFORM_PORT_PIN_SPI:
        /* SPI Configuration */
        NVIC_SetPriority(DSPI_IRQ, DSPI_IRQ_PRIORITY);

        /* Configure SSP chip select. */
        pnK81_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK81_gpio_config.outputLogic  = 1;

        PORT_SetPinConfig(PORT_SSP_SSEL, SSEL_PINNUM_SSP, &pinConfig);
        GPIO_PinInit(GPIO_SSP_SSEL, SSEL_PINNUM_SSP, &pnK81_gpio_config);

        /* Configure SSP pins (SCK, MOSI and MISO) */
        pinConfig.pullSelect = kPORT_PullDisable;
        pinConfig.mux = kPORT_MuxAlt2;
        PORT_SetPinConfig(PORT_SSP_1, FIRST_PINNUM_SSP, &pinConfig);
        PORT_SetPinConfig(PORT_SSP_2, SECOND_PINNUM_SSP, &pinConfig);
        PORT_SetPinConfig(PORT_SSP_3, THIRD_PINNUM_SSP, &pinConfig);
        break;

    case PHPLATFORM_PORT_PIN_BUSY:
        pnK81_gpio_config.pinDirection = kGPIO_DigitalInput;
        pnK81_gpio_config.outputLogic  = 1;

        /* Set Busy pin as GPIO input pin and disable interrupt functionality. */
        PORT_SetPinConfig(PORT_BUSY, PIN_BUSY, &pinConfig);
        PORT_SetPinInterruptConfig(PORT_BUSY, PIN_BUSY, kPORT_InterruptOrDMADisabled);
        GPIO_PinInit(GPIO_BUSY, PIN_BUSY, &pnK81_gpio_config);
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        pnK81_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK81_gpio_config.outputLogic  = 0;

        /* Set DWL_REQ pin as GPIO pin */
        PORT_SetPinConfig(PORT_DWL_REQ, PIN_DWL_REQ, &pinConfig);
        GPIO_PinInit(GPIO_DWL_REQ, PIN_DWL_REQ, &pnK81_gpio_config);
        break;

    default:
        break;
    }

    return PH_ERR_SUCCESS;
}

void phPlatform_Port_Host_SetPinValue(phPlatform_Port_Host_config_t bConfig, uint8_t bVal)
{
    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_RESET:
        GPIO_WritePinOutput(GPIO_RESET, PIN_RESET, bVal);
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        GPIO_WritePinOutput(GPIO_DWL_REQ, PIN_DWL_REQ, bVal);
        break;

    default:
        break;
    }
}

bool phPlatform_Port_Host_GetPinValue(phPlatform_Port_Host_config_t bConfig)
{
    bool    bStatus = false;

    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_IRQ:
        bStatus = (GPIO_GetPinsInterruptFlags(GPIO_INTERRUPT) & PIN_INTERRUPT);
        break;

    case PHPLATFORM_PORT_PIN_BUSY:
        bStatus = GPIO_ReadPinInput(GPIO_BUSY, PIN_BUSY);
        break;

    default:
        break;
    }

    return bStatus;
}

void phPlatform_Port_Host_ClearInt()
{
    GPIO_ClearPinsInterruptFlags(GPIO_INTERRUPT, PIN_INTERRUPT);
}

#endif /* NXPBUILD__PH_KINETIS_K81 */
