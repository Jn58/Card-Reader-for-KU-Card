/*
 * NfcrdlibEx5_ISO15693.h
 *
 *  Created on: May 23, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX11_SIMPLIFIEDAPI_ISO_H_
#define INTFS_NFCRDLIBEX11_SIMPLIFIEDAPI_ISO_H_
#include <ph_Status.h>
#include <phNfcLib.h>
#include <Nfcrdlib_SimplifiedApi_ISO_15693.h>
#include <Nfcrdlib_SimplifiedApi_ISO_18000.h>
#include <Nfcrdlib_SimplifiedApi_ISO_MFDF.h>
#include <Nfcrdlib_SimplifiedApi_ISO_MFUL.h>
#include <Nfcrdlib_SimplifiedApi_ISO_MFC.h>

uint32_t NfcLib_MifareClassic_1k_Reference_app(void);
uint32_t NfcLib_ISO18000p3m3_Reference_app(void);
uint32_t NfcLib_ISO15693_Reference_app(void);
uint32_t NfcLib_MifareDesfire_Reference_app(void);
uint32_t NfcLib_MifareUltralight_Reference_app(void);
uint32_t NfcLib_TypeB_Reference_app(void);
uint32_t NfcLib_Layer4TypeA_Reference_app(void);

void phApp_Print_Buff(uint8_t *pBuff, uint8_t num);

#ifdef   DEBUG
	#ifdef NXPBUILD__PH_KINETIS_K82
		#define  DEBUG_PRINTF	DbgConsole_Printf
	#else /* NXPBUILD__PH_KINETIS_K82 */
		#include <stdio.h>
		#define  DEBUG_PRINTF(...) printf(__VA_ARGS__); fflush(stdout)
	#endif /* NXPBUILD__PH_KINETIS_K82 */
#else /* DEBUG */
	#define  DEBUG_PRINTF(...)
#endif /* DEBUG */
#endif /* INTFS_NFCRDLIBEX11_SIMPLIFIEDAPI_ISO_H_ */
