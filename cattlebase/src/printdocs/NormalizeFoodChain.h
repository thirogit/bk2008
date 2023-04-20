#ifndef __NORMALIZEFOODCHAIN_H__
#define __NORMALIZEFOODCHAIN_H__

#include "../printing/CowDrwDoc.h"

#include "../DataTypes.h"

#define NORM_FOOD_CHAIN_COWS_ON_PAGE 9

class CNormalizeFoodChain : public CCowDrwDoc
{
	class OnePageContent
	{
	public:
		Hent* firstOwner;
		CCowArray cows;
	};

	DECLARE_DYNCREATE(CNormalizeFoodChain)
public:
	CNormalizeFoodChain();
	~CNormalizeFoodChain();
	
	int GetPrintPages() const;
	void PrintPage(int page);
	int LoadInputData(CPtrArray &invInputData);	


	
protected:
	CString FormatCowNo(Cow *pCow);
private:
	PtrFlushArray<OnePageContent> m_content;
	


};

#endif
