#ifndef __OCRPASSDLG_H_
#define __OCRPASSDLG_H_

#include "../PassportRecognizer.h"
#include "xedit.h"
#include "DataTypes.h"
#include "../../classes/types/cowsex.h"
#include "../../combo/SexCombo.h"
#include "../../ctrls/CowListCtrl.h"
#include "OCRPassImage.h"
#include "twain/TwainPP.h"
#include "twain/TraceFile.h"
#include "imgbutton/ImageButton.h"
#include "../../combo/StockCombo.h"
#include "../../../passOcrProfile/include/PassProfilePassProfiles.h"
#include "InputField.h"
#include "../PassportRecognitionResult.h"
#include "../PassportRecognizer.h"
#include "../engine/RecognitionEngine.h"
#include "../../PassportCommon/CowDataPassImgMap.h"
#include "../configuration/PassportRecognizerSettings.h"


class COCRPassDlg : public CDialog,public CTwainPP::ITwainResult
{
public:
	COCRPassDlg(CCowArray &outArray,
				CCowArray &sesArray,
				CCowDataPassImgMap &passMap,
				PassportRecognizerSettings* pSettings,
				CWnd* pParent = NULL);

	~COCRPassDlg();

	enum { IDD = IDD_OCRPASS_DIALOG };
protected:
	PassportRecognizer *m_pPassRecognizer;
	RecognitionEngine* m_pRecognitionEngine;

	CImageButton  m_DoScanBtn;
	CImageButton  m_AddCowBtn;
   
	CImageButton  m_ScanEndBtn;
	CImageButton  m_ORCAgain;

	CInputFieldCtrl<CXEdit> m_passDate;
	CInputFieldCtrl<CXEdit> m_passNo;
	CInputFieldCtrl<CXEdit> m_farmNo;
	CInputFieldCtrl<CXEdit> m_cowNo;
	CInputFieldCtrl<CXEdit> m_birthDate;
	CInputFieldCtrl<CXEdit> m_birthPlace;	
	CInputFieldCtrl<CXEdit> m_motherNo;
	CInputFieldCtrl<CSexCombo> m_cowSex;
	CInputFieldCtrl<CXEdit> m_hentName;
	CInputFieldCtrl<CXEdit> m_hentStreet;
	CInputFieldCtrl<CXEdit> m_hentPobox;
	CInputFieldCtrl<CXEdit> m_hentCity;
	CInputFieldCtrl<CXEdit> m_hentZip;
	CInputFieldCtrl<CStockCombo> m_stockCombo;
	
	CListBox m_passProfiles;
	CTabCtrl m_showTab;
	CInCowListCtrl m_scanList;
	COCRPassImage m_passShow;
	int m_iSelectedTab;

	PtrFlushArray<CInputField> m_inputCtrls;
	PtrFlushArray<CWnd> m_hentSection;

	CCowArray &m_outArray;
	CCowDataPassImgMap &m_passMap;
	CCowArray &m_sesArray;

	Hent *m_foundHent;
	Stock *m_foundStock;
	CString m_cowStock;

	PassProfilePassProfiles m_passProfilesXml;
	
	fipWinImageEx m_passportImg;
	boost::shared_ptr<const PassportRecognitionContext> m_ptrRecognitionContext;
	
	CTwainPP		m_TwainPP;
	CTraceFile		m_TraceFile;
	BOOL            m_bNoSource;
	PassportRecognizerSettings* m_pSettings;

	int ParseLines(CStringList &output,const CString &input);
	BOOL ParseHent(const CStringList &hentAddrList);
	void EnableControls(BOOL bEnable);

	void EnableHentSection(BOOL bEnable);
	void EnableRepeatButton(bool bEnable);
	void EnableAddCowButton(bool bEnable);

	void SetPassportImageFromRecognitionContext(boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext,const PassProfilePassProfile* pPassProfile);
	void ProcessRecognitionResult(const PassportRecognitionResult& recognitionResult);
	void SavePassImg(Cow *pCow);
	BOOL ErrorOcurred(const CString &errorText);
	BOOL ValidateEAN(LPCSTR szEAN);
	virtual bool Look4Duplicates(Cow *cowdata);
	bool ValidatePassNo(CString &passNo);
	CString RepairName(const CString &inputName);
	CString RepairDate(const CString &date);
	char Digit2AlphaReplace(char inputChar);
	CString RemoveWhitespace(const CString &inputNumber);
	CString RepairEAN(const CString &inputNumber);
	CString RepairPassNo(const CString &inputNumber);
	void ReloadStockCombo(bool bAddStockStr);
	virtual bool ValidateDataFields();
	Cow* CreateCowFromFields();
	virtual bool DoManualInput();
	
	virtual  bool EditCow(Cow *pCow);
	
	void ClearInvalid();
	void SetHentFields(Hent* pHent);
	void ClearFields();
	void ClearFirstOwnerFields(bool bClearFarmNo);
	void ProcessCowSex(const CString& sOCRCowSex);
	void ProcessCowEAN(const CString& sOCRCowEANText,const CString& sOCRCowEANBC);
	void ProcessMotherEAN(const CString& sOCRMotherEAN);
	void ProcessPassDate(const CString& sOCRPassDate);
	void ProcessPassNo(const CString& sOCRPassNo);
	void ProcessBirthDatePlace(const CString& sOCRBirthDatePlace);
	void ProcessCowStock(const CString& sOCRCowStock);
	void ProcessFirstOwner(const CString& sOCRFirstOwnerFarmNo,const CString& sOCRFirstOwnerNameAddress);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnTwainImageInfo(int nSizeX, int nSizeY);
	virtual BOOL OnTwainBitmap(HDIB hDIB);
	virtual void OnTwainError(CTwainPP::ETWainError eTwainError);


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnDoScan();
	afx_msg void OnAddCow();
	
	afx_msg void OnAddStock();
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//afx_msg void OnBnClickedOptions();
	afx_msg void OnBnClickedAddHent();
	afx_msg void OnDestroy();
	afx_msg void OnStockCbnChange();
	afx_msg void OnBnClickedScanningFinished();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOCRAgain();
	
	DECLARE_MESSAGE_MAP()
};
#endif