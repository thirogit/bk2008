#ifndef __EANEDIT_H__
#define __EANEDIT_H__


#include "../ctrls/CountryEdit.h"
#include "../flagsrepository/FlagsRepository.h"


class CEANEdit : public CCountryEdit
{
public:
	CEANEdit();
	BOOL IsValidEAN();
	bool SetEAN(const CString &EANValue);
	CString GetEAN() const;
protected:
	CFlagsRepository m_flagRepository;
	CString m_initialValue;
	bool	m_bIgnore;
	CRect	m_rcFlagRect;


	virtual void PreSubclassWindow() ;
	void DrawFlag();
	BOOL CheckEAN(LPCSTR EAN2Check);
	CString EscapeMaskCharacter(const CString& code2a);
	Country* ExtractCountry(const CString& EAN);

	virtual void OnCountryChange(Country *newCountry);
	virtual BOOL OnValidate();
	virtual void PrepareToolTip(CWnd *pParent);
	virtual void ExpandContextMenu(CMenu* pMenu);
	virtual BOOL OnExpandedCommand(UINT nCommand);
	
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

};

//class CEnterEANEdit : public CEANEdit
//{
//public:
//	CEnterEANEdit();
//protected:
//
//	afx_msg UINT OnGetDlgCode();
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//
//	DECLARE_MESSAGE_MAP();
//
//};


#endif