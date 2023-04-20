#ifndef __COWBASEPRINTDOC_H__
#define __COWBASEPRINTDOC_H__

#include <afxole.h>

#include "../DataTypes.h"
#include "../printdocs/printdocs.h"
#include "../classes/format/FloatFormat.h"

#define RELOAD_NO_CONTINUE 0
#define RELOAD_OK_CONTINUE 1
#define RELOAD_CAN_CONTINUE (-1)

class CCowBasePrintDoc : public COleDocument 
{
public:
	CCowBasePrintDoc();
	virtual void CreateDocument(const PRINTDOCS* pDoc);
	virtual void CreateCowDocument(const PRINTDOCS* pDoc) = 0;
	virtual int Load(CPtrArray &docInputData);
	void SetMargOffets(int cx,int cy) { m_MargOff.SetSize(cx,cy); };
	void SetSortBy(COWDATA_MEMBERS nSortBy) { m_nSortBy = nSortBy; };
	void SetSortOrder(bool bAsc) { m_bAsc = bAsc; };
	inline UINT GetCfgItemID() { return m_nCfgID; };	
	inline COWDATA_MEMBERS GetSortBy() { return m_nSortBy; };
	inline bool GetSortOrder() { return m_bAsc; };
	inline CSize GetMarginOffset() { return m_MargOff; };
	//virtual void Draw(CDC* pDC) = 0;
	virtual CSize GetPaperSize() const = 0; //MM / 10
	virtual int GetPrintPages() const = 0;

	const FloatFormat& GetMoneyFormat() const;
	const FloatFormat& GetWeightFormat() const;
	

	void SetMoneyFormat(const FloatFormat& fmt);
	void SetWeightFormat(const FloatFormat& fmt);
	

protected:
	virtual int LoadInputData(CPtrArray &docInputData) = 0;
private:
	CSize m_MargOff;
	COWDATA_MEMBERS m_nSortBy;
	bool m_bAsc;
	UINT m_nCfgID;
	FloatFormat m_MoneyFormat;
	FloatFormat m_WeightFormat;
	

};

#endif