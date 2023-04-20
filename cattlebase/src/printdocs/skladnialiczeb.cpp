#include "stdafx.h"
#include "skladnialiczeb.h"



void CSkladniaLiczebnikow::Init(const CString& nazwaLiczeb1,const CString& nazwaLiczeb2_4,const CString& nazwaLiczebPozost)
{
	m_nazwaLiczeb1 = nazwaLiczeb1;
	m_nazwaLiczeb2_4 = nazwaLiczeb2_4;
	m_nazwaLiczebPozost = nazwaLiczebPozost;
}
CString CSkladniaLiczebnikow::Odmiana(int liczba) const
{
	LPCSTR szOdmiana[3] = 
	{
		(LPCSTR)m_nazwaLiczeb1,
		(LPCSTR)m_nazwaLiczeb2_4,
		(LPCSTR)m_nazwaLiczebPozost		
	};
	UINT a,bc,b,c,abc;
	UINT Index = 0;
			
	abc = liczba%1000;	
	a = abc / 100;
    bc = abc %100;
	b = bc/10;
	c = bc%10;
			
    if(abc != 1)
	{
		if(b !=  1 && c >= 2 && c <= 4) Index =  1;
		else Index =  2;
	}    
	return szOdmiana[Index];
}
	