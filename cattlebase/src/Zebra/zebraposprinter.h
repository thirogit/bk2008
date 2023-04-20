// Created by Microsoft (R) C/C++ Compiler Version 10.00.30319.01 (fbca6d37).
//
// c:\users\root\desktop\mvctest\test\gditest\debug\zebraposprinter.tlh
//
// C++ source equivalent of Win32 type library C:\\Program Files\\Zebra Technologies\\ZebraOPOS\\ZebraPOSPrinter.ocx
// compiler-generated file created 08/04/12 at 10:23:16 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

//
// Forward references and typedefs
//

struct __declspec(uuid("7930f49a-a29a-43e4-a201-cbbef79c9d66"))
/* LIBID */ __ZebraPOSPrinter_1_11_Lib;
struct __declspec(uuid("575fd1ce-81a9-413d-b586-cb73050d67b0"))
/* dispinterface */ _IZebraPOSPrinterEvents;
struct /* coclass */ ZebraPOSPrinter;
struct __declspec(uuid("da6ab9bc-fe80-4c30-a4d5-826169f1a45d"))
/* dual interface */ IZebraPOSPrinter;
struct __declspec(uuid("a6d26a71-835d-4502-b07f-768514a9c184"))
/* dual interface */ IZebraPOSPrinter14;
struct __declspec(uuid("585f4d3f-fde5-4f86-8954-05fa12cf4b55"))
/* dual interface */ IZebraPOSPrinter15;
struct __declspec(uuid("80397ac9-26b9-4cc7-893a-3a520281489f"))
/* dual interface */ IZebraPOSPrinter17;
struct __declspec(uuid("68c031a6-5414-4507-89ee-a522694859d8"))
/* dual interface */ IZebraPOSPrinter18;
struct __declspec(uuid("f634ff03-551b-4413-bb3a-dc10878ad215"))
/* dual interface */ IZebraPOSPrinter19;

//
// Type library items
//

struct __declspec(uuid("575fd1ce-81a9-413d-b586-cb73050d67b0"))
_IZebraPOSPrinterEvents : IDispatch
{};

struct __declspec(uuid("29faeed6-428f-4105-aa73-8b2922d489c8"))
ZebraPOSPrinter;
    // [ default ] interface IZebraPOSPrinter
    // interface IZebraPOSPrinter14
    // interface IZebraPOSPrinter15
    // interface IZebraPOSPrinter17
    // interface IZebraPOSPrinter18
    // interface IZebraPOSPrinter19
    // [ default, source ] dispinterface _IZebraPOSPrinterEvents

