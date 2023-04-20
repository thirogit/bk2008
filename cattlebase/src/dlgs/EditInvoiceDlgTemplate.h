#ifndef __EDITINVOICEDLGTEMPLATE_H__
#define __EDITINVOICEDLGTEMPLATE_H__

#include "../accessor/CowAccessor.h"


template<class BASEINVDLG,class INVDATACLASS>
class CEditInvoiceDlgTemplate : public BASEINVDLG
{
public:
	CEditInvoiceDlgTemplate(INVDATACLASS *pInvoice,CWnd* pParent = NULL) : 
		BASEINVDLG(pInvoice->invData,pParent)
	{
		m_Inv2Edit = pInvoice;
	}
protected:
	virtual BOOL OnInitDialog()
	{

		BASEINVDLG::OnInitDialog();

		m_vat.SetCurVATRate(m_Inv2Edit->VATrate*100);
		CowPriceData::SetVat(m_Inv2Edit->VATrate);
		m_payway.ReloadCombo(m_Inv2Edit->payway);	
		m_extras.SetWindowText(m_Inv2Edit->extras);
		m_customNumber = m_Inv2Edit->customnumber;
		UpdateData(FALSE);
		
		CowPriceData *cowPrice;
		CowDataAccessor * cowAccessor = CowAccessorFactory::createAccessor(CowPriceData::buy_sell);
		for(int i = 0; i < m_cowprices.GetSize();i++)
		{	
			cowPrice = m_cowprices[i];
			cowAccessor->set(cowPrice->srcData);

			acowPrice->SetCustomPriceNetto(cowAccessor->GetPrice());
			acowPrice->SetCustomStock(cowAccessor->GetStock());
			acowPrice->SetCustomWeight(cowAccessor->GetWeight().ToInt());
		}
		delete cowAccessor;

		ComputeTotal();

		return TRUE;


	}

	CString GetInvDlgCaption()
	{
		CString caption;	
		caption.Format(IDS_EDITINVCAPTION,m_Inv2Edit->invoiceno,(LPCSTR)m_invHent->name);	
		return caption;
	}

	INVDATACLASS *m_Inv2Edit;

};

#endif