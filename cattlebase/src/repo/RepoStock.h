#ifndef __REPOSTOCK_H__
#define __REPOSTOCK_H__

#include "../classes/types/Money.h"
#include "../classes/types/CowSex.h"
#include "../datatypes/Stock.h"

class RepoStock
{
public:
	RepoStock();
	RepoStock(const RepoStock& src);
	RepoStock(const Stock& srcStock);

	void CopyFrom(const RepoStock& src);
	void CopyFrom(const Stock& srcStock);

	const CString&	GetStockName() const;
	void			SetStockName(const CString& stockName);
	

	const CString&	GetStockCode() const;
	void			SetStockCode(const CString& stockCode);
	

	const Money&	GetMyPerKgPrice() const;
	void			SetMyPerKgPrice(const Money& dMyPerKgPrice);
	

	const CowSex&	GetPredefSex() const;
	void			SetPredefSex(const CowSex& cowsex);
		
private:
	CString	stockname;
	CString stockcode;
	Money	myperkgprice;
	CowSex	predefsex;	
};


	

#endif