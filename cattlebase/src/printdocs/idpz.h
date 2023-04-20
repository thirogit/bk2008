#ifndef __COWTRANSINFO_H__
#define __COWTRANSINFO_H__

#include "../printing/CowDrwDoc.h"
#include "zapisslowny.h"
#include "../DataTypes.h"
#include "../barcodeimage/BarcodeImage.h"
#include "../barcodeimage/Code39.h"
#include "../barcodeimage/PDF417.h"
#include "PrintDocsCfg.h"
#include "CustomPrintDocOptDlg.h"

class CTransInfoDocOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CTransInfoDocOptDlg)
public:
	CTransInfoDocOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_IDPZDOCOPT };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();
private:
	CString m_Spiecies;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);		
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};


class CBaseCowTransInfoDoc : public CCowDrwDoc
{	
public:
	CBaseCowTransInfoDoc();
	int GetPrintPages() const;
	virtual int LoadInputData(CPtrArray &docInputData);
	void PreparePage(int nPage);
	void PrintPage(int page);
	
	virtual int GetCowsOnPage1() = 0;
	virtual int GetCowsOnPage2() = 0;
	virtual void PrintCow(int row,int LP,const Cow* pCow);
	virtual void PrepareCow(const Cow* pCow,int row);

	enum CFG_ITEM_INDEX
	{
		CFG_SPIECIES = CPrintCfg::CFG_PRINT_OPTION_MAX		 		
	};

protected:
	CDocDataArray m_docs;
	Doc *m_currDoc;
	int starti;
	int endi;
	int pageOff;
	int m_nPages;
	int m_currDocPageSize;
	CCowArray m_currDocCows;
	
};

class CCowTransInfoDoc : public CBaseCowTransInfoDoc
{
	DECLARE_DYNCREATE(CCowTransInfoDoc)
public:
	virtual int GetCowsOnPage1();
	virtual int GetCowsOnPage2();	
public:
};

class CBarcodesCowTransInfoDoc : public CBaseCowTransInfoDoc
{
	DECLARE_DYNCREATE(CBarcodesCowTransInfoDoc)
public:
	CBarcodesCowTransInfoDoc();
	~CBarcodesCowTransInfoDoc();
protected:
	class CowBarcodeImages
	{
	public:
		CowBarcodeImages(const Cow* pCow);
		~CowBarcodeImages();

		void GetCOWNOBCImageStream(COleStreamFile& cowNoBcImgStream);
		void GetPASSNOBCImageStream(COleStreamFile& passNoBcImgStream);
		void GetPDF417ImageStream(COleStreamFile& pdf417ImgStream);
	protected:
		CCode39			m_bcCode39EAN;
		CBarcodeImage	m_bcCode39EANImage;

		CCode39			m_bcCode39PassNo;
		CBarcodeImage	m_bcCode39PassNoImage;

		CPDF417			m_bcPFD417;
		CBarcodeImage	m_bcPFD417Image;

	
	};

	virtual int LoadInputData(CPtrArray &docInputData);
	virtual int GetCowsOnPage1();
	virtual int GetCowsOnPage2();
	virtual void PrepareCow(const Cow* pCow,int row);

	void ClearBarcodesMap();

	CMap<const Cow*,const Cow*,CowBarcodeImages*,CowBarcodeImages*> m_cowBarcodesMap;


};

#endif
