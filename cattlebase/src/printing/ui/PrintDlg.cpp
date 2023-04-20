#include "stdafx.h"
#include "PrintDlg.h"
#include "MainCfg.h"
#include "../../ctrls/DocListCtrl.h"
#include "../../ctrls/InvListCtrl.h"
#include "../../ctrls/CowListCtrl.h"
#include "../../ctrls/ExpensesListCtrl.h"
#include "../../ctrls/HentsListCtrl.h"
#include "PrintOptionsDlg.h"
#include "../../classes/TempStringLoader.h"
#include "../../datatypes/DocDataComparator.h"
#include "../../datatypes/InvoiceDataComparator.h"
#include "../../datatypes/ExpenseDataComparator.h"
#include "../../datatypes/HentComparator.h"
#include "../../exceptions/StrException.h"
#include "../../classes/TempStringLoader.h"
#include "../../printing/BaseDocView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintDlg dialog

GRIDHEADERS CPrintDlg::PrintListCowHeader[] = 
{ 
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADERBIRTHDATE,COWS_SORT_BY_BIRTHDATE},
	{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
	{IDS_HEADERSEX,COWS_SORT_BY_SEX},
	{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
	{IDS_HEADERDELIVER,COWS_SORT_BY_DELIVER},
	{0,0}
	
};

GRIDHEADERS CPrintDlg::PrintListInDocHeader[] =
{
	{IDS_HEADERDOCNO,DOCS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,DOCS_SORT_BY_DOCDATE},
	{IDS_HEADERDELIVER,DOCS_SORT_BY_HENT},
	{IDS_HEADERINDATE,DOCS_SORT_BY_LOADDATE},
	{IDS_HEADERPLATENO,DOCS_SORT_BY_PLATENO},
	{0,0}
}; 

GRIDHEADERS CPrintDlg::PrintListOutDocHeader[] =
{
	{IDS_HEADERDOCNO,DOCS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,DOCS_SORT_BY_DOCDATE},
	{IDS_HEADERBUYER,DOCS_SORT_BY_HENT},
	{IDS_HEADEROUTDATE,DOCS_SORT_BY_LOADDATE},
	{IDS_HEADERPLATENO,DOCS_SORT_BY_PLATENO},
	{0,0}
};

GRIDHEADERS CPrintDlg::PrintListInvoiceHeader[] =
{
	{IDS_HEADERDOCNO,INVS_SORT_BY_NUMBER},
	{IDS_HEADERDOCDATE,INVS_SORT_BY_INVDATE},	
	{IDS_HEADERHENT,INVS_SORT_BY_HENT},
	{IDS_HEADERAMOUNT,INVS_SORT_BY_QTY},
	{IDS_HEADERVAT,INVS_SORT_BY_VAT},
	{IDS_HEADERBRUTTO,INVS_SORT_BY_GROSSVALUE},
	{IDS_HEADERCUSTNO,INVS_SORT_BY_CUSTOMNO},
	{IDS_HEADERCORRECT,INVS_SORT_BY_CORRECT},
	{0,0}
};

GRIDHEADERS CPrintDlg::PrintListExpensesHeader[] =
{
	{IDS_HEADEREXPENSETITLE,	EXPENSEDATA_EXPENSETITLE},
	{IDS_HEADEREXPENSECATEGORY,	EXPENSEDATA_EXPENSECATEGORY},
	{IDS_HEADERBRUTTO,			EXPENSEDATA_GROSVALUE},
	{IDS_HEADERVAT,				EXPENSEDATA_VATRATE},
	{IDS_HEADEREXPENSETYPE,		EXPENSEDATA_EXPENSETYPE},	
	{IDS_HEADERINVOICE,			EXPENSEDATA_INVOICENO},
	{IDS_HEADERDOCDATE,			EXPENSEDATA_EXPENSEDATE},
	{0,0}
};

GRIDHEADERS CPrintDlg::PrintListHentsHeader[] =
{
	{IDS_HEADERNAME,	HENTS_SORT_BY_NAME},
	{IDS_HEADERARIMRNO,	HENTS_SORT_BY_ARIMRNO},
	{IDS_HEADERALIAS,	HENTS_SORT_BY_ALIAS},
	{IDS_HEADERSTREET,	HENTS_SORT_BY_STREET},
	{IDS_HEADERPOBOX,	HENTS_SORT_BY_POBOX},
	{IDS_HEADERCITY,	HENTS_SORT_BY_CITY},
	{IDS_HEADERZIP,		HENTS_SORT_BY_ZIP},
	{IDS_HEADERHENTTYPE,HENTS_SORT_BY_HENTTYPE},
	{0,0}
};



CPrintDlg::CPrintDlg(CRawDataArray &DataArr,int DocId,CWnd* pParent /*=NULL*/) : CDialog(CPrintDlg::IDD, pParent)
{

	m_pTemplate=NULL;	
	m_pDoc = NULL;
	
	m_pList = NULL;
	m_DataArray = NULL;

	m_PRINTDOC = FindPRINTDOC(DocId);
	ASSERT(m_PRINTDOC);
	
	CreateList(m_PRINTDOC->iDataType);

	m_DataArray->Append(DataArr);
}


const PRINTDOCS* CPrintDlg::FindPRINTDOC(UINT nDocId)
{
	const PRINTDOCS *pPrintDoc = g_PrintDocs;
	while(pPrintDoc->nID != NULL_DOC_ID)
	{		
		if(nDocId == pPrintDoc->nID) return pPrintDoc;
		pPrintDoc++;
	}
	return NULL;
}


CPrintDlg::~CPrintDlg()
{	
	if(m_pDoc) delete m_pDoc;
	if(m_pList) delete m_pList;
	if(m_DataArray) delete m_DataArray;
}

void CPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
		
	DDX_Control(pDX,IDC_GOPRINT,m_GoPrint);
	DDX_Control(pDX,IDC_PRINTOPT,m_PrintOpt);
	DDX_Control(pDX,IDC_PRINTDELETE,m_Delete);
	DDX_Control(pDX,IDCANCEL,m_Cancel);
	DDX_Control(pDX,IDC_PRINTDOCTYPE,m_PrintDocCombo);
	DDX_Control(pDX,IDC_PRINTPREVIEW,m_Preview);
	
	
}


