#ifndef __BASEHENTDLG_H__
#define __BASEHENTDLG_H__

#include "maskededit/OXMaskedEdit.h"
#include "imgbutton/ImageButton.h"
#include "ctrls/CountryCombo.h"
#include "ctrls/ExtrasMemo.h"
#include "combo/HentTypeCombo.h"
#include "../FilterEdit/FilterEdit.h"
#include "../ctrls/AccountNoEdit.h"
#include "../ctrls/curdateedit/Curdateedit.h"
#include "../ctrls/FarmNoEdit.h"
#include "../ctrls/LongitudeEdit.h"
#include "../ctrls/LatitudeEdit.h"

class CBaseHentDlg : public CDialog
{
public:
	CBaseHentDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_HENT };
protected:
		CFarmNoEdit		m_FarmNo;
		COXMaskedEdit	m_NIP;
		COXMaskedEdit	m_WetNo;
		COXMaskedEdit	m_ZipCode;
		CExtrasMemo		m_extras;
		CCountryCombo	m_Countries;
		CString			m_plate;
		CString			m_name;
		CString			m_alias;
		CString			m_phone;
		CString			m_pobox;
		CString			m_city;
		CString			m_street;
		CHentTypeCombo	m_htype;

		COXMaskedEdit	m_pesel;
		CFilterEdit		m_regon;
		CFilterEdit		m_idno;
		CCurDateEdit	m_issuedate;
		CString			m_issuepost;
		CAccountNoEdit	m_accountno;
		CString			m_bankname;
	
		CImageButton	m_HentAdd;
		CImageButton	m_HentCancel;


		CString			m_emailaddress;
		CString			m_cellphoneno;
		CString			m_wetlicenceno;
		CLongitudeEdit	m_Longitude;
		CLatitudeEdit	m_Latitude;
		

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog() ;
	virtual void  OnOK();
	virtual void SetupDialogControls();
	virtual void InitDialogControls();
	bool ValidateFields();

	virtual CString GetWindowCaption() = 0;
	virtual bool CommitHent() = 0;
	Hent* CreateHent();

protected:

	afx_msg void OnCountryChange();
	afx_msg void OnAliasFocus();
	afx_msg void OnAliasKillFocus();

	DECLARE_MESSAGE_MAP()
};

#endif