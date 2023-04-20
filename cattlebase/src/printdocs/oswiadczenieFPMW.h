#ifndef __OSWIADCZENIEFPWM_H__
#define __OSWIADCZENIEFPWM_H__

#include "../printing/CowDrwDoc.h"
#include "../DataTypes.h"


class COswiadczenieFPWMDoc : public CCowDrwDoc
{
	DECLARE_DYNCREATE(COswiadczenieFPWMDoc)
public:
	COswiadczenieFPWMDoc();
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int nPage);

private:
	void PrintBuyer(const Hent *buyer);
	void PrintSeller(const Hent *seller);

	CInvoiceDataArray m_invs;
	Invoice *m_currInv;	
};

#endif