BEGIN_MESSAGE_MAP(CPrintDlg, CDialog)
		
	ON_BN_CLICKED(IDC_GOPRINT, OnPrint)
	ON_BN_CLICKED(IDC_PRINTOPT, OnPrintOpt)
	ON_BN_CLICKED(IDC_PRINTDELETE,OnPrintDelete)
	ON_BN_CLICKED(IDC_PREVIEW, OnPrintPreview)

	ON_BN_CLICKED(IDC_COPIES_2, OnPrintTwoCopies)
	ON_BN_CLICKED(IDC_COPIES_3, OnPrintThreeCopies)
	ON_BN_CLICKED(IDC_COPIES_4, OnPrintFourCopies)


	
	
	
END_MESSAGE_MAP()





BOOL CPrintDlg::OnInitDialog()
{
	
	const PRINTDOCS *pDoc = NULL;
	int item,selItem = -1;	
	

	CDialog::OnInitDialog();
	m_GoPrint.SetButtonImage(IDB_PRINTER);
	m_PrintOpt.SetButtonImage(IDB_PRINTOPT);
	m_Delete.SetButtonImage(IDB_TRASH);
	m_Preview.SetButtonImage(IDB_PICTURE);
	m_Cancel.SetButtonImage(IDB_CANCEL);


	m_pList->SubclassWindow(GetDlgItem(IDC_PRINTLIST)->GetSafeHwnd());

	pDoc = g_PrintDocs;

	while(pDoc->nID != NULL_DOC_ID)
	{
		if(m_PRINTDOC->iDataType == pDoc->iDataType)
		{
			item = m_PrintDocCombo.AddString(CTempStringLoader(pDoc->nResDocName));
			if(m_PRINTDOC->nID == pDoc->nID) 
				selItem = item;
			else 
				if(item <= selItem) 
					selItem++;	
			m_PrintDocCombo.SetItemData(item,(DWORD_PTR)pDoc);
		}
		pDoc++;
	}

	m_PrintDocCombo.SetCurSel(selItem);

	return TRUE;
}
void CPrintDlg::CreateList(UINT nDataType)
{

	CInvoiceDataArray *pInvsArray = NULL;
	CCowArray *pCowsArray = NULL;
	CDocDataArray *pDocsArray = NULL;
	CExpenseDataArray *pExpensesArray = NULL;
	CHentArray *pHentsArray = NULL;

	
	switch(nDataType)
	{
	case DATATYPE_COWS:
		pCowsArray = new CCowArray;
		m_pList = new CCowListCtrl(pCowsArray,PrintListCowHeader,CFGID_PLIST);
		m_DataArray = pCowsArray;
	break;
	case DATATYPE_INDOCS:
		pDocsArray = new CDocDataArray;
		m_pList = new CDocListCtrl(pDocsArray,PrintListInDocHeader,CFGID_PINLIST);
		m_DataArray = pDocsArray;
	break;
	case DATATYPE_OUTDOCS:
		pDocsArray = new CDocDataArray;
		m_pList = new CDocListCtrl(pDocsArray,PrintListOutDocHeader,CFGID_POUTLIST);
		m_DataArray = pDocsArray;
	break;	
	case DATATYPE_RRINVS:
	case DATATYPE_BUYINVS:
	case DATATYPE_SELLINVS:
	case DATATYPE_INVS:
	case DATATYPE_VATINVS:
	case DATATYPE_BUYVATINVS:
	case DATATYPE_SELLVATINVS:
		pInvsArray = new CInvoiceDataArray;
		m_pList = new CInvListCtrl(pInvsArray,PrintListInvoiceHeader,CFGID_PINVLIST);
		m_DataArray = pInvsArray;
	break;
	case DATATYPE_EXPENSES:
		pExpensesArray = new CExpenseDataArray;
		m_pList = new CExpensesListCtrl(pExpensesArray,PrintListExpensesHeader,CFGID_PEXPLIST);
		m_DataArray = pExpensesArray;
	break;
	case DATATYPE_HENTS:
		pHentsArray = new CHentArray;
		m_pList = new CHentsListCtrl(pHentsArray,PrintListHentsHeader,CFGID_PHENTLIST);
		m_DataArray = pHentsArray;
		break;
	default:
		ASSERT(0);
		break;
	}

}

