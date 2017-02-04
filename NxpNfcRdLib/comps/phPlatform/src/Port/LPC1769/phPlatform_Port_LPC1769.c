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

#ifdef NXPBUILD__PH_LPC1769

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
         * Configure LPC pin to Output which is connected to Reset/PowerDown pin of FE.
         * Reset can be done by sending a pulse on this pin or front-end can be put into
         * power down mode by setting high on this pin.
         */
        /* Set reset pin as GPIO pin */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_RESET, PIN_RESET, IOCON_MODE_INACT, IOCON_FUNC0);

        /* Set RC663 PDOWN pin to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_RESET, PIN_RESET);
        break;

    case PHPLATFORM_PORT_PIN_IRQ:
        /* Interrupt Configuration */
        /* Clear any pending interrupts before enabling interrupt. */
        Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, PORT_INTERRUPT, PIN_INTERRUPT);

        /* Configure the LPC11U6x GPIO pin connected to IRQ pin of reader IC as input pin */
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, PORT_INTERRUPT, PIN_NO);

        if (INT_TRIGGER_TYPE == PHPLATFORM_PORT_INT_FALLING_EDGE)
        {
            /* Configure the GPIO interrupt */
            Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, PORT_INTERRUPT, PIN_INTERRUPT);
        }
        else
        {
            /* Configure the GPIO interrupt */
            Chip_GPIOINT_SetIntRising(LPC_GPIOINT, PORT_INTERRUPT, PIN_INTERRUPT);
        }

        NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
        /* Enable interrupt in the NVIC */
        NVIC_ClearPendingIRQ(EINT_IRQn);
        NVIC_EnableIRQ(EINT_IRQn);
        break;

    case PHPLATFORM_PORT_PIN_SPI:
        /* Configure SSP pins (SCK, MOSI and MISO) */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, MOSI_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, MISO_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC2);
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP_SCK, SCK_PINNUM_SSP, IOCON_MODE_INACT, IOCON_FUNC2);

        /* Configure SSP chip select. */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_SSP, SSEL_PINNUM_SSP, IOCON_MODE_PULLUP, IOCON_FUNC0);
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP);
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_SSP, SSEL_PINNUM_SSP, PH_PLATFORM_SET_HIGH);
        break;

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_I2C:
        Board_I2C_Init(LPC_I2C);
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        /* Set Busy pin as GPIO functionality */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_BUSY, PIN_BUSY, IOCON_MODE_PULLUP, IOCON_FUNC0);
        /* Configure as input pin. */
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, PORT_BUSY, PIN_BUSY);
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        /* Set DWL_REQ pin as GPIO pin */
        Chip_IOCON_PinMux(LPC_IOCON, PORT_DWL_REQ, PIN_DWL_REQ, IOCON_MODE_INACT, IOCON_FUNC0);
        /* Set port pin - DWL_REQ to output */
        Chip_GPIO_SetPinDIROutput( LPC_GPIO, PORT_DWL_REQ, PIN_DWL_REQ);
        /* DWL_REQ low and keep low forever */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_DWL_REQ, PIN_DWL_REQ, PH_PLATFORM_SET_LOW);
        break;
#endif
#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        /*
         * Configure HOST pins connected to IFSEL pins of RC663 to Output.
         * Setting value will be done by phhalHw_Rc663_Init() API to respective value based on BAL initialized.
         */
        /* Set MCU pin connected to IFSEL0 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_IFSEL, PIN_IFSEL0);
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
        /* Set MCU pin connected to IFSEL1 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_IFSEL, PIN_IFSEL1);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
        /* Set port pin MCU pin connected to AD0 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD0, PIN_AD0);
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        /* Set port pin MCU pin connected to AD1 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD1, PIN_AD1);
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        /* Set port pin MCU pin connected to AD2 to output */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, PORT_AD2, PIN_AD2);
        break;
#endif
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

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
        /* Select I2C adresss -> AD0 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD0, PIN_AD0, PH_PLATFORM_SET_LOW);
        break;

    case PHPLATFORM_PORT_PIN_AD1:
        /* Select I2C adresss -> AD1 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD1, PIN_AD1, PH_PLATFORM_SET_LOW);
        break;
#endif

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        /* Select I2C adresss -> AD2 = 0 */
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_AD2, PIN_AD2, PH_PLATFORM_SET_LOW);
        break;
#endif
#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_DWL:
        Chip_GPIO_SetPinState(LPC_GPIO, PORT_DWL_REQ, PIN_DWL_REQ, bVal);
        break;
#endif
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
        bStatus = Chip_GPIOINT_IsIntPending(LPC_GPIOINT, PORT_INTERRUPT);
        break;
#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        bStatus = Chip_GPIO_GetPinState(LPC_GPIO, PORT_BUSY, PIN_BUSY);
        break;
#endif
    default:
        break;
    }

    return bStatus;
}

void phPlatform_Port_Host_ClearInt()
{
    Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, PORT_INTERRUPT, PIN_INTERRUPT);
}

#endif /* NXPBUILD__PH_LPC1769 */