struct __declspec(uuid("da6ab9bc-fe80-4c30-a4d5-826169f1a45d"))
IZebraPOSPrinter : IDispatch
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall SODataDummy (
        /*[in]*/ long Status ) = 0;
      virtual HRESULT __stdcall SODirectIO (
        /*[in]*/ long EventNumber,
        /*[in,out]*/ long * pData,
        /*[in,out]*/ BSTR * pString ) = 0;
      virtual HRESULT __stdcall SOError (
        /*[in]*/ long ResultCode,
        /*[in]*/ long ResultCodeExtended,
        /*[in]*/ long ErrorLocus,
        /*[in,out]*/ long * pErrorResponse ) = 0;
      virtual HRESULT __stdcall SOOutputComplete (
        /*[in]*/ long OutputID ) = 0;
      virtual HRESULT __stdcall SOStatusUpdate (
        /*[in]*/ long Data ) = 0;
      virtual HRESULT __stdcall SOProcessID (
        /*[out,retval]*/ long * pProcessID ) = 0;
      virtual HRESULT __stdcall get_OpenResult (
        /*[out,retval]*/ long * pOpenResult ) = 0;
      virtual HRESULT __stdcall get_CheckHealthText (
        /*[out,retval]*/ BSTR * pCheckHealthText ) = 0;
      virtual HRESULT __stdcall get_Claimed (
        /*[out,retval]*/ VARIANT_BOOL * pClaimed ) = 0;
      virtual HRESULT __stdcall get_DeviceEnabled (
        /*[out,retval]*/ VARIANT_BOOL * pDeviceEnabled ) = 0;
      virtual HRESULT __stdcall put_DeviceEnabled (
        /*[in]*/ VARIANT_BOOL pDeviceEnabled ) = 0;
      virtual HRESULT __stdcall get_FreezeEvents (
        /*[out,retval]*/ VARIANT_BOOL * pFreezeEvents ) = 0;
      virtual HRESULT __stdcall put_FreezeEvents (
        /*[in]*/ VARIANT_BOOL pFreezeEvents ) = 0;
      virtual HRESULT __stdcall get_OutputID (
        /*[out,retval]*/ long * pOutputID ) = 0;
      virtual HRESULT __stdcall get_ResultCode (
        /*[out,retval]*/ long * pResultCode ) = 0;
      virtual HRESULT __stdcall get_ResultCodeExtended (
        /*[out,retval]*/ long * pResultCodeExtended ) = 0;
      virtual HRESULT __stdcall get_State (
        /*[out,retval]*/ long * pState ) = 0;
      virtual HRESULT __stdcall get_ControlObjectDescription (
        /*[out,retval]*/ BSTR * pControlObjectDescription ) = 0;
      virtual HRESULT __stdcall get_ControlObjectVersion (
        /*[out,retval]*/ long * pControlObjectVersion ) = 0;
      virtual HRESULT __stdcall get_ServiceObjectDescription (
        /*[out,retval]*/ BSTR * pServiceObjectDescription ) = 0;
      virtual HRESULT __stdcall get_ServiceObjectVersion (
        /*[out,retval]*/ long * pServiceObjectVersion ) = 0;
      virtual HRESULT __stdcall get_DeviceDescription (
        /*[out,retval]*/ BSTR * pDeviceDescription ) = 0;
      virtual HRESULT __stdcall get_DeviceName (
        /*[out,retval]*/ BSTR * pDeviceName ) = 0;
      virtual HRESULT __stdcall CheckHealth (
        /*[in]*/ long Level,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall ClaimDevice (
        /*[in]*/ long Timeout,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall ClearOutput (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall Close (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall DirectIO (
        /*[in]*/ long Command,
        /*[in,out]*/ long * pData,
        /*[in,out]*/ BSTR * pString,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall Open (
        /*[in]*/ BSTR DeviceName,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall ReleaseDevice (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_AsyncMode (
        /*[out,retval]*/ VARIANT_BOOL * pAsyncMode ) = 0;
      virtual HRESULT __stdcall put_AsyncMode (
        /*[in]*/ VARIANT_BOOL pAsyncMode ) = 0;
      virtual HRESULT __stdcall get_CapConcurrentJrnRec (
        /*[out,retval]*/ VARIANT_BOOL * pCapConcurrentJrnRec ) = 0;
      virtual HRESULT __stdcall get_CapConcurrentJrnSlp (
        /*[out,retval]*/ VARIANT_BOOL * pCapConcurrentJrnSlp ) = 0;
      virtual HRESULT __stdcall get_CapConcurrentRecSlp (
        /*[out,retval]*/ VARIANT_BOOL * pCapConcurrentRecSlp ) = 0;
      virtual HRESULT __stdcall get_CapCoverSensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapCoverSensor ) = 0;
      virtual HRESULT __stdcall get_CapJrn2Color (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrn2Color ) = 0;
      virtual HRESULT __stdcall get_CapJrnBold (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnBold ) = 0;
      virtual HRESULT __stdcall get_CapJrnDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnDhigh ) = 0;
      virtual HRESULT __stdcall get_CapJrnDwide (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnDwide ) = 0;
      virtual HRESULT __stdcall get_CapJrnDwideDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnDwideDhigh ) = 0;
      virtual HRESULT __stdcall get_CapJrnEmptySensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnEmptySensor ) = 0;
      virtual HRESULT __stdcall get_CapJrnItalic (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnItalic ) = 0;
      virtual HRESULT __stdcall get_CapJrnNearEndSensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnNearEndSensor ) = 0;
      virtual HRESULT __stdcall get_CapJrnPresent (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnPresent ) = 0;
      virtual HRESULT __stdcall get_CapJrnUnderline (
        /*[out,retval]*/ VARIANT_BOOL * pCapJrnUnderline ) = 0;
      virtual HRESULT __stdcall get_CapRec2Color (
        /*[out,retval]*/ VARIANT_BOOL * pCapRec2Color ) = 0;
      virtual HRESULT __stdcall get_CapRecBarCode (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecBarCode ) = 0;
      virtual HRESULT __stdcall get_CapRecBitmap (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecBitmap ) = 0;
      virtual HRESULT __stdcall get_CapRecBold (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecBold ) = 0;
      virtual HRESULT __stdcall get_CapRecDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecDhigh ) = 0;
      virtual HRESULT __stdcall get_CapRecDwide (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecDwide ) = 0;
      virtual HRESULT __stdcall get_CapRecDwideDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecDwideDhigh ) = 0;
      virtual HRESULT __stdcall get_CapRecEmptySensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecEmptySensor ) = 0;
      virtual HRESULT __stdcall get_CapRecItalic (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecItalic ) = 0;
      virtual HRESULT __stdcall get_CapRecLeft90 (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecLeft90 ) = 0;
      virtual HRESULT __stdcall get_CapRecNearEndSensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecNearEndSensor ) = 0;
      virtual HRESULT __stdcall get_CapRecPapercut (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecPapercut ) = 0;
      virtual HRESULT __stdcall get_CapRecPresent (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecPresent ) = 0;
      virtual HRESULT __stdcall get_CapRecRight90 (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecRight90 ) = 0;
      virtual HRESULT __stdcall get_CapRecRotate180 (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecRotate180 ) = 0;
      virtual HRESULT __stdcall get_CapRecStamp (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecStamp ) = 0;
      virtual HRESULT __stdcall get_CapRecUnderline (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecUnderline ) = 0;
      virtual HRESULT __stdcall get_CapSlp2Color (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlp2Color ) = 0;
      virtual HRESULT __stdcall get_CapSlpBarCode (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpBarCode ) = 0;
      virtual HRESULT __stdcall get_CapSlpBitmap (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpBitmap ) = 0;
      virtual HRESULT __stdcall get_CapSlpBold (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpBold ) = 0;
      virtual HRESULT __stdcall get_CapSlpDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpDhigh ) = 0;
      virtual HRESULT __stdcall get_CapSlpDwide (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpDwide ) = 0;
      virtual HRESULT __stdcall get_CapSlpDwideDhigh (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpDwideDhigh ) = 0;
      virtual HRESULT __stdcall get_CapSlpEmptySensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpEmptySensor ) = 0;
      virtual HRESULT __stdcall get_CapSlpFullslip (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpFullslip ) = 0;
      virtual HRESULT __stdcall get_CapSlpItalic (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpItalic ) = 0;
      virtual HRESULT __stdcall get_CapSlpLeft90 (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpLeft90 ) = 0;
      virtual HRESULT __stdcall get_CapSlpNearEndSensor (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpNearEndSensor ) = 0;
      virtual HRESULT __stdcall get_CapSlpPresent (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpPresent ) = 0;
      virtual HRESULT __stdcall get_CapSlpRight90 (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpRight90 ) = 0;
      virtual HRESULT __stdcall get_CapSlpRotate180 (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpRotate180 ) = 0;
      virtual HRESULT __stdcall get_CapSlpUnderline (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpUnderline ) = 0;
      virtual HRESULT __stdcall get_CharacterSet (
        /*[out,retval]*/ long * pCharacterSet ) = 0;
      virtual HRESULT __stdcall put_CharacterSet (
        /*[in]*/ long pCharacterSet ) = 0;
      virtual HRESULT __stdcall get_CharacterSetList (
        /*[out,retval]*/ BSTR * pCharacterSetList ) = 0;
      virtual HRESULT __stdcall get_CoverOpen (
        /*[out,retval]*/ VARIANT_BOOL * pCoverOpen ) = 0;
      virtual HRESULT __stdcall get_ErrorStation (
        /*[out,retval]*/ long * pErrorStation ) = 0;
      virtual HRESULT __stdcall get_FlagWhenIdle (
        /*[out,retval]*/ VARIANT_BOOL * pFlagWhenIdle ) = 0;
      virtual HRESULT __stdcall put_FlagWhenIdle (
        /*[in]*/ VARIANT_BOOL pFlagWhenIdle ) = 0;
      virtual HRESULT __stdcall get_JrnEmpty (
        /*[out,retval]*/ VARIANT_BOOL * pJrnEmpty ) = 0;
      virtual HRESULT __stdcall get_JrnLetterQuality (
        /*[out,retval]*/ VARIANT_BOOL * pJrnLetterQuality ) = 0;
      virtual HRESULT __stdcall put_JrnLetterQuality (
        /*[in]*/ VARIANT_BOOL pJrnLetterQuality ) = 0;
      virtual HRESULT __stdcall get_JrnLineChars (
        /*[out,retval]*/ long * pJrnLineChars ) = 0;
      virtual HRESULT __stdcall put_JrnLineChars (
        /*[in]*/ long pJrnLineChars ) = 0;
      virtual HRESULT __stdcall get_JrnLineCharsList (
        /*[out,retval]*/ BSTR * pJrnLineCharsList ) = 0;
      virtual HRESULT __stdcall get_JrnLineHeight (
        /*[out,retval]*/ long * pJrnLineHeight ) = 0;
      virtual HRESULT __stdcall put_JrnLineHeight (
        /*[in]*/ long pJrnLineHeight ) = 0;
      virtual HRESULT __stdcall get_JrnLineSpacing (
        /*[out,retval]*/ long * pJrnLineSpacing ) = 0;
      virtual HRESULT __stdcall put_JrnLineSpacing (
        /*[in]*/ long pJrnLineSpacing ) = 0;
      virtual HRESULT __stdcall get_JrnLineWidth (
        /*[out,retval]*/ long * pJrnLineWidth ) = 0;
      virtual HRESULT __stdcall get_JrnNearEnd (
        /*[out,retval]*/ VARIANT_BOOL * pJrnNearEnd ) = 0;
      virtual HRESULT __stdcall get_MapMode (
        /*[out,retval]*/ long * pMapMode ) = 0;
      virtual HRESULT __stdcall put_MapMode (
        /*[in]*/ long pMapMode ) = 0;
      virtual HRESULT __stdcall get_RecEmpty (
        /*[out,retval]*/ VARIANT_BOOL * pRecEmpty ) = 0;
      virtual HRESULT __stdcall get_RecLetterQuality (
        /*[out,retval]*/ VARIANT_BOOL * pRecLetterQuality ) = 0;
      virtual HRESULT __stdcall put_RecLetterQuality (
        /*[in]*/ VARIANT_BOOL pRecLetterQuality ) = 0;
      virtual HRESULT __stdcall get_RecLineChars (
        /*[out,retval]*/ long * pRecLineChars ) = 0;
      virtual HRESULT __stdcall put_RecLineChars (
        /*[in]*/ long pRecLineChars ) = 0;
      virtual HRESULT __stdcall get_RecLineCharsList (
        /*[out,retval]*/ BSTR * pRecLineCharsList ) = 0;
      virtual HRESULT __stdcall get_RecLineHeight (
        /*[out,retval]*/ long * pRecLineHeight ) = 0;
      virtual HRESULT __stdcall put_RecLineHeight (
        /*[in]*/ long pRecLineHeight ) = 0;
      virtual HRESULT __stdcall get_RecLineSpacing (
        /*[out,retval]*/ long * pRecLineSpacing ) = 0;
      virtual HRESULT __stdcall put_RecLineSpacing (
        /*[in]*/ long pRecLineSpacing ) = 0;
      virtual HRESULT __stdcall get_RecLinesToPaperCut (
        /*[out,retval]*/ long * pRecLinesToPaperCut ) = 0;
      virtual HRESULT __stdcall get_RecLineWidth (
        /*[out,retval]*/ long * pRecLineWidth ) = 0;
      virtual HRESULT __stdcall get_RecNearEnd (
        /*[out,retval]*/ VARIANT_BOOL * pRecNearEnd ) = 0;
      virtual HRESULT __stdcall get_RecSidewaysMaxChars (
        /*[out,retval]*/ long * pRecSidewaysMaxChars ) = 0;
      virtual HRESULT __stdcall get_RecSidewaysMaxLines (
        /*[out,retval]*/ long * pRecSidewaysMaxLines ) = 0;
      virtual HRESULT __stdcall get_SlpEmpty (
        /*[out,retval]*/ VARIANT_BOOL * pSlpEmpty ) = 0;
      virtual HRESULT __stdcall get_SlpLetterQuality (
        /*[out,retval]*/ VARIANT_BOOL * pSlpLetterQuality ) = 0;
      virtual HRESULT __stdcall put_SlpLetterQuality (
        /*[in]*/ VARIANT_BOOL pSlpLetterQuality ) = 0;
      virtual HRESULT __stdcall get_SlpLineChars (
        /*[out,retval]*/ long * pSlpLineChars ) = 0;
      virtual HRESULT __stdcall put_SlpLineChars (
        /*[in]*/ long pSlpLineChars ) = 0;
      virtual HRESULT __stdcall get_SlpLineCharsList (
        /*[out,retval]*/ BSTR * pSlpLineCharsList ) = 0;
      virtual HRESULT __stdcall get_SlpLineHeight (
        /*[out,retval]*/ long * pSlpLineHeight ) = 0;
      virtual HRESULT __stdcall put_SlpLineHeight (
        /*[in]*/ long pSlpLineHeight ) = 0;
      virtual HRESULT __stdcall get_SlpLinesNearEndToEnd (
        /*[out,retval]*/ long * pSlpLinesNearEndToEnd ) = 0;
      virtual HRESULT __stdcall get_SlpLineSpacing (
        /*[out,retval]*/ long * pSlpLineSpacing ) = 0;
      virtual HRESULT __stdcall put_SlpLineSpacing (
        /*[in]*/ long pSlpLineSpacing ) = 0;
      virtual HRESULT __stdcall get_SlpLineWidth (
        /*[out,retval]*/ long * pSlpLineWidth ) = 0;
      virtual HRESULT __stdcall get_SlpMaxLines (
        /*[out,retval]*/ long * pSlpMaxLines ) = 0;
      virtual HRESULT __stdcall get_SlpNearEnd (
        /*[out,retval]*/ VARIANT_BOOL * pSlpNearEnd ) = 0;
      virtual HRESULT __stdcall get_SlpSidewaysMaxChars (
        /*[out,retval]*/ long * pSlpSidewaysMaxChars ) = 0;
      virtual HRESULT __stdcall get_SlpSidewaysMaxLines (
        /*[out,retval]*/ long * pSlpSidewaysMaxLines ) = 0;
      virtual HRESULT __stdcall BeginInsertion (
        /*[in]*/ long Timeout,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall BeginRemoval (
        /*[in]*/ long Timeout,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall CutPaper (
        /*[in]*/ long Percentage,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall EndInsertion (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall EndRemoval (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintBarCode (
        /*[in]*/ long Station,
        /*[in]*/ BSTR Data,
        /*[in]*/ long Symbology,
        /*[in]*/ long Height,
        /*[in]*/ long Width,
        /*[in]*/ long Alignment,
        /*[in]*/ long TextPosition,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintBitmap (
        /*[in]*/ long Station,
        /*[in]*/ BSTR FileName,
        /*[in]*/ long Width,
        /*[in]*/ long Alignment,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintImmediate (
        /*[in]*/ long Station,
        /*[in]*/ BSTR Data,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintNormal (
        /*[in]*/ long Station,
        /*[in]*/ BSTR Data,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintTwoNormal (
        /*[in]*/ long Stations,
        /*[in]*/ BSTR Data1,
        /*[in]*/ BSTR Data2,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall RotatePrint (
        /*[in]*/ long Station,
        /*[in]*/ long Rotation,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall SetBitmap (
        /*[in]*/ long BitmapNumber,
        /*[in]*/ long Station,
        /*[in]*/ BSTR FileName,
        /*[in]*/ long Width,
        /*[in]*/ long Alignment,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall SetLogo (
        /*[in]*/ long Location,
        /*[in]*/ BSTR Data,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_CapCharacterSet (
        /*[out,retval]*/ long * pCapCharacterSet ) = 0;
      virtual HRESULT __stdcall get_CapTransaction (
        /*[out,retval]*/ VARIANT_BOOL * pCapTransaction ) = 0;
      virtual HRESULT __stdcall get_ErrorLevel (
        /*[out,retval]*/ long * pErrorLevel ) = 0;
      virtual HRESULT __stdcall get_ErrorString (
        /*[out,retval]*/ BSTR * pErrorString ) = 0;
      virtual HRESULT __stdcall get_FontTypefaceList (
        /*[out,retval]*/ BSTR * pFontTypefaceList ) = 0;
      virtual HRESULT __stdcall get_RecBarCodeRotationList (
        /*[out,retval]*/ BSTR * pRecBarCodeRotationList ) = 0;
      virtual HRESULT __stdcall get_RotateSpecial (
        /*[out,retval]*/ long * pRotateSpecial ) = 0;
      virtual HRESULT __stdcall put_RotateSpecial (
        /*[in]*/ long pRotateSpecial ) = 0;
      virtual HRESULT __stdcall get_SlpBarCodeRotationList (
        /*[out,retval]*/ BSTR * pSlpBarCodeRotationList ) = 0;
      virtual HRESULT __stdcall TransactionPrint (
        /*[in]*/ long Station,
        /*[in]*/ long Control,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall ValidateData (
        /*[in]*/ long Station,
        /*[in]*/ BSTR Data,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_BinaryConversion (
        /*[out,retval]*/ long * pBinaryConversion ) = 0;
      virtual HRESULT __stdcall put_BinaryConversion (
        /*[in]*/ long pBinaryConversion ) = 0;
      virtual HRESULT __stdcall get_CapPowerReporting (
        /*[out,retval]*/ long * pCapPowerReporting ) = 0;
      virtual HRESULT __stdcall get_PowerNotify (
        /*[out,retval]*/ long * pPowerNotify ) = 0;
      virtual HRESULT __stdcall put_PowerNotify (
        /*[in]*/ long pPowerNotify ) = 0;
      virtual HRESULT __stdcall get_PowerState (
        /*[out,retval]*/ long * pPowerState ) = 0;
      virtual HRESULT __stdcall get_CapJrnCartridgeSensor (
        /*[out,retval]*/ long * pCapJrnCartridgeSensor ) = 0;
      virtual HRESULT __stdcall get_CapJrnColor (
        /*[out,retval]*/ long * pCapJrnColor ) = 0;
      virtual HRESULT __stdcall get_CapRecCartridgeSensor (
        /*[out,retval]*/ long * pCapRecCartridgeSensor ) = 0;
      virtual HRESULT __stdcall get_CapRecColor (
        /*[out,retval]*/ long * pCapRecColor ) = 0;
      virtual HRESULT __stdcall get_CapRecMarkFeed (
        /*[out,retval]*/ long * pCapRecMarkFeed ) = 0;
      virtual HRESULT __stdcall get_CapSlpBothSidesPrint (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpBothSidesPrint ) = 0;
      virtual HRESULT __stdcall get_CapSlpCartridgeSensor (
        /*[out,retval]*/ long * pCapSlpCartridgeSensor ) = 0;
      virtual HRESULT __stdcall get_CapSlpColor (
        /*[out,retval]*/ long * pCapSlpColor ) = 0;
      virtual HRESULT __stdcall get_CartridgeNotify (
        /*[out,retval]*/ long * pCartridgeNotify ) = 0;
      virtual HRESULT __stdcall put_CartridgeNotify (
        /*[in]*/ long pCartridgeNotify ) = 0;
      virtual HRESULT __stdcall get_JrnCartridgeState (
        /*[out,retval]*/ long * pJrnCartridgeState ) = 0;
      virtual HRESULT __stdcall get_JrnCurrentCartridge (
        /*[out,retval]*/ long * pJrnCurrentCartridge ) = 0;
      virtual HRESULT __stdcall put_JrnCurrentCartridge (
        /*[in]*/ long pJrnCurrentCartridge ) = 0;
      virtual HRESULT __stdcall get_RecCartridgeState (
        /*[out,retval]*/ long * pRecCartridgeState ) = 0;
      virtual HRESULT __stdcall get_RecCurrentCartridge (
        /*[out,retval]*/ long * pRecCurrentCartridge ) = 0;
      virtual HRESULT __stdcall put_RecCurrentCartridge (
        /*[in]*/ long pRecCurrentCartridge ) = 0;
      virtual HRESULT __stdcall get_SlpCartridgeState (
        /*[out,retval]*/ long * pSlpCartridgeState ) = 0;
      virtual HRESULT __stdcall get_SlpCurrentCartridge (
        /*[out,retval]*/ long * pSlpCurrentCartridge ) = 0;
      virtual HRESULT __stdcall put_SlpCurrentCartridge (
        /*[in]*/ long pSlpCurrentCartridge ) = 0;
      virtual HRESULT __stdcall get_SlpPrintSide (
        /*[out,retval]*/ long * pSlpPrintSide ) = 0;
      virtual HRESULT __stdcall ChangePrintSide (
        /*[in]*/ long Side,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall MarkFeed (
        /*[in]*/ long Type,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_CapMapCharacterSet (
        /*[out,retval]*/ VARIANT_BOOL * pCapMapCharacterSet ) = 0;
      virtual HRESULT __stdcall get_MapCharacterSet (
        /*[out,retval]*/ VARIANT_BOOL * pMapCharacterSet ) = 0;
      virtual HRESULT __stdcall put_MapCharacterSet (
        /*[in]*/ VARIANT_BOOL pMapCharacterSet ) = 0;
      virtual HRESULT __stdcall get_RecBitmapRotationList (
        /*[out,retval]*/ BSTR * pRecBitmapRotationList ) = 0;
      virtual HRESULT __stdcall get_SlpBitmapRotationList (
        /*[out,retval]*/ BSTR * pSlpBitmapRotationList ) = 0;
      virtual HRESULT __stdcall get_CapStatisticsReporting (
        /*[out,retval]*/ VARIANT_BOOL * pCapStatisticsReporting ) = 0;
      virtual HRESULT __stdcall get_CapUpdateStatistics (
        /*[out,retval]*/ VARIANT_BOOL * pCapUpdateStatistics ) = 0;
      virtual HRESULT __stdcall ResetStatistics (
        /*[in]*/ BSTR StatisticsBuffer,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall RetrieveStatistics (
        /*[in,out]*/ BSTR * pStatisticsBuffer,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall UpdateStatistics (
        /*[in]*/ BSTR StatisticsBuffer,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_CapCompareFirmwareVersion (
        /*[out,retval]*/ VARIANT_BOOL * pCapCompareFirmwareVersion ) = 0;
      virtual HRESULT __stdcall get_CapUpdateFirmware (
        /*[out,retval]*/ VARIANT_BOOL * pCapUpdateFirmware ) = 0;
      virtual HRESULT __stdcall CompareFirmwareVersion (
        /*[in]*/ BSTR FirmwareFileName,
        /*[out]*/ long * pResult,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall UpdateFirmware (
        /*[in]*/ BSTR FirmwareFileName,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall get_CapConcurrentPageMode (
        /*[out,retval]*/ VARIANT_BOOL * pCapConcurrentPageMode ) = 0;
      virtual HRESULT __stdcall get_CapRecPageMode (
        /*[out,retval]*/ VARIANT_BOOL * pCapRecPageMode ) = 0;
      virtual HRESULT __stdcall get_CapSlpPageMode (
        /*[out,retval]*/ VARIANT_BOOL * pCapSlpPageMode ) = 0;
      virtual HRESULT __stdcall get_PageModeArea (
        /*[out,retval]*/ BSTR * pPageModeArea ) = 0;
      virtual HRESULT __stdcall get_PageModeDescriptor (
        /*[out,retval]*/ long * pPageModeDescriptor ) = 0;
      virtual HRESULT __stdcall get_PageModeHorizontalPosition (
        /*[out,retval]*/ long * pPageModeHorizontalPosition ) = 0;
      virtual HRESULT __stdcall put_PageModeHorizontalPosition (
        /*[in]*/ long pPageModeHorizontalPosition ) = 0;
      virtual HRESULT __stdcall get_PageModePrintArea (
        /*[out,retval]*/ BSTR * pPageModePrintArea ) = 0;
      virtual HRESULT __stdcall put_PageModePrintArea (
        /*[in]*/ BSTR pPageModePrintArea ) = 0;
      virtual HRESULT __stdcall get_PageModePrintDirection (
        /*[out,retval]*/ long * pPageModePrintDirection ) = 0;
      virtual HRESULT __stdcall put_PageModePrintDirection (
        /*[in]*/ long pPageModePrintDirection ) = 0;
      virtual HRESULT __stdcall get_PageModeStation (
        /*[out,retval]*/ long * pPageModeStation ) = 0;
      virtual HRESULT __stdcall put_PageModeStation (
        /*[in]*/ long pPageModeStation ) = 0;
      virtual HRESULT __stdcall get_PageModeVerticalPosition (
        /*[out,retval]*/ long * pPageModeVerticalPosition ) = 0;
      virtual HRESULT __stdcall put_PageModeVerticalPosition (
        /*[in]*/ long pPageModeVerticalPosition ) = 0;
      virtual HRESULT __stdcall ClearPrintArea (
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PageModePrint (
        /*[in]*/ long Control,
        /*[out,retval]*/ long * pRC ) = 0;
      virtual HRESULT __stdcall PrintMemoryBitmap (
        /*[in]*/ long Station,
        /*[in]*/ BSTR Data,
        /*[in]*/ long Type,
        /*[in]*/ long Width,
        /*[in]*/ long Alignment,
        /*[out,retval]*/ long * pRC ) = 0;
};

struct __declspec(uuid("a6d26a71-835d-4502-b07f-768514a9c184"))
IZebraPOSPrinter14 : IZebraPOSPrinter
{};

struct __declspec(uuid("585f4d3f-fde5-4f86-8954-05fa12cf4b55"))
IZebraPOSPrinter15 : IZebraPOSPrinter
{};

struct __declspec(uuid("80397ac9-26b9-4cc7-893a-3a520281489f"))
IZebraPOSPrinter17 : IZebraPOSPrinter
{};

struct __declspec(uuid("68c031a6-5414-4507-89ee-a522694859d8"))
IZebraPOSPrinter18 : IZebraPOSPrinter
{};

struct __declspec(uuid("f634ff03-551b-4413-bb3a-dc10878ad215"))
IZebraPOSPrinter19 : IZebraPOSPrinter
{};

//
// Named GUID constants initializations
//

extern "C" const GUID __declspec(selectany) LIBID_ZebraPOSPrinter_1_11_Lib =
    {0x7930f49a,0xa29a,0x43e4,{0xa2,0x01,0xcb,0xbe,0xf7,0x9c,0x9d,0x66}};
extern "C" const GUID __declspec(selectany) DIID__IZebraPOSPrinterEvents =
    {0x575fd1ce,0x81a9,0x413d,{0xb5,0x86,0xcb,0x73,0x05,0x0d,0x67,0xb0}};
extern "C" const GUID __declspec(selectany) CLSID_ZebraPOSPrinter =
    {0x29faeed6,0x428f,0x4105,{0xaa,0x73,0x8b,0x29,0x22,0xd4,0x89,0xc8}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter =
    {0xda6ab9bc,0xfe80,0x4c30,{0xa4,0xd5,0x82,0x61,0x69,0xf1,0xa4,0x5d}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter14 =
    {0xa6d26a71,0x835d,0x4502,{0xb0,0x7f,0x76,0x85,0x14,0xa9,0xc1,0x84}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter15 =
    {0x585f4d3f,0xfde5,0x4f86,{0x89,0x54,0x05,0xfa,0x12,0xcf,0x4b,0x55}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter17 =
    {0x80397ac9,0x26b9,0x4cc7,{0x89,0x3a,0x3a,0x52,0x02,0x81,0x48,0x9f}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter18 =
    {0x68c031a6,0x5414,0x4507,{0x89,0xee,0xa5,0x22,0x69,0x48,0x59,0xd8}};
extern "C" const GUID __declspec(selectany) IID_IZebraPOSPrinter19 =
    {0xf634ff03,0x551b,0x4413,{0xbb,0x3a,0xdc,0x10,0x87,0x8a,0xd2,0x15}};

#pragma pack(pop)
