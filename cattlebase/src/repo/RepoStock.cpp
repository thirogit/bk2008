#include "stdafx.h"
#include "RepoStock.h"


RepoStock::RepoStock()
{
}

RepoStock::RepoStock(const RepoStock& src)
{
	CopyFrom(src);
}

RepoStock::RepoStock(const Stock& srcStock)
{
	CopyFrom(srcStock);
}

void RepoStock::CopyFrom(const RepoStock& src)
{
	stockname = src.stockname;
	stockcode = src.stockcode;
	myperkgprice = src.myperkgprice;
	predefsex = src.predefsex;
}

void RepoStock::CopyFrom(const Stock& srcStock)
{
	stockname = srcStock.GetStockName();
	stockcode = srcStock.GetStockCode();
	myperkgprice = srcStock.GetMyPerKgPrice();
	predefsex = srcStock.GetPredefSex();
}

const CString&	RepoStock::GetStockName() const
{
	return stockname;
}

void RepoStock::SetStockName(const CString& stockName)
{
	stockname = stockName;
}

const CString&	RepoStock::GetStockCode() const
{
	return stockcode;
}

void RepoStock::SetStockCode(const CString& stockCode)
{
	stockcode = stockCode;
}

const Money& RepoStock::GetMyPerKgPrice() const
{
	return myperkgprice;
}

void RepoStock::SetMyPerKgPrice(const Money& dMyPerKgPrice)
{
	myperkgprice = dMyPerKgPrice;
}

const CowSex& RepoStock::GetPredefSex() const
{
	return predefsex;
}

void RepoStock::SetPredefSex(const CowSex& cowsex)
{
	predefsex = cowsex;
}