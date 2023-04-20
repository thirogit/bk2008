#include "stdafx.h"
#include "FindDocBaseDlg.h"

BEGIN_MESSAGE_MAP(CFindDocBaseDlg, CFindBaseDlg)
	ON_BN_CLICKED(IDC_BYDOCDATE, OnCheckByDate)
	ON_BN_CLICKED(IDC_BYLOADDATE, OnCheckByDate)
END_MESSAGE_MAP()

CFindDocBaseDlg::CFindDocBaseDlg(const CString& sCaption) : CFindBaseDlg(CFindDocBaseDlg::IDD),m_dochent(false)
{
	m_bByLoadDate = FALSE;
	m_bByDocDate = FALSE;
	
	m_strCaption = sCaption;
	m_psp.pszTitle = m_strCaption;
	m_psp.dwFlags |= PSP_USETITLE;

}

void CFindDocBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindBaseDlg::DoDataExchange(pDX);
	
	DDX_Control (pDX, IDC_DOCDATE  , m_docdatespan ) ;
	DDX_Control (pDX, IDC_LOADDATE ,m_loaddatespan  ) ;
	DDX_Check(pDX,IDC_BYDOCDATE,m_bByDocDate);
	DDX_Check(pDX,IDC_BYLOADDATE,m_bByLoadDate);


	DDX_Text(pDX, IDC_DOCNOFROM, m_docnofrom);
	DDX_Text(pDX, IDC_DOCNOTO, m_docnoto);

	DDX_Control(pDX, IDC_DOCHENT, m_dochent);
	DDX_Text(pDX, IDC_DOCPLATENO, m_docplateno);

	DDX_Control(pDX, IDC_BYAMOUNT, m_amount);
	DDX_Control(pDX,IDC_BYREASON,m_reason);
}


BOOL CFindDocBaseDlg::OnInitDialog()
{
	CFindBaseDlg::OnInitDialog();
	OnCheckByDate();
	m_dochent.InitFindCombo();

	InitControls();

	UpdateData();

	return TRUE;
}




void CFindDocBaseDlg::OnCheckByDate()
{
	UpdateData();
	m_docdatespan.EnableWindow(m_bByDocDate);
	m_loaddatespan.EnableWindow(m_bByLoadDate);
}




int CFindDocBaseDlg::GoSearch(CFindSetup& setup)
{
	
	UpdateData();

	CDocDataMatcher docMatcher;
		
	int DocNoFrom = 0,DocNoTo = INT_MAX;
	bool bByDocNo = false;
	if(m_docnofrom.GetLength() > 0)
	{
		DocNoFrom = atoi(m_docnofrom);
		bByDocNo = true;
	}
	if(m_docnoto.GetLength() > 0)
	{
		DocNoTo = atoi(m_docnoto);
		bByDocNo = true;
	}

	if(bByDocNo)
		docMatcher.FindDocNo(DocNoFrom,DocNoTo);

	if(!m_dochent.IsNeutral())
		docMatcher.FindHent(m_dochent.GetSelItemData());
	
	if(m_docplateno.GetLength() > 0)
		docMatcher.FindPlateNo(m_docplateno);

	if(!m_reason.IsNeutral())
		docMatcher.FindReason(m_reason.GetSelReason());

	if(m_amount.HasValue())
	{
		int amount = m_amount.GetValue();
		CMP_ACTION_TYPE amount_span = m_amount.GetRangeType();
		docMatcher.FindAmount(amount,amount_span);
	}

	if(m_bByLoadDate) 
	{
		CDateTime load_t1,load_t2;
		TIMESPAN load_span_type = m_loaddatespan.GetSpan(load_t1,load_t2);
		docMatcher.FindLoadDate(load_t1,load_t2,load_span_type);
		
	}
	if(m_bByDocDate) 
	{
		CDateTime doc_t1,doc_t2;
		TIMESPAN doc_span_type = m_docdatespan.GetSpan(doc_t1,doc_t2);
		docMatcher.FindDocDate(doc_t1,doc_t2,doc_span_type);
	}

	if(setup.ByCreationTime())
	{
		CDateTime time1,time2;
		TIMESPAN creationSearchSpan = setup.CreationTimeSpan(time1,time2);
		docMatcher.FindCreationTime(time1,time2,creationSearchSpan);
	}
	
	if(setup.ByLastModificationTime())
	{
		CDateTime time1,time2;
		TIMESPAN lastModificationSearchSpan = setup.LastModificationTimeSpan(time1,time2);
		docMatcher.FindLastModificationTime(time1,time2,lastModificationSearchSpan);
	}

	UserData *pCreator = setup.SelectedCreator();
	if(pCreator)
		docMatcher.FindCreator(pCreator);
		
	UserData* pLastModifier = setup.SelectedLastModifier();
	if(pLastModifier)
		docMatcher.FindLastModifier(pLastModifier);
	
	if(setup.GetInResult())
		docMatcher.FindMarked();

	if(setup.ByExtras())
		docMatcher.FindExtras(setup.GetExtras());
	
	CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc> findDoc(setup.GetAddToResult(),&docMatcher);


	SearchDoc(findDoc,setup);
	
	return findDoc.GetFoundCount();	
		
}
