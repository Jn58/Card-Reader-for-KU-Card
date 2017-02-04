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
* $Revision: 6445 $ (v4.040.05.011646)
* $Date: 2016-11-11 12:26:54 +0530 (Fri, 11 Nov 2016) $
*
*/

#include <ph_Status.h>

#ifdef NXPBUILD__PH_KINETIS_K82

#include <phPlatform.h>
#include <phPlatform_Port_Host.h>
#include <fsl_port.h>

#include "system_MK82F25615.h"

#ifdef DEBUG

#include <fsl_clock.h>
#include <fsl_debug_console.h>

#define KINETIS_K82_DEBUG_UART_CLK_FREQ			CLOCK_GetOsc0ErClkFreq()
#define KINETIS_K82_DEBUG_UART_BASEADDR			((uint32_t)LPUART4)
#define KINETIS_K82_DEBUG_UART_BAUDRATE 		115200
#define KINETIS_K82_DEBUG_UART_TYPE 			DEBUG_CONSOLE_DEVICE_TYPE_LPUART

static void phPlatform_Port_InitDebugConsole(void);
#endif

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    osc_config_t oscConfig;       /*!< OSC configuration.      */
    uint32_t coreClock;           /*!< core clock frequency.   */
} clock_config_t;

/* Configuration for enter RUN mode. Core clock = 50MHz. */
const clock_config_t g_defaultClockConfigRun = {
    .mcgConfig =
        {
            .mcgMode = kMCG_ModePEE,             /* Work in PEE mode. */
            .irclkEnableMode = kMCG_IrclkEnable, /* MCGIRCLK enable. */
            .ircs = kMCG_IrcSlow,                /* Select IRC32k. */
            .fcrdiv = 0U,                        /* FCRDIV is 0. */

            .frdiv = 4U,
            .drs = kMCG_DrsLow,         /* Low frequency range. */
            .dmx32 = kMCG_Dmx32Default, /* DCO has a default range of 25%. */
            .oscsel = kMCG_OscselOsc,   /* Select OSC. */

            .pll0Config =
                {
                    .enableMode = 0U, .prdiv = 0x01U, .vdiv = 0x01U,
                },
        },
    .simConfig =
        {
            .pllFllSel = 1U,        /* PLLFLLSEL select PLL. */
            .pllFllDiv = 0U,        /* PLLFLLSEL clock divider divisor. */
            .pllFllFrac = 0U,       /* PLLFLLSEL clock divider fraction. */
            .er32kSrc = 5U,         /* ERCLK32K selection, use RTC. */
            .clkdiv1 = 0x01140000U, /* SIM_CLKDIV1. */
        },
    .oscConfig = {.freq = CPU_XTAL_CLK_HZ,
                  .capLoad = 0,
                  .workMode = kOSC_ModeOscLowPower,
                  .oscerConfig =
                      {
                          .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                          .erclkDiv = 0U,
#endif
                      }},

};

void phPlatform_Controller_Init(void)
{
    CLOCK_SetSimSafeDivs();

    CLOCK_InitOsc0(&g_defaultClockConfigRun.oscConfig);
    CLOCK_SetXtal0Freq(CPU_XTAL_CLK_HZ);

    CLOCK_BootToPeeMode(g_defaultClockConfigRun.mcgConfig.oscsel, kMCG_PllClkSelPll0,
                        &g_defaultClockConfigRun.mcgConfig.pll0Config);

    CLOCK_SetInternalRefClkConfig(g_defaultClockConfigRun.mcgConfig.irclkEnableMode,
                                  g_defaultClockConfigRun.mcgConfig.ircs, g_defaultClockConfigRun.mcgConfig.fcrdiv);

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);

    SystemCoreClockUpdate();

#ifdef DEBUG

	/* Initialize LPUART4 pins below used to Print */
	/* Ungate the port clock */
	CLOCK_EnableClock(kCLOCK_PortC);
	/* Affects PORTC_PCR14 register */
	PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);
	/* Affects PORTC_PCR15 register */
	PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);
    phPlatform_Port_InitDebugConsole();
#endif /* DEBUG */

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
}

