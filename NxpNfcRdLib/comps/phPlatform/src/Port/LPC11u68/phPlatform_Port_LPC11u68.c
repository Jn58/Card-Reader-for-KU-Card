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
* $Revision: 5812 $ (v4.040.05.011646)
* $Date: 2016-08-30 20:13:02 +0530 (Tue, 30 Aug 2016) $
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PH_LPC11U68

#include <phPlatform.h>
#include <phPlatform_Port_Host.h>

void phPlatform_Controller_Init(void)
{
	return ;
}

phStatus_t phPlatform_Port_Host_SetPinConfig(phPlatform_Port_Host_config_t bConfig)
{
    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_RESET:
        /*
         * Configure LPC pin to Output which is connected to Reset/PowerDown pin of RC663.
         * Reset can be done by sending a pulse on this pin or front-end can be put into
         * power down mode by setting high on this pin.
         */
        /* Set reset pin as GPIO pin */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_RESET, PIN_RESET, IOCON_MODE_INACT, IOCON_FUNC0);

        /* Set PDOWN pin to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_RESET, PIN_RESET);
        break;

    case PHPLATFORM_PORT_PIN_IRQ:
        /* Interrupt Configuration */
        /* Configure the LPC11U6x GPIO pin connected to IRQ pin of reader IC as input pin */
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, PORT_INTERRUPT, PIN_NO);

        /* configure IOCON 0th divider */
        Chip_Clock_SetIOCONFiltClockDiv(0, 1);

        /* Configure pin as GPIO with pullup and use optional IOCON divider
            0 with 3 filter clocks for input filtering */
        Chip_IOCON_PinMuxSet(LPC_IOCON, PORT_INTERRUPT, PIN_NO,
                 (IOCON_FUNC0 |
                  IOCON_MODE_PULLUP |
                  IOCON_CLKDIV(0) |
                  IOCON_S_MODE(1)));

        Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);

        /* Configure interrupt channel for the GPIO pin in SysCon block
         * In drivers 2v06 there is a bug in Chip_SYSCTL_SetPinInterrupt(), thus must be set manually here*/
        LPC_SYSCTL->PINTSEL[0] = (uint32_t) (24 + PIN_NO);

        /* Configure channel interrupt as edge sensitive and falling edge interrupt */
        Chip_PININT_ClearIntStatus(LPC_PININT, PININTCH(0));
        Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(0));
        Chip_PININT_EnableIntLow(LPC_PININT,  PININTCH(0));

        NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
        /* Enable interrupt in the NVIC */
        NVIC_ClearPendingIRQ(EINT_IRQn);
        NVIC_EnableIRQ(EINT_IRQn);
        break;

    case PHPLATFORM_PORT_PIN_SPI:
        /* Configure SSP pins (SCK, MOSI and MISO) */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, MOSI_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, MISO_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC1);
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP_SCK, SCK_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC1);

        /* Configure SSP chip select. */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, SSEL_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC0);
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP);
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP, PH_PLATFORM_SET_HIGH);
        break;

    case PHPLATFORM_PORT_PIN_I2C:
        Chip_SYSCTL_PeriphReset(LPC_RESET_I2C);

        Chip_IOCON_PinMuxSet(LPC_IOCON, PORT_SDA, PIN_SDA,
            (IOCON_FUNC1 | 0) | IOCON_DIGMODE_EN);

        Chip_IOCON_PinMuxSet(LPC_IOCON, PORT_SCL, PIN_SCL,
            (IOCON_FUNC1 | 0) | IOCON_DIGMODE_EN);
        break;

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        /*
         * Configure LPC11u68 pins connected to IFSEL pins of RC663 to Output.
         * Setting value will be done by phbalReg_XXX_Inti() API to respective value.
         */
        /* Set MCU pin connected to IFSEL0 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_IFSEL, PIN_IFSEL0);
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
        /* Set MCU pin connected to IFSEL1 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_IFSEL, PIN_IFSEL1);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

    case PHPLATFORM_PORT_PIN_AD0:
        /* Set port pin MCU pin connected to AD0 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD0, PIN_AD0);
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        /* Set port pin MCU pin connected to AD1 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD1, PIN_AD1);
        break;

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        /* Set port pin MCU pin connected to AD2 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD2, PIN_AD2);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC523 */

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
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_RESET, PIN_RESET, bVal);
        break;

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_IFSEL, PIN_IFSEL0, bVal);
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_IFSEL, PIN_IFSEL1, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

    case PHPLATFORM_PORT_PIN_AD0:
        /* Select I2C adresss -> AD0 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD0, PIN_AD0, PH_PLATFORM_SET_LOW);
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        /* Select I2C adresss -> AD1 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD1, PIN_AD1, PH_PLATFORM_SET_LOW);
        break;

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        /* Select I2C adresss -> AD2 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD2, PIN_AD2, PH_PLATFORM_SET_LOW);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC523 */

    default:
        break;
    }
}

bool phPlatform_Port_Host_GetPinValue(phPlatform_Port_Host_config_t bConfig)
{
    uint8_t regVal = 0;
    bool    bStatus = false;

    switch (bConfig)
    {
    case PHPLATFORM_PORT_PIN_IRQ:
        regVal = Chip_PININT_GetIntStatus(LPC_PININT);
        if (regVal == (1 << EINT_IRQn))
        {
            bStatus = true;
        }
        break;

    default:
        break;
    }

    return bStatus;
}

void phPlatform_Port_Host_ClearInt()
{
    Chip_PININT_ClearIntStatus(LPC_PININT, EINT_CHANNEL);
}

#endif /* NXPBUILD__PH_LPC11U68 */
