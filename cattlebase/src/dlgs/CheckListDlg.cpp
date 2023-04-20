#include "stdafx.h"
#include "CheckListDlg.h"


//IMPLEMENT_DYNAMIC(CCheckListDlg, CDialog)

CCheckListDlg::CCheckListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckListDlg::IDD, pParent)
{
}


CCheckListDlg::~CCheckListDlg()
{
	std::map<bool*,CCheckOption*>::iterator mapIt = m_optionsMap.begin();

	while(mapIt != m_optionsMap.end())
	{
		delete mapIt->second;
		mapIt++;
	}
	m_optionsMap.clear();
}

void CCheckListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_OPTIONSLIST,m_checkList);
}


BEGIN_MESSAGE_MAP(CCheckListDlg, CDialog)
	ON_BN_CLICKED(IDC_USEONLYONCE,OnUseOnlyOnce)
	ON_BN_CLICKED(IDC_SAVEANDUSE,OnSaveAndUse)
END_MESSAGE_MAP()


BOOL CCheckListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(GetDlgCaption());

	Init();

	int iItem;
	CCheckOption* pCheckOption = NULL;
	std::map<bool*,CCheckOption*>::iterator mapIt = m_optionsMap.begin();
	while(mapIt != m_optionsMap.end())
	{
		pCheckOption = mapIt->second;
		iItem = m_checkList.InsertString(-1,(LPCSTR)pCheckOption->GetOptionDesc());
		m_checkList.SetCheck(iItem,pCheckOption->GetOptionValue());
		m_checkList.SetItemData(iItem,(DWORD_PTR)pCheckOption);
		mapIt++;
	}

	return TRUE;
}

void CCheckListDlg::AddOption(const CString& sOptDesc,bool* bValue)
{
	m_optionsMap[bValue] = (new CCheckOption(sOptDesc,bValue));
}

void CCheckListDlg::SetOptions()
{
	for(int i = 0, count = m_checkList.GetCount();i < count;i++)
	{
		CCheckOption* pCheckOption = (CCheckOption*)m_checkList.GetItemData(i);
		pCheckOption->SetOptionValue(m_checkList.GetCheck(i) != 0);
	}
}

void CCheckListDlg::OnUseOnlyOnce()
{	
	UseOnlyOnce();
}

void CCheckListDlg::OnSaveAndUse()
{
	SaveAndUse();
}