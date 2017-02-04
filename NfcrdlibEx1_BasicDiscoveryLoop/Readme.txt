            _____________________________________________________

                            NXPNFCRDLIB EXAMPLE :
                       NFCRDLIBEX1_BASICDISCOVERYLOOP
                             (V4.040.05.011646)
            _____________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the NfcrdlibEx1_BasicDiscoveryLoop
3 Restrictions on NfcrdlibEx1_BasicDiscoveryLoop
4 Configurations of NfcrdlibEx1_BasicDiscoveryLoop
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to follow
10 List of supported NFC Reader Boards/ICs





1 Document Purpose
==================

  This document describes the steps to be followed to execute
  NfcrdlibEx1_BasicDiscoveryLoop example as well as it's known problems and
  restrictions.


2 Description of the NfcrdlibEx1_BasicDiscoveryLoop
===================================================

  - NfcrdlibEx1_BasicDiscoveryLoop can be run with PNEV512B, CLEV663B, PN5180
    and PN7462AU.  (See Section-10 for supported versions)
  - This example will use both POLL and LISTEN modes of discovery loop.
  - It displays/prints detected tag information (like UID, SAK, Product Type
    for MIfare Cards) and prints information when it gets activated as a
    target by an external Initiator/reader.
  - Whenever multiple technologies are detected, example will select first
    detected technology to resolve.
  - This example will activate device at index zero whenever multiple devices
    are detected.


3 Restrictions on NfcrdlibEx1_BasicDiscoveryLoop
================================================

  - NfcrdlibEx1_BasicDiscoveryLoop is restricted to be run on NFC Reader
    Boards/ICs mentioned in Section-10.
  - While running in Debug Mode on LPC11u68, it is recommended to ensure
    optimization is set to Og or above to deterministically detect 18000P3P3
    Tags.
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


4 Configurations of NfcrdlibEx1_BasicDiscoveryLoop
==================================================

  - The example application is written to detect and activate one tag of
    supported technologies.
  - Listen mode is supported only for PN512, PN5180 and PN7462AU and the
    example application receives ATR_REQ or RATS from a NFC Peer/Reader.
    Additional functionality is not supported in this example.
  - With CLRC663, owing to the hardware limitations of CLRC663, it only
    supports poll without Active mode.


5 Package Contents
==================

  - Readme.txt
    + This readme file
  - NfcrdlibEx1_BasicDiscoveryLoop.c
    + Main example file.
  - intfs/NfcrdlibEx1_BasicDiscoveryLoop.h
    + Interfaces/Defines specific to the example
  - intfs/ph_NxpBuild_App.h
    + Reader library build configuration file
  - src/phApp_Init.c and intfs/phApp_Init.h
    + Common utility functions (common across all examples)
  - .cproject and .project
    + LPCXpresso project configuration file


6 Mandatory materials (not included)
====================================

  - LPCXpresso IDE as described on the web site:
    [http://www.nxp.com/products/software-and-tools/software-development-tools/:LPCXPRESSO]
  - Kinetis(R) Design Studio IDE as described on the web site:
    [http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/:KDS_IDE]
  - LPCXpresso LPC1769 / LPCXpresso 11u68 / Freedom K82 development boards
    For NFC ICs listed in Section-10 (Note: PN7462AU (Section-10 Bullet-4)
    does not need any other other microcontroller).


7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.

  - AN11308: Quick Start Up Guide PNEV512B Board.
    + Important: See Section on "Soldering the interrupt connection" in
      that document.
  - AN11211: Quick Start Up Guide RC663 Blueboard
    + Important: See Section on "Soldering the interrupt connection" in
      that document.
  - AN11744: PN5180 Evaluation board quick start guide
  - UM10883: PN7462 Quick Start Guide - Customer Board
    + Important: See Section on "Adding PN7462AU Plugin" in that document.


8 Software Configuration
========================

  - The Software can be compiled for Cortex M3 LPC1769 micro-controller,
    Cortex M0+ LPC11u68 micro-controller, Cortex M4 Freedom K82 and Cortex M0
    based PN7462AU from NXP.
  - Since this example can be configured to run on various MCU ICs and
    various NFC ICs, appropriate changes are required as mentioned in
    Section-9, Bullet-3, Bullet-4 and Bullet-5.


9 Steps to follow
=================

  Please note - If you have not done the hardware changes as required in
  Section-7, please do so.

  Steps for opening the package in LPCXpresso:

  1) For running this application, connect one of the boards as listed in
     Section-10

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
     Board/IC as listed in Section-10

  6) Build the project.

  7) Start the "Debug" session of IDE which will first flash the executable
     and start the task.

  8) When this application is running on the target MCU setup and when you
     bring the NFC cards or phone in proximity, the example application will
     detect and reports the NFC technology types detected.


10 List of supported NFC Reader Boards/ICs
==========================================

  1) PNEV512B Blueboard 1.5
  2) CLEV663B v3.0
  3) PN5180 TFBGA64 performance board v1.0
  4) PN7462AU Customer Evaluation Board

  ---------------------------------------------------------------------------

  For updates of this example, see
  [http://www.nxp.com/pages/:NFC-READER-LIBRARY]