void CPrintDlg::OnPrintPreview()
{
	PrintDocument(true,0);
}
void CPrintDlg::OnPrint()
{
	PrintDocument(false,1);
}
void CPrintDlg::OnPrintTwoCopies()
{
	PrintDocument(false,2);
}
void CPrintDlg::OnPrintThreeCopies()
{
	PrintDocument(false,3);
}
void CPrintDlg::OnPrintFourCopies()
{
	PrintDocument(false,4);
}


void CPrintDlg::PrintDocument(bool bPreview,int nCopies)
{
	int iSel;
	
	CRuntimeClass *pDocClass = NULL;

	UpdateData();

	if(!HAS_PRIVLEDGE(PRINTDOC))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	if((iSel = m_PrintDocCombo.GetCurSel()) == -1)
	{
		AfxMessageBox(IDS_ERRSELDOCTYPE,MB_OK,0);
		return;
	}

	if(!m_DataArray->GetSize()) 
	{
		AfxMessageBox(IDS_ERRNOPRINTDATA,MB_OK,0);
		return;
	}

	const PRINTDOCS *pDoc = (PRINTDOCS*)m_PrintDocCombo.GetItemData(iSel);
	if(m_PRINTDOC != pDoc || m_pDoc == NULL)
	{
		if(m_pDoc) delete m_pDoc;
		m_PRINTDOC = pDoc;
		
		pDocClass = pDoc->pDocClass;
   		ASSERT(pDocClass->IsDerivedFrom(RUNTIME_CLASS(CCowBasePrintDoc))); 
		m_pDoc = (CCowBasePrintDoc*)pDocClass->CreateObject();

	

		try
		{
			m_pDoc->CreateDocument(pDoc);
		}
		catch(CStrException *e)
		{
			AfxMessageBox((LPCSTR)e->GetError());
			e->Delete();
			delete m_pDoc;
			m_pDoc = NULL;
			return;
		}
	} 

		
	switch(m_pDoc->Load(*m_DataArray))
	{
		case RELOAD_CAN_CONTINUE:
			if(AfxMessageBox(IDS_ERRRELOADCONTINUE,MB_YESNO,0) != IDYES)
				return;
		case RELOAD_NO_CONTINUE:
			AfxMessageBox(IDS_ERRRELOADNOCONTINUE,MB_OK,0);
			return;
	}

	if (!m_pTemplate)
	{
		m_pTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			m_pDoc->GetRuntimeClass(),
			RUNTIME_CLASS(CFrameWnd),
			pDoc->pDocViewClass);
		AfxGetApp()->AddDocTemplate(m_pTemplate);
	}

	m_pDoc->m_bAutoDelete = FALSE; 

	CFrameWnd * pFrameWnd = m_pTemplate->CreateNewFrame( m_pDoc, NULL );
	m_pTemplate->InitialUpdateFrame(pFrameWnd, NULL);
	CBaseDocView* pView = (CBaseDocView*)pFrameWnd->GetActiveView();


	//m_pDoc->SetInForm(pDoc->bCanInForm && m_bPrintInForm);
		

	if(bPreview)
	{
		CFrameWnd* pOldFrame=(CFrameWnd*)AfxGetThread()->m_pMainWnd;
		pOldFrame->ShowWindow(SW_HIDE); 
		pView->m_pOldFrame = pOldFrame;
		pView->m_pCtrlDlg = this;
		AfxGetApp()->m_pMainWnd = pFrameWnd;

		pFrameWnd->SetWindowText((LPCSTR)CTempStringLoader(IDS_PRINTPREVIEW));
		
		pFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pView->PrintPreview();
		
	}
	else 
	{
		pFrameWnd->ShowWindow(SW_HIDE); 

		


		pView->Print(nCopies);
		delete pView;
		delete pFrameWnd;
	}


}

void CPrintDlg::OnPrintOpt()
{
	int iSel = m_PrintDocCombo.GetCurSel();
	if(iSel >= 0)
	{
		PRINTDOCS * const pDoc = (PRINTDOCS* const) m_PrintDocCombo.GetItemData(iSel);
		CPrintDocOptDlg printdocopt(pDoc,this);
		printdocopt.DoModal();
	}
}



void CPrintDlg::OnPrintDelete()
{
	m_pList->Lock();
	POSITION pos = m_pList->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_DataArray->RemoveAt(m_pList->GetNextSelectedItem(pos));
	}
	m_pList->UpdateItemCount();
	m_pList->Unlock();
}




