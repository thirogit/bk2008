#include "stdafx.h"
#include "DataSettingsPage.h"
#include "../../../classes/TempStringLoader.h"

#include "../../../global_arrays.h"
#include "../../../MainCfg.h"
#include "../../../consts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDM_SETFIELDONOFF (WM_APP+1001)

IMPLEMENT_DYNAMIC(CDataSettingsPage, CPropertyPage)

CDataSettingsPage::CDataSettingsPage(DataSettings *pDataSettings) 	: CPropertyPage(CDataSettingsPage::IDD),m_OptionalFieldsList(m_OptionalFields)
{
	m_pDataSettings = pDataSettings;

	const struct 
	{
		DataSettings::OPTIONAL_FIELDS fieldId;
		UINT nFieldNameRsrcId;		
	} 
	optionalFieldNames[] =
	{
		{DataSettings::PASSPORT_DATE,	IDS_PASSDATEFIELDNAME},
		{DataSettings::PASSPORT_NO,		IDS_PASSNOFIELDNAME},
		{DataSettings::BIRTH,			IDS_BIRTHFIELDNAME},
		{DataSettings::SEX,				IDS_SEXFIELDNAME},
		{DataSettings::STOCK,			IDS_STOCKFIELDNAME},
		{DataSettings::MOTHER_EAN,		IDS_MOTHEREANFIELDNAME},
		{DataSettings::FIRST_OWNER,		IDS_FSTOWNRADRFIELDNAME},
		{(DataSettings::OPTIONAL_FIELDS)0,0}
	};

	int i = 0;
	while(optionalFieldNames[i].nFieldNameRsrcId || optionalFieldNames[i].fieldId)
	{	
		m_OptionalFields.Add(
			new OptionalFieldsListRow(	optionalFieldNames[i].fieldId,
										CTempStringLoader(optionalFieldNames[i].nFieldNameRsrcId),
										m_pDataSettings->IsFieldEnabled(optionalFieldNames[i].fieldId)
									  )
							);
								  
		i++;
	}
}

CDataSettingsPage::~CDataSettingsPage()
{
	m_OptionalFields.FreeAndFlush();
}

void CDataSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX,IDC_UPPERCASEHENTDATA,m_bUppercaseHent);
	DDX_Control(pDX,IDC_OPTFIELDS,m_OptionalFieldsList);
	DDX_Control(pDX,IDC_DEFAULTSTOCKCOMBO,m_DefaultStockCb);
	DDX_Check(pDX,IDC_USEDEFAULTSTOCK,m_bUseDefaultStock);
	DDX_Check(pDX,IDC_NOUPDATEIFPRESENT,m_bDoNotUpdateValueIfPresent);
	
}


BEGIN_MESSAGE_MAP(CDataSettingsPage, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_OPTFIELDS, OnRClick)
	ON_COMMAND(IDM_SETFIELDONOFF,OnFieldOnOff)
	ON_BN_CLICKED(IDC_USEDEFAULTSTOCK,OnUseDefaultStockClicked)
END_MESSAGE_MAP()


BOOL CDataSettingsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bUppercaseHent = m_pDataSettings->GetDoUppercaseHent();
	m_bDoNotUpdateValueIfPresent = m_pDataSettings->GetDoNotUpdateValueIfPresent();
	m_bUseDefaultStock = m_pDataSettings->GetUseDefaultStock();

	m_DefaultStockCb.ReloadCombo();
	Stock *pDefaultStock = ary_stocks.GetStockDataByStockId(m_pDataSettings->GetDefaultStockId());
	m_DefaultStockCb.ReloadCombo(pDefaultStock);
	m_DefaultStockCb.EnableWindow(m_bUseDefaultStock);

	UpdateData(FALSE);
	return TRUE;
}

