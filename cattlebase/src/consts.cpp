#include "stdafx.h"
#include "consts.h"

const char g_szRegExPrice[] = "[0-9]{1,6}([,.][0-9]{0,2})?";
const char g_szRegExUnsigned[] = "[0-9]{1,6}([,.][0-9]{0,4})?";
const char g_szIDNoRegEx[] = "^[a-zA-Z]{3}[0-9]{6}|^[a-zA-Z]{2}[0-9]{7}";
const char g_szRegExSigned[] =  "[-+]?[0-9]{1,6}([,.][0-9]{0,4})?";
const char g_szREGONRegEx[] = "[0-9]{7}([0-9]{2})?([0-9]{5})?";
const char szHiddenCol[] = "***"; 
const char szMissing[] = "BRAK";
//const char g_szLinkFmt[] = "%s%04d";
const char PayWayNames[][10] =
{
	"przelew",
	"gotówka"
};
//const char g_szDocPrefix[][3] = {"PZ","WZ"};
//const char g_DOC_NO_FMT[] = "%04d";
//const char g_PRICE_FMT[] = "%.2f";
const char g_szYesNo[][4] = {"NIE","TAK"};
const COLORREF g_GotThisCowClr = RGB(0,159,0);
const COLORREF g_SoldThisCowClr = RGB(198,0,0);
//const COLORREF g_NeutralColor = RGB(0xFF,0xFF,0x7F);
const char g_EANMaskPL[] = "PL############";
const char g_EANMaskXX[] = "%c%c############";
const char g_PassMask[] = ">>########";
const char g_ZipMask[] = "##-###";
const char g_NIPMask[] = "##########";
const char g_WetNoMask[] = "########";
const char g_szEANMatchRegEx[] = _T("[a-zA-Z]{2}[0-9]{12}");





