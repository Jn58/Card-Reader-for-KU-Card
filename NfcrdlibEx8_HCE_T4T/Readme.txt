            _____________________________________________________

                  NXPNFCRDLIB EXAMPLE : NFCRDLIBEX8_HCE_T4T
                             (V4.040.05.011646)
            _____________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the NfcrdlibEx8_HCE_T4T
3 Restrictions on NfcrdlibEx8_HCE_T4T
4 Configurations of NfcrdlibEx8_HCE_T4T
5 Package Contents
6 Mandatory materials (not included)
7 Optional HW (not included)
8 Hardware Configuration
9 Software Configuration
10 Steps to follow
11 List of supported NFC Reader Boards/ICs





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  NfcrdlibEx8_HCE_T4T example as well as it's known problems and
  restrictions.


2 Description of the NfcrdlibEx8_HCE_T4T
========================================

  - NfcrdlibEx8_HCE_T4T can be run with PNEV512B, PN5180 and PN7462AU.  (See
    Section-11 for supported versions)
  - This example will demonstrate card emulation of type 4A tag (T4AT).
  - If any reader device is brought in proximity then this example will go
    into card emulation mode.
  - In card emulation mode this allows both reading and writing of NDEF
    message by reader.
  - By default the configured NDEF message is "www.nxp.com".
  - The maximum NDEF length the reader can write is limited by NDEF file size
    used in example (default configured as 1024 bytes).


3 Restrictions on NfcrdlibEx8_HCE_T4T
=====================================

  - NfcrdlibEx8_HCE_T4T is restricted to be run on NFC Reader Boards/ICs
    mentioned in Section-11.
  - To execute the ISO PICC test bench using the configurations provided,
    Micropross MP500 TCL3 tool with ISO 10373-6 PICC test suite is required.
  - The APIs are intended for NXP contact-less reader ICs only, as such the
    APIs are not to be ported to any technology from any other vendor.
  - NXP will not support porting to any other vendor platform.
  - This software project requires NxpNfcRdLib v4.040.05.011646 or later.
  - To compile and flash this project into the LPC Family or PN7462AU, you
    need to import this project using NXP LPCXpresso v8.1.x or later.
  - To compile and flash this project into the K82 Freedom Board, you need to
    import this project using KDS-3.2.0 or later.
  - The packages for KDS and LPCXpresso are separate. Package for KDS can not
    be imported into LPCXpresso and vice-versa.


4 Configurations of NfcrdlibEx8_HCE_T4T
=======================================

  - This example can also be used for running ISO 10373-6 PICC test suite.
  - To run with PICC test suite, set ISO_10373_6_PICC_TEST_BENCH macro to 1
    in intfs/NfcrdlibEx8_HCE_T4T.h and load the Micropross test bench
    configuration file ISO_10373_6_PICC_Test_Bench_Config.mpecc in Micropross
    PICC test suite.
  - Enabling ISO_10373_6_PICC_TEST_BENCH macro will enable the extra lines of
    code needed for testing.


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - NfcrdlibEx8_HCE_T4T.c
    + Main example file.
  - intfs/NfcrdlibEx8_HCE_T4T.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - .cproject and .project
    + LPCXpresso project configuration file
  - ISO_10373_6_PICC_Test_Bench_Config.mpecc
    + Micropross ISO PICC test suite configuration file


