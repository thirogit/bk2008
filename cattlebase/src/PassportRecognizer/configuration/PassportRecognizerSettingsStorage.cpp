#include "stdafx.h"
#include "PassportRecognizerSettingsStorage.h"
#include "../../missing/util.h"
#include "../../missing/paths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define __CFG_IMPLEMENTATION__
#include "PassportRecognizerSettingsStorage.h"
									// When __CFG_IMPLEMENTATION__ is defined 
									// PassportRecognizerSettingsStorage.h actually defines the CFGDEF
									// structure array



PassportRecognizerSettingsStorage::PassportRecognizerSettingsStorage()
{
	m_storage.MakeCfgList(OCRCFG);
	CString sSettingsFilePath = paths_ConcatPath(CUtil::GetAppDir(),"PASSRECOGSETTINGS.CFG");
	m_storage.Init(sSettingsFilePath);	
}

PassportRecognizerSettingsStorage::~PassportRecognizerSettingsStorage()
{
}

void PassportRecognizerSettingsStorage::Save(PassportRecognizerSettings& settings)
{
	SaveDataSettings(settings.GetDataSettings());
	SaveEngineSettings(settings.GetEngineSettings());
	SaveImageSettings(settings.GetImageSettings());
	SaveScannerSettings(settings.GetScannerSettings());
	m_storage.Flush();
}

void PassportRecognizerSettingsStorage::Load(PassportRecognizerSettings& settings)
{
	LoadDataSettings(settings.GetDataSettings());
	LoadEngineSettings(settings.GetEngineSettings());
	LoadImageSettings(settings.GetImageSettings()); 
	LoadScannerSettings(settings.GetScannerSettings());
}

void PassportRecognizerSettingsStorage::SaveDataSettings(const DataSettings& dataSettings)
{
	m_storage.SetBool(CFG_UPPERCASEHENTDATA,dataSettings.GetDoUppercaseHent());
	m_storage.SetNum(CFG_DEFAULTSTOCKID,dataSettings.GetDefaultStockId());
	m_storage.SetBool(CFG_USEDEFAULTSTOCK,dataSettings.GetUseDefaultStock());
	m_storage.SetBool(CFG_NOUPDATEIFPRESENT,dataSettings.GetDoNotUpdateValueIfPresent());
	m_storage.SetBool(CFG_INPUTPASSDATE,dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE));
	m_storage.SetBool(CFG_INPUTBIRTH,dataSettings.IsFieldEnabled(DataSettings::BIRTH));		
	m_storage.SetBool(CFG_INPUTMOTHEREAN,dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN));
	m_storage.SetBool(CFG_INPUTFSTOWNRADR,dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_storage.SetBool(CFG_INPUTPASSNO,dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO));
	m_storage.SetBool(CFG_INPUTSEX,dataSettings.IsFieldEnabled(DataSettings::SEX));
	m_storage.SetBool(CFG_INPUTSTOCK,dataSettings.IsFieldEnabled(DataSettings::STOCK));
}

void PassportRecognizerSettingsStorage::SaveEngineSettings(const EngineSettings& engineSettings)
{
	m_storage.SetString(CFG_ENGINEPATH,engineSettings.GetEnginePath());
	m_storage.SetString(CFG_LICENSEFILEPATH,engineSettings.GetLicenseFilePath());	
	m_storage.SetString(CFG_DEVELOPERSN,engineSettings.GetDeveloperSN());
	m_storage.SetString(CFG_PROJECTID,engineSettings.GetProjectId());		
}

void PassportRecognizerSettingsStorage::SaveImageSettings(const ImageSettings& imageSettings)
{
	m_storage.SetNum(CFG_BLACKLEVEL,imageSettings.GetBlackLevel());
	m_storage.SetNum(CFG_CENTER,imageSettings.GetPaletteCenter());
	m_storage.SetNum(CFG_SIGMA,imageSettings.GetPaletteSigma());	
	m_storage.SetBool(CFG_DOPALENHANCE,imageSettings.GetDoPaletteEnhance());
}

void PassportRecognizerSettingsStorage::SaveScannerSettings(const ScannerSettings& scanerSettings)
{
	m_storage.SetNum(CFG_BRIGHTNESS,scanerSettings.GetBrightness());
	m_storage.SetNum(CFG_CONTRAST,scanerSettings.GetContrast());	
	m_storage.SetNum(CFG_RESOLUTION,scanerSettings.GetResolution());
}


void PassportRecognizerSettingsStorage::LoadDataSettings(DataSettings& dataSettings)
{
	dataSettings.SetDoUppercaseHent(m_storage.GetBool(CFG_UPPERCASEHENTDATA) == TRUE);
	dataSettings.SetDefaultStockId(m_storage.GetNum(CFG_DEFAULTSTOCKID));
	dataSettings.SetUseDefaultStock(m_storage.GetBool(CFG_USEDEFAULTSTOCK) == TRUE);
	dataSettings.SetDoNotUpdateValueIfPresent(m_storage.GetBool(CFG_NOUPDATEIFPRESENT) == TRUE);

	dataSettings.SetOptionalFieldEnabled(DataSettings::PASSPORT_DATE, m_storage.GetBool(CFG_INPUTPASSDATE) == TRUE);
	dataSettings.SetOptionalFieldEnabled(DataSettings::BIRTH, m_storage.GetBool(CFG_INPUTBIRTH) == TRUE);		
	dataSettings.SetOptionalFieldEnabled(DataSettings::MOTHER_EAN, m_storage.GetBool(CFG_INPUTMOTHEREAN) == TRUE);
	dataSettings.SetOptionalFieldEnabled(DataSettings::FIRST_OWNER, m_storage.GetBool(CFG_INPUTFSTOWNRADR) == TRUE);
	dataSettings.SetOptionalFieldEnabled(DataSettings::PASSPORT_NO, m_storage.GetBool(CFG_INPUTPASSNO) == TRUE);
	dataSettings.SetOptionalFieldEnabled(DataSettings::SEX, m_storage.GetBool(CFG_INPUTSEX) == TRUE);
	dataSettings.SetOptionalFieldEnabled(DataSettings::STOCK, m_storage.GetBool(CFG_INPUTSTOCK) == TRUE);
}

void PassportRecognizerSettingsStorage::LoadEngineSettings(EngineSettings& engineSettings)
{
	engineSettings.SetEnginePath(m_storage.GetString(CFG_ENGINEPATH));
	engineSettings.SetLicenseFilePath(m_storage.GetString(CFG_LICENSEFILEPATH));	
	engineSettings.SetDeveloperSN(m_storage.GetString(CFG_DEVELOPERSN));
	engineSettings.SetProjectId(m_storage.GetString(CFG_PROJECTID));
}

void PassportRecognizerSettingsStorage::LoadImageSettings(ImageSettings& imageSettings)
{
	imageSettings.SetBlackLevel(m_storage.GetNum(CFG_BLACKLEVEL));
	imageSettings.SetPaletteCenter(m_storage.GetNum(CFG_CENTER));
	imageSettings.SetPaletteSigma(m_storage.GetNum(CFG_SIGMA));	
	imageSettings.SetDoPaletteEnhance(m_storage.GetBool(CFG_DOPALENHANCE) == TRUE);
}

void PassportRecognizerSettingsStorage::LoadScannerSettings(ScannerSettings& scanerSettings)
{
	scanerSettings.SetBrightness(m_storage.GetNum(CFG_BRIGHTNESS));
	scanerSettings.SetContrast(m_storage.GetNum(CFG_CONTRAST));	
	scanerSettings.SetResolution(m_storage.GetNum(CFG_RESOLUTION));
}
