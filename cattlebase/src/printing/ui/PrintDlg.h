#if !defined(__PRINTDLG_H__)
#define __PRINTDLG_H__


#include "imgbutton\ImageButton.h"
#include "combo\TTComboBox.h"
#include "..\..\datatypes\ExpenseDataArray.h"
#include "..\..\ctrls\BaseListCtrl.h"
#include "..\..\printdocs\printdocs.h"
#include "..\CowBasePrintDoc.h"


class CPrintDlg : public CDialog
{
// Construction
public:
	CPrintDlg(CRawDataArray &DataArr,int DocId,CWnd* pParent = NULL);
	virtual ~CPrintDlg();
	enum { IDD = IDD_PRINT };

private:

	CTTComboBox m_PrintDocCombo;
	CImageButton m_GoPrint;
	CImageButton m_PrintOpt;
	CImageButton m_Preview;
	CImageButton m_Delete;
	CImageButton m_Cancel;
	
	CSingleDocTemplate* m_pTemplate;
	const PRINTDOCS		*m_PRINTDOC;
	CCowBasePrintDoc	*m_pDoc;


	CPtrArray		*m_DataArray;
	CBaseListCtrl	*m_pList;

	static GRIDHEADERS PrintListCowHeader[];
	static GRIDHEADERS PrintListInvoiceHeader[];
	static GRIDHEADERS PrintListInDocHeader[];
	static GRIDHEADERS PrintListOutDocHeader[];
	static GRIDHEADERS PrintListExpensesHeader[];
	static GRIDHEADERS PrintListHentsHeader[];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog() ;
	//void CreateNewDoc(void);
	//bool DoReload();
	void CreateList(UINT nDataType);
	//void CowsOptEnable(BOOL bEnable);
	//void SetDocOptions(CCowBasePrintDoc *pCowDoc);
	const PRINTDOCS* FindPRINTDOC(UINT nDocId);
	//void SwitchDataArray(UINT nDataType);
	//void Init();
	void PrintDocument(bool bPreview,int nCopies);
	

// Implementation
protected:
	
	afx_msg void OnPrintPreview();
	afx_msg void OnPrint();
	afx_msg void OnPrintOpt();
	afx_msg void OnPrintDelete();

	afx_msg void OnPrintTwoCopies();
	afx_msg void OnPrintThreeCopies();
	afx_msg void OnPrintFourCopies();

	
	DECLARE_MESSAGE_MAP()
};

#endif 