6 Mandatory materials (not included)
====================================

  - LPCXpresso IDE as described on the web site:
    [http://www.nxp.com/products/software-and-tools/software-development-tools/:LPCXPRESSO]
  - Kinetis(R) Design Studio IDE as described on the web site:
    [http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/:KDS_IDE]
  - LPCXpresso LPC1769 / LPCXpresso 11u68 / Freedom K82 development boards
    For NFC ICs listed in Section-11 (Note: PN7462AU (Section-11 Bullet-3)
    does not need any other other microcontroller).


7 Optional HW (not included)
============================

  - To run PICC ISO 10373-6 test bench, Micropross MP500 TCL3 tool along with
    PICC test suite is required.


8 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11308: Quick Start Up Guide PNEV512B Board.
    + Important: See Section on "Soldering the interrupt connection" in
      that document.
  - AN11744: PN5180 Evaluation board quick start guide
  - UM10883: PN7462 Quick Start Guide - Customer Board
    + Important: See Section on "Adding PN7462AU Plugin" in that document.


9 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller,
    Cortex M0+ LPC11u68 micro-controller, Cortex M4 Freedom K82 and Cortex M0
    based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and
    various NFC ICs, appropriate changes are required as mentioned in
    Section-10, Bullet-3, Bullet-4 and Bullet-5.
  - Listen Mode: In discovery loop listen mode, active listen is disabled and
    only passive listen is enabled to support Type 4A card emulation. Poll
    mode is not used.
  - ISO 10373-6 PICC Test Bench: To run this project with ISO PICC test
    bench, set ISO_10373_6_PICC_TEST_BENCH macro to 1
    (intfs/NfcrdlibEx8_HCE_T4T.h) and load the configuration file
    ISO_10373_6_PICC_Test_Bench_Config.mpecc in the Micropross PICC test
    suite.


10 Steps to follow
==================

  Please note - If you have not done the hardware changes as required in
  Section-8, please do so.

  Steps for opening the package in LPCXpresso:

  1) For running this application, connect one of the boards as listed in
     Section-11

  2) Create a new LpcXpresso workspace and import this project and it's
     dependencies:

     For LPC1769 : NxpNfcRdLib, lpc_board_nxp_lpcxpresso_1769, lpc_chip_175x_6x
                   and FreeRTOS_Library needs to be imported
     For LPC11u68: NxpNfcRdLib, lpc_board_nxp_lpcxpresso_11u68, lpc_chip_11u6x
                   and FreeRTOS_Library needs to be imported
     For FRDMK82 : NxpNfcRdLib and SDK_2.0_FRDM-K82F and FreeRTOS_Library needs
                   to be imported
     For PN7462AU: NxpNfcRdLib, PN7462AU and FreeRTOS_Library needs to be
                   imported

  3) Ensure MCU Type in "Project Properties" --> "C/C++ Build" --> "MCU
     Settings" for this project and its dependencies as listed in Step-2 is
     set Correctly:

     For LPC1769 : LPC1700 --> LPC1769
     For LPC11u68: LPC11U6x --> LPC11U68
     For PN7462AU: PN74xxxx --> PN7462AU-C3-00

  4) Ensure Build Configuration for this project is selected correctly.
     Where allowed, set the build configuration of dependent projects also.

     For LPC1769 : DebugLPC1769 / ReleaseLPC1769
     For LPC11u68: DebugLPC11u68 / ReleaseLPC11u68
     For PN7462AU: DebugPN7462AU / ReleasePN7462AU
     For FRDMK82 : DebugFRDMK82F / ReleaseFRDMK82F

  5) Please ensure that ph_NxpBuild_App.h is updated as per the selection of
     the IC and Platform.

     e.g. For LPC1769, NXPBUILD__PH_LPC1769 needs to be enabled, and
     NXPBUILD__PH_LPC11U68, NXPBUILD__PHHAL_HW_PN7462AU, etc. needs to be
     disabled.  Similarly, for NXPBUILD__PHHAL_HW_PN5180,
     NXPBUILD__PHHAL_HW_PN7462AU, NXPBUILD__PHHAL_HW_RC523 needs to be
     disabled.  Similar change is needed when switching to other supported
     Board/IC as listed in Section-11

  6) Build the project.

  7) Start the "Debug" session of IDE which will first flash the executable
     and start the task.

  8) When the application is running, if any reader device is brought in
     proximity then this example will behave like a Type 4A tag (in card
     emulation mode).

  9) In card emulation mode this allows both reading and writing of NDEF
     message by reader.


11 List of supported NFC Reader Boards/ICs
==========================================

  1) PNEV512B Blueboard 1.5
  2) PN5180 TFBGA64 performance board v1.0
  3) PN7462AU Customer Evaluation Board

  ---------------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/pages/:NFC-READER-LIBRARY]
