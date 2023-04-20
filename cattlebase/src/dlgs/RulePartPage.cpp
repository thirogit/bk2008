#include "stdafx.h"
#include "RulePartPage.h"
#include "../missing/util.h"
#include "../consts.h"


//
//void DDX_FileEditCtrl(CDataExchange *pDX, int nIDC, CString& rStr, DWORD dwFlags)
//{
//    CWnd *pWnd = pDX->m_pDlgWnd->GetDlgItem(nIDC);
//    ASSERT(pWnd);
//    if (pDX->m_bSaveAndValidate)                // update string with text from control
//    {
//        // ensure the control is a CFileEditCtrl control
//        ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CFileEditCtrl)));
//        // copy the first file listed in the control to the string
//        rStr.Empty();
//        CFileEditCtrl *pFEC = (CFileEditCtrl *)pWnd;
//        POSITION pos = pFEC->GetStartPosition();
//        if (pos)
//            rStr = pFEC->GetNextPathName(pos);
//    }
//    else                                        // create the control if it is not already created
//    {                                           // set the control text to the text in string
//        CFileEditCtrl *pFEC = NULL;
//        if (!pWnd->IsKindOf(RUNTIME_CLASS(CFileEditCtrl)))    // not subclassed yet
//        {
//            // create then subclass the control to the edit control with the ID nIDC
//            HWND hWnd = pDX->PrepareEditCtrl(nIDC);
//            pFEC = new CFileEditCtrl(TRUE);     // create the control with autodelete
//            if (!pFEC)                          // failed to create control
//            {
//                ASSERT(FALSE);
//                AfxThrowNotSupportedException();
//            }
//            if (!pFEC->SubclassWindow(hWnd))
//            {                                   // failed to subclass the edit control
//                delete pFEC;
//                ASSERT(FALSE);
//                AfxThrowNotSupportedException();
//            }
//            // call CFileEditCtrl::SetFlags() to initialize the internal data structures
//            dwFlags &= ~FEC_MULTIPLE;           // can not put multiple files in one CString
//            if (!pFEC->SetFlags(dwFlags))
//            {
//                delete pFEC;
//                ASSERT(FALSE);
//                AfxThrowNotSupportedException();
//            }
//        }
//        else                                    // control already exists
//            pFEC = (CFileEditCtrl *)pWnd;
//        if (pFEC)
//            pFEC->SetWindowText(rStr);          // set the control text
//    }
//}

IMPLEMENT_DYNAMIC(CRulePartPage, CPropertyPage)
CRulePartPage::CRulePartPage(UINT nIDCaption)
	: CPropertyPage(CRulePartPage::IDD,nIDCaption),m_REsigned(g_szRegExSigned),m_REunsigned(g_szRegExUnsigned),
								m_RateBelowWeightXY(&m_REunsigned),m_RateAboveWeightXY(&m_REunsigned),
								m_RateBelowWeightXX(&m_REunsigned),m_RateAboveWeightXX(&m_REunsigned),
                                m_LimWeight(0),	m_pUseOnInit(NULL)
{

	
}

CRulePartPage::~CRulePartPage()
{
	if(m_pUseOnInit)
		delete m_pUseOnInit;
}

void CRulePartPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_LIMWEIGHT,m_LimWeight);
	DDX_Control(pDX,IDC_XYPRICEBELOW,m_RateBelowWeightXY);
	DDX_Control(pDX,IDC_XYPRICEOVER,m_RateAboveWeightXY);
	DDX_Control(pDX,IDC_XXPRICEBELOW,m_RateBelowWeightXX);
	DDX_Control(pDX,IDC_XXPRICEOVER,m_RateAboveWeightXX);
}

BEGIN_MESSAGE_MAP(CRulePartPage, CPropertyPage)
		ON_BN_CLICKED(IDC_SAMEASXY,OnSameAsXY)	
END_MESSAGE_MAP()

void CRulePartPage::InitWith(const RulePart* pUseOnInit)
{
	m_pUseOnInit = new RulePart();
	m_pUseOnInit->CopyFrom(*pUseOnInit);

	if(m_hWnd)
		SetControls(m_pUseOnInit);
}

BOOL CRulePartPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	if(m_pUseOnInit)
	{
		SetControls(m_pUseOnInit);		
	}
	return TRUE;
}

void CRulePartPage::SetControls(const RulePart* pRulePart)
{
	SetDlgItemInt(IDC_LIMWEIGHT,pRulePart->GetLimWeight());		
	m_RateBelowWeightXY.SetWindowText(pRulePart->GetPriceUnderWeightXY().ToString());
	m_RateAboveWeightXY.SetWindowText(pRulePart->GetPriceOverWeightXY().ToString());
	m_RateBelowWeightXX.SetWindowText(pRulePart->GetPriceUnderWeightXX().ToString());
	m_RateAboveWeightXX.SetWindowText(pRulePart->GetPriceOverWeightXX().ToString());
}

void CRulePartPage::OnSameAsXY()
{
	CString sText;
	m_RateBelowWeightXY.GetWindowText(sText);
	m_RateBelowWeightXX.SetWindowText(sText);

	m_RateAboveWeightXY.GetWindowText(sText);
	m_RateAboveWeightXX.SetWindowText(sText);	
}

bool CRulePartPage::ValidateFields(bool bVerbose)
{
	if(m_LimWeight == 0)
	{
		if(bVerbose)
		{
			AfxMessageBox(IDS_ZEROLIMWEIGHT,MB_OK,0);
			GetDlgItem(IDC_LIMWEIGHT)->SetFocus();
		}
		return false;
	}

	return	ValidatePriceEdit(m_RateBelowWeightXY,bVerbose) &&
			ValidatePriceEdit(m_RateAboveWeightXY,bVerbose) &&
			ValidatePriceEdit(m_RateBelowWeightXX,bVerbose) &&
			ValidatePriceEdit(m_RateAboveWeightXX,bVerbose);	

}

bool CRulePartPage::ValidatePriceEdit(CEdit& priceEdit,bool bVerbose)
{
	if(priceEdit.GetWindowTextLength() == 0)
	{
		if(bVerbose)
		{
			AfxMessageBox(IDS_EMPTYPRICE);
			priceEdit.SetFocus();
		}
		return false;
	}
	return true;
}

Money CRulePartPage::GetPrice(CEdit& priceEdit)
{
	CString sPrice;
	priceEdit.GetWindowText(sPrice);
	return Money(CUtil::myatof(sPrice));
}

void CRulePartPage::UpdateRulePart(RulePart* rulePart)
{
	rulePart->SetLimWeight(m_LimWeight);
	rulePart->SetPriceOverWeightXY(GetPrice(m_RateAboveWeightXY));
	rulePart->SetPriceUnderWeightXY(GetPrice(m_RateBelowWeightXY));
	rulePart->SetPriceOverWeightXX(GetPrice(m_RateAboveWeightXX));
	rulePart->SetPriceUnderWeightXX(GetPrice(m_RateBelowWeightXX));	
}


