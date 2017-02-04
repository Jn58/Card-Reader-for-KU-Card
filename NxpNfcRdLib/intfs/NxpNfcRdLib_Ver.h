#ifndef NXPNFCRDLIB_VERSION_INFO_H_INCLUDED
#define NXPNFCRDLIB_VERSION_INFO_H_INCLUDED


/*
 *                  Copyright (c), NXP Semiconductors
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
/*
 * Version Information
 * Auto-Generated by:
 *     docs\22_rel_doc\version_update.py (v2016.10.17_00)
 *
 * Do not edit this file. Update version_info.txt instead. */


#define NxpNfcRdLib_VER_FILEDESCRIPTION    "NxpNfcRdLib_v4.040.05.011646_20161115"
#define NxpNfcRdLib_VER_FILEVERSION         4,40,5,11646
#define NxpNfcRdLib_VER_FILEVERSION_STR    "4,40,5,11646"
#define NxpNfcRdLib_VER_PRODUCTVERSION      4,40,5,11646
#define NxpNfcRdLib_VER_PRODUCTVERSION_STR "4,40,5,11646"
#define NxpNfcRdLib_VER_PRODUCT_STREAM (4)
#define NxpNfcRdLib_VER_MAJOR  (40)
#define NxpNfcRdLib_VER_MINOR  (5)
#define NxpNfcRdLib_VER_DEV    (11646)

/* ______GENERATED_FROM____version_info.txt
 * # _____________________________________________________________________________
 * # Product Description
 * #
 * sw_c_define_name = "NxpNfcRdLib"
 * sw_product_name  = "NxpNfcRdLib" # Product Name
 * sw_cs_namespace  = "NxpNfcRdLibNet" # C# Namespace
 * rd_lib_stream    = "v4"       # PN7462AU Flavour
 * # NN => Release No.
 * # X => 0 : Library w/o Crypto components
 * #      1 : Library with Crypto components
 * #      2 : Library with Crypto components - Internal usage only.
 * #
 * sw_v_major       = "040"     # Major Release NNX
 * sw_v_minor       = "05"      # SW Minor Version
 * sw_v_dev         = "01"      # SW Development Version
 * sw_maturity      = "B"       # SW Maturity "A" Alpha, "B":Beta, "P": Production
 * sw_metastring    = ""        # Any meta string
 * # _____________________________________________________________________________
 * #
 * # History: Descending
 * #                  :
 * # v4.040.05.011646 : NxpNfcRdLib_v4.040.05.011646_20161115
 * #                  : [artf212345] : Linux Porting updates for phOsal_ThreadDelay
 * #                  : [artf212238] : Enable On-Board Pull Up by SW for IRQ pin used for RC523.
 * #                  :
 * #                  :
 * # v4.040.05.001642 : NxpNfcRdLib_v4.040.05.001642_20161017
 * #                  : Fixed [artf200311] #FRDM
 * # v4.040.04.001641 : NxpNfcRdLib_v4.040.04.001641_20161010
 * #                  : Fixed for artifacts : [artf205559] [artf205886] [artf205966]
 * # v4.040.03.001640 : NxpNfcRdLib_v4.040.03.001640_20161005
 * #                  : Linux support
 * # V4.040.02.001638 : NxpNfcRdLib_v4.040.02.001638_20160923
 * #				   : Release of Simplified API - EMVCo profile and beta code of ISO Profile.
 * #				   : Fixed for artifacts : artf193863 artf191954 artf199412
 * # v4.040.01.001637 : NxpNfcRdLib_v4.040.01.001637_20160916
 * # v4.040.00.001636 : NxpNfcRdLib_v4.040.00.001636_20160907
 * # v4.031.00.001631 : NxpNfcRdLib_v4.031.00.001631_SmplAPI_20160801
 * # v4.030.00.011627 : PN7462AU_04.03.01_20160706 / NxpNfcRdLib_v4.030.00.011627_20160706
 * # 06.4010.01.02    : Based on NFCRDLIB_V4.010.01.021604
 * # 06.4010.01.02    : Based on NFCRDLIB_V4.010.01.021604
 * # 06.4010.05.00    : Based on NFCRDLIB_V4.010.01.011604
 * # 06.4010.04.00    : Based on NFCRDLIB_V4.010.01.001603
 * # 06.4010.03.00    : Based on NxpRdLib_4.010.00.001601_ExC
 * #                  : Based on @r4100 - 2016.01.07 of
 * #                  :    /branch/NxpNfcRdLib_Branch_R_Gate_V4.010.00.001601
 * #                  :
 * # 06.4010.02.00    : Based on NxpRdLib_4.010.00.001551_ExC
 * # 06.4010.01.00    : Based on NfcRdLib_V_Gate_RC3_V4.010.01.001550
 * # 06.092.11.04     : Based on NfcRdLib_V_Gate_PN5180_V3.092.12.001547
 * #                  : +Fix for artf126517 Frame Delay Time Listen -> Poll is not
 * #                  : being changed based on the minimum TR2 value received in sensb
 * #                  : res
 * # 06.092.11.03     : PN7462AU_02_07_00_MRA2_20151113
 * # 06.092.11.00     : Based on NfcRdLib_V_Gate_RC3_V3.092.11.001546
 * # 06.092.11.00     : Based on branch/NfcRdLib_V_Gate_RC2_V3.092.11.001541 (V3.092.11.001545)
 * # 06.092.11.00     : Based on NfcRdLib_V_Gate_RC2_V3.092.11.001541_TempWk45
 * # 06.092.10.00     : Based on V3.092.10.001539 of NxpRdLib
 * # 06.092.07.01     : Corresponding to PN7462AU_02_06_00_MRA2_20150911 Drop
 * # 06.092.07.00     : Based on V3.092.07.001537 of NxpRdLib
 * # 06.092.05.00     : Based on v3.092.05.001534 of NxpRdLib
 * # 06.090.00.00     : Based on v3.092.00.001529 of NxpRdLib
 * # 06.080.06.00     : Based on v3.082.06.001528 of NxpRdLib
 * # 06.080.05.00     : Based on v3.082.05.001526 of NxpRdLib
 * # 06.080.04.00     : Based on v3.082.04.001524 of NxpRdLib
 * # 06.080.03.01     : Fixes for artifacts : artf631092, artf631750, artf631751
 * # 06.080.03.00     : Based on v3.082.03.001522 of NxpRdLib
 * # 06.080.01.00     : Based on V3.082.01.001518 of NxpRdLib
 * # 06.080.00.00     : Based on V3.082.00.001516 of NxpRdLib
 * # 06.010.04.00     : Release PN640FLASH_00_08_00_20150227 with NxpRdLib
 * # 06.010.03.00     : Updated with the fix of DUT Handling Mode and
 * #                    HIF Frame mode for compatibility between LPC and DUT.
 * # 06.010.02.02     : UHost::DUT_Switch Renamed to UHost_HIF_Switch
 * # 06.010.02.01     : HIF Related changes
 * # 06.010.02.00     : Corresponding to pn640Flash_00.03.00_DEV_20140905
 * # 06.010.01.02     : phhalHw_UHost_GetVersion returns 32bit Dev Value
 * # 06.010.01.01     : Corresponding to pn640Flash_00.02.01_CES_TST_20140904
 * # 06.010.01.00     : Corresponding to pn640Flash_00.02.00_Dev_20140808
 * # 06.010.00.00     : Migrated to UHost/DUT based NxpRdLib
 * # 06.000.01.00     : Intial Release
 * # _____________________________________________________________________________
 */


#endif /* NXPNFCRDLIB_VERSION_INFO_H_INCLUDED */