phStatus_t phPlatform_Port_Host_SetPinConfig(phPlatform_Port_Host_config_t bConfig)
{
    gpio_pin_config_t pnK82_gpio_config;
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
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = RESET_POWERDOWN_LEVEL;

        /* Configure Kinetis pin to Output which is connected to Reset/PowerDown pin of FE. */
        PORT_SetPinConfig(PORT_RESET, PIN_RESET, &pinConfig);
        GPIO_PinInit(GPIO_RESET, PIN_RESET, &pnK82_gpio_config);
        break;

    case PHPLATFORM_PORT_PIN_IRQ:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalInput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Interrupt Configuration */
        NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
        /* Clear interrupt flag. */
        GPIO_ClearPinsInterruptFlags(GPIO_INTERRUPT, PIN_NO);

        /* Initialize the interrupt pin */
        pinConfig.pullSelect = kPORT_PullUp;
        PORT_SetPinConfig(PORT_INTERRUPT, PIN_NO, &pinConfig);
        PORT_SetPinInterruptConfig(PORT_INTERRUPT, PIN_NO, INT_TRIGGER_TYPE);
        GPIO_PinInit(GPIO_INTERRUPT, PIN_NO, &pnK82_gpio_config);

        /* Enable interrupt in the NVIC. */
        NVIC_ClearPendingIRQ(EINT_IRQn);
        EnableIRQ(EINT_IRQn);
        break;

    case PHPLATFORM_PORT_PIN_SPI:
        /* SPI Configuration */
        NVIC_SetPriority(DSPI_IRQ, DSPI_IRQ_PRIORITY);

        /* Configure SSP chip select. */
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 1;

        PORT_SetPinConfig(PORT_SSP_SSEL, SSEL_PINNUM_SSP, &pinConfig);
        GPIO_PinInit(GPIO_SSP_SSEL, SSEL_PINNUM_SSP, &pnK82_gpio_config);

        /* Configure SSP pins (SCK, MOSI and MISO) */
        pinConfig.pullSelect = kPORT_PullDisable;
        pinConfig.mux = kPORT_MuxAlt2;
        PORT_SetPinConfig(PORT_SSP_1, FIRST_PINNUM_SSP, &pinConfig);
        PORT_SetPinConfig(PORT_SSP_2, SECOND_PINNUM_SSP, &pinConfig);
        PORT_SetPinConfig(PORT_SSP_3, THIRD_PINNUM_SSP, &pinConfig);

        break;

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_I2C:
        NVIC_SetPriority(I2C_IRQ, I2C_IRQ_PRIORITY);

        /* Declare and initialise for pull up configuration */
        pinConfig.pullSelect = kPORT_PullUp;
        pinConfig.mux = kPORT_MuxAlt2;
        pinConfig.openDrainEnable = kPORT_OpenDrainEnable;
        pinConfig.driveStrength = kPORT_LowDriveStrength,
        /* I2C0 pull up resistor setting */
        PORT_SetPinConfig(PORT_I2C_1, FIRST_PINNUM_I2C, &pinConfig);
        PORT_SetPinConfig(PORT_I2C_2, SECOND_PINNUM_I2C, &pinConfig);
    	break;
#endif /* (NXPBUILD__PHHAL_HW_RC663) || (NXPBUILD__PHHAL_HW_RC523) */

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalInput;
        pnK82_gpio_config.outputLogic  = 1;

        /* Set Busy pin as GPIO input pin and disable interrupt functionality. */
        PORT_SetPinConfig(PORT_BUSY, PIN_BUSY, &pinConfig);
        PORT_SetPinInterruptConfig(PORT_BUSY, PIN_BUSY, kPORT_InterruptOrDMADisabled);
        GPIO_PinInit(GPIO_BUSY, PIN_BUSY, &pnK82_gpio_config);
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set DWL_REQ pin as GPIO pin */
        PORT_SetPinConfig(PORT_DWL_REQ, PIN_DWL_REQ, &pinConfig);
        GPIO_PinInit(GPIO_DWL_REQ, PIN_DWL_REQ, &pnK82_gpio_config);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set IFSEL0 pin as GPIO pin */
        PORT_SetPinConfig(PORT_IFSEL0_REQ, PIN_IFSEL0_REQ, &pinConfig);
        GPIO_PinInit(GPIO_IFSEL0_REQ, PIN_IFSEL0_REQ, &pnK82_gpio_config);
    	break;
    case PHPLATFORM_PORT_PIN_IFSEL1:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set IFSEL1 pin as GPIO pin */
        PORT_SetPinConfig(PORT_IFSEL1_REQ, PIN_IFSEL1_REQ, &pinConfig);
        GPIO_PinInit(GPIO_IFSEL1_REQ, PIN_IFSEL1_REQ, &pnK82_gpio_config);
    	break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */
#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set AD0 pin as GPIO pin */
        PORT_SetPinConfig(PORT_AD0_REQ, PIN_AD0_REQ, &pinConfig);
        GPIO_PinInit(GPIO_AD0_REQ, PIN_AD0_REQ, &pnK82_gpio_config);
    	break;
    case PHPLATFORM_PORT_PIN_AD1:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set AD1 pin as GPIO pin */
        PORT_SetPinConfig(PORT_AD1_REQ, PIN_AD1_REQ, &pinConfig);
        GPIO_PinInit(GPIO_AD1_REQ, PIN_AD1_REQ, &pnK82_gpio_config);
    	break;
#endif /* NXPBUILD__PHHAL_HW_RC663 || NXPBUILD__PHHAL_HW_RC523 */

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
        pnK82_gpio_config.pinDirection = kGPIO_DigitalOutput;
        pnK82_gpio_config.outputLogic  = 0;

        /* Set AD2 pin as GPIO pin */
        PORT_SetPinConfig(PORT_AD2_REQ, PIN_AD2_REQ, &pinConfig);
        GPIO_PinInit(GPIO_AD2_REQ, PIN_AD2_REQ, &pnK82_gpio_config);
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
        GPIO_WritePinOutput(GPIO_RESET, PIN_RESET, bVal);
        break;

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        break;

    case PHPLATFORM_PORT_PIN_DWL:
        GPIO_WritePinOutput(GPIO_DWL_REQ, PIN_DWL_REQ, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

#ifdef NXPBUILD__PHHAL_HW_RC663
    case PHPLATFORM_PORT_PIN_IFSEL0:
        GPIO_WritePinOutput(GPIO_IFSEL0_REQ, PIN_IFSEL0_REQ, bVal);
        break;

    case PHPLATFORM_PORT_PIN_IFSEL1:
    	GPIO_WritePinOutput(GPIO_IFSEL1_REQ, PIN_IFSEL1_REQ, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 */

#if defined (NXPBUILD__PHHAL_HW_RC663) || defined (NXPBUILD__PHHAL_HW_RC523)
    case PHPLATFORM_PORT_PIN_AD0:
    	GPIO_WritePinOutput(GPIO_AD0_REQ, PIN_AD0_REQ, bVal);
        break;

    case PHPLATFORM_PORT_PIN_AD1:
    	GPIO_WritePinOutput(GPIO_AD1_REQ, PIN_AD1_REQ, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC663 || NXPBUILD__PHHAL_HW_RC523 */

#ifdef NXPBUILD__PHHAL_HW_RC523
    case PHPLATFORM_PORT_PIN_AD2:
    	GPIO_WritePinOutput(GPIO_AD0_REQ, PIN_AD0_REQ, bVal);
        break;
#endif /* NXPBUILD__PHHAL_HW_RC523 */

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

#ifdef NXPBUILD__PHHAL_HW_PN5180
    case PHPLATFORM_PORT_PIN_BUSY:
        bStatus = GPIO_ReadPinInput(GPIO_BUSY, PIN_BUSY);
        break;
#endif /* NXPBUILD__PHHAL_HW_PN5180 */

    default:
        break;
    }

    return bStatus;
}

void phPlatform_Port_Host_ClearInt()
{
    GPIO_ClearPinsInterruptFlags(GPIO_INTERRUPT, PIN_INTERRUPT);
}

#ifdef DEBUG
static void phPlatform_Port_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK, or MCGPLLCLK, or IRC48M
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(2);

    uartClkSrcFreq = KINETIS_K82_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(KINETIS_K82_DEBUG_UART_BASEADDR, KINETIS_K82_DEBUG_UART_BAUDRATE, KINETIS_K82_DEBUG_UART_TYPE, uartClkSrcFreq);
}
#endif /* DEBUG */

#endif /* NXPBUILD__PH_KINETIS_K82 */
