#include "stdafx.h"
#include "CowListCtrl.h"
#include "MarkWarnShowOptions.h"
#include "../consts.h"
#include "MainCfg.h"


#define COWS_SORT_BY_LP (COWDATA_MEMBER_MAX+1)

GRIDHEADERS CInCowListCtrl::InListCowHeader[] = 
{ 
	{IDS_HEADERINDX,COWS_SORT_BY_LP},
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADERBIRTHDATE,COWS_SORT_BY_BIRTHDATE},
	{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
	{IDS_HEADERSEX,COWS_SORT_BY_SEX},
	{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
	{IDS_HEADERFSTOWNER,COWS_SORT_BY_FSTOWNER},
	{IDS_HEADEROUTGRP,COWS_SORT_BY_INGRP},
	{0,0}
	
};

//***************************************************************************************

GRIDHEADERS COutCowListCtrl::OutListCowHeader[] = 
{ 
	{IDS_HEADERINDX,COWS_SORT_BY_LP},
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADERBIRTHDATE,COWS_SORT_BY_BIRTHDATE},
	{IDS_HEADERWEIGHT,COWS_SORT_BY_WEIGHT},
	{IDS_HEADERSEX,COWS_SORT_BY_SEX},
	{IDS_HEADERSTOCK,COWS_SORT_BY_STOCK},
	{IDS_HEADERDELIVER,COWS_SORT_BY_DELIVER},
	{IDS_HEADEROUTGRP,COWS_SORT_BY_OUTGRP},
	{0,0}
	
};

BEGIN_MESSAGE_MAP(CCowListCtrl, CBaseListCtrl)
		
END_MESSAGE_MAP()

CCowListCtrl::CCowListCtrl(CCowArray *cowArray,const GRIDHEADERS *ppHeaders,int iConfigId/* = -1*/) : CBaseListCtrl(ppHeaders,iConfigId) ,m_cowArray(cowArray)
{
	ASSERT(cowArray);
	m_mws_opts = CMarkWarnShowOptions::GetInstance();
}


void CCowListCtrl::SortByCol(int Column,bool bAsc)
{
	UINT colId = GetColID(Column);
	if(colId != COWS_SORT_BY_LP)
		m_cowArray->SortBy(colId,bAsc);
	
}


void CCowListCtrl::UpdateItemCount()
{
	SetItemCount(m_cowArray->GetSize());

}

void CCowListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	CString sTemp;

		Cow *cowdata = (*m_cowArray)[nItem];
		switch(GetColID(nSubItem))
		{
			case COWS_SORT_BY_LP:
				sTemp.Format("%d",nItem+1);				
			break;
			case COWS_SORT_BY_EAN:
				sTemp = cowdata->GetEAN();
			break;
			case COWS_SORT_BY_PASSNO:
				sTemp = cowdata->GetPassNo();
			break;
			case COWS_SORT_BY_BIRTHDATE:
				sTemp =  cowdata->GetBirthDate().GetDayDate();
			break;
			case COWS_SORT_BY_WEIGHT:
				sTemp = cowdata->GetWeight().ToString();
				
			break;
			case COWS_SORT_BY_SEX:
				sTemp = (LPCSTR)cowdata->GetSex();
			break;
			case COWS_SORT_BY_STOCK:
				sTemp = cowdata->GetStock()->GetStockCode();
			break;
			case COWS_SORT_BY_FSTOWNER:
				if(cowdata->HasFirstOwner())
					sTemp = cowdata->GetFirstOwner()->GetAlias();
			else
				sTemp =  szMissing;
			break;
			case COWS_SORT_BY_DELIVER:
				sTemp = cowdata->GetDeliver()->GetAlias();
			break;
			case COWS_SORT_BY_OUTGRP:
				sTemp = cowdata->GetOutGroup().ToString();
			break;
			case COWS_SORT_BY_INGRP:
				sTemp = cowdata->GetInGroup().ToString();
			break;
			default:
				ASSERT(0);

		}
		strcpy(pszItemText,(LPCSTR)sTemp);
	


}

bool CCowListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	Cow *cowdata = (*m_cowArray)[nItem];
	switch(GetColID(nSubItem))
	{
		case COWS_SORT_BY_LP:
			
			if(cowdata->IsNew())
			{
				*cellBkg = RGB(0,128,255);
			}
			else
			{

			if(cowdata->IsOut())
				*cellBkg = g_SoldThisCowClr;
			else
				*cellBkg = g_GotThisCowClr;
			}

			break;
		case COWS_SORT_BY_BIRTHDATE:
		
			if(m_mws_opts->GetMarkAge())
			{
				if(cowdata->HasBirthDate())
				{
					if(cowdata->GetBirthDate().GetDiffrenceInDays(CDateTime::now()) > m_mws_opts->GetMarkDays())
					{
						*cellBkg = m_mws_opts->GetMarkCowAgeColor();
						break;
					}
				}
			}
			return false;			
		default:
			return false;
	}

	return true;
}

//***************************************************************************************
COutCowListCtrl::COutCowListCtrl(CCowArray *cowArray,int iConfigId/* = -1*/) :
	  CCowListCtrl(cowArray,OutListCowHeader,iConfigId)
{

	for(int i = 0;i <= MAX_INOUT_GRP;i++)
	{
		m_grpColors[i] = AppCfg().GetRGB(CFG_GROUPCOLORS_ITEM,i);
	}
}

bool COutCowListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{

	if(!CCowListCtrl::GetCellColor(nItem,nSubItem,cellBkg))
	{
		if(GetColID(nSubItem) == COWS_SORT_BY_OUTGRP)
		{
			Cow *cowdata = (*m_cowArray)[nItem];
			NullInt outGrp = cowdata->GetOutGroup();
			if(!outGrp.IsNull())
			{
				int outgrp = outGrp.ToInt();
				if(outgrp >= 0 && outgrp <= MAX_INOUT_GRP)
				{
					*cellBkg = m_grpColors[outgrp];
					return true;
				}

			}
		}
		return false;
	}
	return true;
}





