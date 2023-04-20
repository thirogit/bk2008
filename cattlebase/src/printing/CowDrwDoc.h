#ifndef __COWDRWDOC_H__
#define __COWDRWDOC_H__

#include "CowBasePrintDoc.h"
#include "GroupList.h"
#include "TextStyleList.h"
#include "DocObjList.h"
#include "DocSummary.h"
#include "DocTextObj.h"

class CDocObj;

class CCowDrwDoc : public CCowBasePrintDoc
{		
public:
	CCowDrwDoc();
	~CCowDrwDoc();

	void CreateCowDocument(const PRINTDOCS* pDoc);
	virtual int Load(CPtrArray &docInputData);
	bool SetTemplatePage(int nPage);
	int GetTemplatePages() const;

	void DrawText(const CString& sLabel,const CString &sText);
	void DrawImage(const CString& sLabel,COleStreamFile &pictureStream);
	
	void ResetGroupsVisibility(bool bVisible);
	bool SetGroupVisible(const CString& sGroupName,bool bVisible);

	bool SetUserDefStyle(const CString& sFaceName,int Height);
	virtual CSize GetPaperSize() const;
	const CSize& GetPhyMarginsSize() const;
	COLORREF GetPaperColor() const;
	const CSize& GetPageSize() const;
	void ComputePageSize();
	virtual void Draw(CDC* pDC);
	void SetPrintDC(CDC *pDC);
	virtual void PrintPage(int page) = 0;
	virtual void PreparePage(int nPage);
	void Serialize(CArchive& ar);

	CGroupList* GetGroupList();
	CTextStyleList* GetStyleList();

protected:
	virtual bool OpenDocumentTemplate(const CString& templatePath);
	CDocObj* GetObjByLabel(const CString& sObjLabel);
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void DestroyDocument();
	void SetDocOptions();
	
	CDocObjList* GetObjects();
	CDocObjList* GetPageObjects(int nPage);
private:
		
	CGroupList m_groups;
	CTextStyleList m_styles;
	
	CPtrArray m_DocPages;
	
	int m_CurrPage;
	//int m_NumPages;
	CSize m_PaperSize;
	CSize m_PageSize;
	CSize m_PhyMargins;
	COLORREF m_paperColor;

	CDocSummary *m_pDocSummary;
	int m_orientation;
	int m_PaperType;

	CDC *m_pDC;
};

#endif