void CDataSettingsPage::OnOK()
{
	UpdateData();


	Stock *pDefaultStock = m_DefaultStockCb.GetSelItemData();

	if(pDefaultStock)
	{
		m_pDataSettings->SetDefaultStockId(pDefaultStock->GetId());
	}

	m_pDataSettings->SetDoNotUpdateValueIfPresent(m_bDoNotUpdateValueIfPresent == TRUE);
	m_pDataSettings->SetDoUppercaseHent(m_bUppercaseHent == TRUE); 	
	m_pDataSettings->SetUseDefaultStock(m_bUseDefaultStock == TRUE);

	OptionalFieldsListRow* pRow = NULL;
	OptionalFieldsArrayForList::ArrayIterator it = m_OptionalFields.iterator();
	while(it.hasNext())
	{
		pRow = *it;
		m_pDataSettings->SetOptionalFieldEnabled(pRow->GetField(), pRow->IsEnabled());
		it++;
	}

	
}

void CDataSettingsPage::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;		
		if(nItem >= 0)
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			OptionalFieldsListRow *pRow = m_OptionalFields[nItem];
			
			CMenu OnOffMenu;
			OnOffMenu.CreatePopupMenu();			
			
			CString sItemText;
			sItemText.LoadString(pRow->IsEnabled() ?  IDS_OFFMENUTEXT : IDS_ONMENUTEXT);

			OnOffMenu.InsertMenu(0, MF_BYPOSITION ,IDM_SETFIELDONOFF,sItemText);
			OnOffMenu.TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);


		}		
	}
	*pResult = 0;
}

void CDataSettingsPage::OnFieldOnOff()
{
	int iItem;
	POSITION selectedPos = m_OptionalFieldsList.GetFirstSelectedItemPosition();
	while(selectedPos)
	{
		iItem = m_OptionalFieldsList.GetNextSelectedItem(selectedPos);
		OptionalFieldsListRow* pRow = m_OptionalFields[iItem];
		pRow->SetEnabled(!pRow->IsEnabled());
	}
	m_OptionalFieldsList.Invalidate();
}
void CDataSettingsPage::OnUseDefaultStockClicked()
{
	UpdateData();
	m_DefaultStockCb.EnableWindow(m_bUseDefaultStock);
}


/////////////////////////////////////////////////////////////////////////////////////////

CDataSettingsPage::OptionalFieldsListRow::OptionalFieldsListRow(DataSettings::OPTIONAL_FIELDS field,const CString &sFieldName,bool bEnabled) :
		m_sFieldName(sFieldName),m_Field(field),m_bEnabled(bEnabled)
{
}
	
DataSettings::OPTIONAL_FIELDS CDataSettingsPage::OptionalFieldsListRow::GetField() const
{
	return m_Field;
}

const CString& CDataSettingsPage::OptionalFieldsListRow::GetFieldName() const
{
	return m_sFieldName;
}

bool CDataSettingsPage::OptionalFieldsListRow::IsEnabled() const
{
	return m_bEnabled;
}

void CDataSettingsPage::OptionalFieldsListRow::SetEnabled(bool bEnabled)
{
	m_bEnabled = bEnabled;
}


/////////////////////////////////////////////////////////////////////////////////////////	

const GRIDHEADERS CDataSettingsPage::COptionalFieldsListCtrl::szOptionalFieldsHeaderCols[] = 
{
	{IDS_HEADERFIELDNAME,NAME_COLUMN},
	{IDS_HEADERFIELDONOFF,ENABLED_COLUMN},	
	{0,0}
};


CDataSettingsPage::COptionalFieldsListCtrl::COptionalFieldsListCtrl(OptionalFieldsArrayForList & optionalFieldsArray) :
		CBaseListCtrl(szOptionalFieldsHeaderCols,CFGID_OPTFIELDLIST), m_optionalFieldsArray(optionalFieldsArray)
{
	
}


void CDataSettingsPage::COptionalFieldsListCtrl::UpdateItemCount()
{
	SetItemCount(m_optionalFieldsArray.GetSize());
}

void CDataSettingsPage::COptionalFieldsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	const char *pszText = NULL;
	
	OptionalFieldsListRow *pRow = m_optionalFieldsArray[nItem];
	switch(GetColID(nSubItem))
	{
	case NAME_COLUMN:
		pszText = pRow->GetFieldName();
	break;
	case ENABLED_COLUMN:
		pszText = g_szYesNo[pRow->IsEnabled() ? 1 : 0];
	break;	
	default:
		ASSERT(0);		
	}
	strcpy(pszItemText,pszText);

}
	
/////////////////////////////////////////////////////////////////////////////////////////