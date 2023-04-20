#ifndef __DATASETTINGSPAGE_H__
#define __DATASETTINGSPAGE_H__

#include "../DataSettings.h"
#include "../../../PtrFlushArray.h"
#include "../../../ctrls/BaseListCtrl.h"
#include "../../../combo/StockCombo.h"

class CDataSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataSettingsPage)
private:
	
	class OptionalFieldsListRow
	{
	public:
		OptionalFieldsListRow(DataSettings::OPTIONAL_FIELDS field,const CString &sFieldName,bool bEnabled);
		
		DataSettings::OPTIONAL_FIELDS GetField() const;
		const CString& GetFieldName() const;
		bool IsEnabled() const;
		void SetEnabled(bool bEnabled);
	private:
		DataSettings::OPTIONAL_FIELDS m_Field;
		CString m_sFieldName;
		bool m_bEnabled;
	};

	typedef PtrFlushArray<OptionalFieldsListRow> OptionalFieldsArrayForList;

	class COptionalFieldsListCtrl : public CBaseListCtrl
	{
	public:
		COptionalFieldsListCtrl(OptionalFieldsArrayForList & optionalFieldsArray);
		void UpdateItemCount();

		
	protected:
		enum HEADER_COLS
		{
			NAME_COLUMN = 1,
			ENABLED_COLUMN = 2
		};
		static const GRIDHEADERS szOptionalFieldsHeaderCols[]; 

		OptionalFieldsArrayForList &m_optionalFieldsArray;
	protected:
		void GetItemDispText(int nItem,int nSubItem,char *pszItemText);

	};


public:
	CDataSettingsPage(DataSettings *pDataSettings);
	virtual ~CDataSettingsPage();

	enum { IDD = IDD_RECOGNITIONDATAOPTS };

protected:
	BOOL				m_bUppercaseHent;
	BOOL				m_bDoNotUpdateValueIfPresent;
	
	COptionalFieldsListCtrl			m_OptionalFieldsList;
	OptionalFieldsArrayForList		m_OptionalFields;

	CStockCombo			m_DefaultStockCb;
	BOOL				m_bUseDefaultStock;

	DataSettings*		m_pDataSettings;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldOnOff();
	afx_msg void OnUseDefaultStockClicked();

	DECLARE_MESSAGE_MAP()

};

#endif