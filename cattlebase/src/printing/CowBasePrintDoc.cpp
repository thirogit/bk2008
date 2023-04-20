#include "stdafx.h"
#include "CowBasePrintDoc.h"


CCowBasePrintDoc::CCowBasePrintDoc()
{
	m_MargOff.SetSize(0,0);
	m_nCfgID = 0;
	m_bAsc = true;
}

int CCowBasePrintDoc::Load(CPtrArray &docInputData)
{	
	return LoadInputData(docInputData);
}


void CCowBasePrintDoc::CreateDocument(const PRINTDOCS* pDoc)
{
	m_nCfgID = pDoc->iCfgItemId;
	CreateCowDocument(pDoc);
}


const FloatFormat& CCowBasePrintDoc::GetMoneyFormat() const
{
	return m_MoneyFormat;
}

const FloatFormat& CCowBasePrintDoc::GetWeightFormat() const
{
	return m_WeightFormat;
}



void CCowBasePrintDoc::SetMoneyFormat(const FloatFormat& fmt)
{
	m_MoneyFormat = fmt;
}

void CCowBasePrintDoc::SetWeightFormat(const FloatFormat& fmt)
{
	m_WeightFormat = fmt;
}

