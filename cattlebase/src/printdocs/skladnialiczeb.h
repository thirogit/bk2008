#ifndef __SKLADNIALICZEB_H__
#define __SKLADNIALICZEB_H__


class CSkladniaLiczebnikow
{
public:
	void Init(const CString& nazwaLiczeb1,const CString& nazwaLiczeb2_4,const CString& nazwaLiczebPozost);
	CString Odmiana(int liczba) const;
private:
	CString m_nazwaLiczeb1;
	CString m_nazwaLiczeb2_4;
	CString m_nazwaLiczebPozost;
};


#endif
