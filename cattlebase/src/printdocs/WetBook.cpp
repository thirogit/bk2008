#include "stdafx.h"
#include "WetBook.h"
#include "PrintDocsCfg.h"
#include "../consts.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

IMPLEMENT_DYNCREATE(CWetBookOptDlg, CCustomPrintDocOptDlg)
CWetBookOptDlg::CWetBookOptDlg (CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent) 
{
	m_LP = PrintCfg().GetNum(CFGID_WETBOOK_LP);
}
void CWetBookOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_LPEDIT,m_LP);				
}
void CWetBookOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetNum(CFGID_WETBOOK_LP,m_LP);
}

BEGIN_MESSAGE_MAP(CWetBookOptDlg, CCustomPrintDocOptDlg)
	
END_MESSAGE_MAP()
//#######################################################################################

IMPLEMENT_DYNCREATE(CWetBookDoc, CCowDrwDoc)

CWetBookDoc::CWetBookDoc() : LPnumber(1)
{    
}
int CWetBookDoc::LoadInputData(CPtrArray &docInputData)
{
	m_cows.RemoveAll();
	m_cows.Append((CCowArray&)docInputData);

	LPnumber = PrintCfg().GetNum(CFGID_WETBOOK_LP);	
	return RELOAD_OK_CONTINUE;
}

int CWetBookDoc::GetPrintPages() const
{
	int pages = (int)m_cows.GetSize()/WETBOOKPAGESIZE;
    if (m_cows.GetSize()%WETBOOKPAGESIZE)  pages += 1;
    return pages;
		
}

CString CWetBookDoc::FormatHent(const Hent* pHent) const
{
	CString result;
	
	result += pHent->GetFarmNo();
	result += '\n';
	result += pHent->GetName();
	result += '\n';
	result += pHent->GetStreet();
	result += ' ';
	result += pHent->GetPOBox();
	result += '\n';
	result += pHent->GetZip();
	result += ' ';
	result += pHent->GetCity();
	
	if(pHent->HasWetNo())
	{
		result += '\n';
		result += "Nr ident. wet.: ";
		result += pHent->GetWetNo();
	}

	if(pHent->HasWetLicenceNo())
	{
		result += '\n';
		result += "Nr lic. wet.: ";
		result += pHent->GetWetLicenceNo();
	}

	return result;
}

CString CWetBookDoc::FormatLastOwner(const Hent* pHent) const
{
	CString result;

	result += pHent->GetName();
	result += '\n';
	result += pHent->GetStreet();
	result += ' ';
	result += pHent->GetPOBox();
	result += '\n';
	result += pHent->GetZip();
	result += ' ';
	result += pHent->GetCity();

	if(pHent->HasWetNo())
	{
		result += '\n';
		result += "Nr ident. wet.: ";
		result += pHent->GetWetNo();
	}

	return result;
}

CString CWetBookDoc::FormatCowNo(Cow *pCow)
{
	CString result = pCow->GetEAN();
	CString sPassNo = pCow->GetPassNo();
	CString sHealthCertNo = pCow->GetHealthCertNo();

	if(!sPassNo.IsEmpty())
	{
		result += "\n(";
		result += sPassNo;
		result += ')';
	}

	if(!sHealthCertNo.IsEmpty())
	{
		result += "\n[";
		result += sHealthCertNo;
		result += ']';
	}
	return result;
}

void CWetBookDoc::PrintPage(int page)
{
				
	int startidx = (page-1)*WETBOOKPAGESIZE,
		endidx  = (startidx+WETBOOKPAGESIZE)-1,
		size = (int)m_cows.GetSize(),
		i,row;

	
	Cow *cowd;
					
	if (endidx >= size)	endidx = size-1;
		
	for(i=startidx,row=1;i<=endidx;i++,row++)	
	{
		cowd = m_cows[i];
		
		CString sFstOwner = szMissing;
		CString sFstOwnerFarmNo = szMissing;
		if(cowd->HasFirstOwner())
		{
			sFstOwner = FormatLastOwner(cowd->GetFirstOwner());
			sFstOwnerFarmNo = cowd->GetFirstOwner()->GetFarmNo();
		}
		
			
		DrawText(CStringFormat("DELIVER%d",row),sFstOwner);

		
		DrawText(CStringFormat("FARM%d",row),sFstOwnerFarmNo);


		DrawText(CStringFormat("COW%d",row),FormatCowNo(cowd));

		
		DrawText(CStringFormat("FSTOWNER%d",row),FormatHent(cowd->GetDeliver()));

		
		DrawText(CStringFormat("INDATE%d",row),cowd->GetInDoc()->GetLoadDate().GetDayDate());

		if(cowd->IsOut())
		{			
			DrawText(CStringFormat("OUTDATE%d",row),cowd->GetOutDoc()->GetLoadDate().GetDayDate());
	
			DrawText(CStringFormat("PLATEOUT%d",row),cowd->GetOutDoc()->GetPlateNo());
			
			DrawText(CStringFormat("OUTPLACE%d",row),FormatHent(cowd->GetBuyer()));
		}

	
		
		DrawText(CStringFormat("PLATEIN%d",row),cowd->GetInDoc()->GetPlateNo());

		
		DrawText(CStringFormat("NUM%d",row),CIntString(LPnumber+i));					


	}



}
