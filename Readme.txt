            _____________________________________________________

                        NXPNFCRDLIB LINUX SW PACKAGE :                       
                         (v4.040.05.011646_20161115)
            _____________________________________________________


Table of Contents
_________________

1 Document Purpose
2 Description of the package
3 Restrictions
4 Configurations
5 Package Contents
6 Mandatory materials (not included)
7 Hardware Configuration
8 Software Configuration
9 Steps to follow
10 List of supported NFC Reader Boards/ICs


1 Document Purpose
==================

  This document describes the content of the NXPNfcRdLib Linux package and
  steps how to run examples on Raspberry Pi. It also describes known problems
  and restrictions.
  More complete information can be find in:
  - AN11802 NFC Reader Library for Linux Installation Guidelines

  
2 Description of the package
===================================================

  The package contains implementation of the NXPNfcReaderLibrary, Linux OS
  support and examples to demonstrate how to use library.


3 Restrictions
================================================

  Package contains implementation for Linux only and it does not provide 
  support for any other platform.
  

4 Configurations 
==================================================

  This package is preconfigured for Linux OS (running on Raspberry Pi) and
  PN512 (EXPLORE-NFC demo kit).
  Any other must be reflected in the NXPNfcReaderLibrary header files (see 8).


5 Package Contents
==================

  - NfcrdlibEx... 
    + code examples of the NFC Reader Library use 
  - Nfcrdlib_SimplifiedAPI... 
  	+ code examples of the NFC Reader Library using simplified API
  - NxpNfcRdLib
    + NXPNfcReaderLibrary source code
  - CMakeLists.txt
    + input file for makefile structure creation
  - README.txt
    + this readme file

    
6 Mandatory materials (not included)
====================================

  - Raspberry Pi 2 or Raspberry Pi 3
  - Raspbian Jessie Lite from Raspberry Pi
  - NXP's NFC Reader demoboard

  
7 Hardware Configuration
========================

  Before starting this application, HW Changes may be required for the used
  board.  Refer to the following User Manuals / Application notes before
  starting with this example.
  - AN11480: Quick Start-up Guide for EXPLORE-NFC working with Raspberry Pi
  - AN11211: Quick Start Up Guide RC663 Blueboard
  - AN11308: Quick Start Up Guide PNEV512B Board
  - AN11744: PN5180 Evaluation board quick start guide

  
8 Software Configuration
========================

  NXP's NFC Reader selection is done in NxpNfcRdLib/types/ph_NxpBuild_Lpc.h 
  header file, defining the related compile flag: NXPBUILD__PHHAL_HW_RC523 for
  PN512, NXPBUILD__PHHAL_HW_RC663 for CLRC663, or NXPBUILD__PHHAL_HW_PN5180
  for PN5180.
  The way NFC Reader IC is connected is defined in header file located under 
  NxpNfcRdLib\intfs sub-folder: phPlatform_Port_Pi_RC523.h for PN512, 
  phPlatform_Port_Pi_RC663.h for CLRC663 and phPlatform_Port_Pi_PN5180.h for
  PN5180.

  
9 Steps to follow
=================

  Details described in:
  - AN11802 NFC Reader Library for Linux Installation Guidelines


10 List of supported NFC Reader Boards/ICs
==========================================

  1) EXPLORE-NFC
  2) PNEV512B Blueboard 1.5
  3) CLEV663B v3.0
  4) PN5180 TFBGA64 performance board v2.0
  
  ---------------------------------------------------------------------------

  For updates of this package, see [http://www.nxp.com/pages/:NFC-READER-LIBRARY]
  For more complete description see [http://www.nxp.com/documents/application_note/AN11802.pdf]
