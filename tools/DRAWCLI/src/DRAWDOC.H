
#ifndef __DRAWDOC_H__
#define __DRAWDOC_H__

#include "drawobj.h"
#include "summinfo.h"

#include "TextStyle.h"
#include <afxole.h>

#if !defined(FOR_CATTLEBASE)

#include <xercesc/dom/DOM.hpp>
#include < Prof-UIS.h > 
#include "drawobjgroup.h"
#define CATTLEPRINT_DLLAPI

#else

#define CATTLEPRINT_DLLAPI
//AFX_EXT_CLASS

#define RELOAD_NO_CONTINUE 0
#define RELOAD_OK_CONTINUE 1
#define RELOAD_CAN_CONTINUE (-1)

#include "DataTypes.h"

#endif

#define USERDEF_STYLENAME "USERDEF"
#define USERDEF_STYLEID (-1)

class CDrawView;

class CATTLEPRINT_DLLAPI CDrawDoc : public COleDocument
{
#if !defined(FOR_CATTLEBASE) // we need public constructor
protected:
#else
public:
#endif
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

// Attributes
public:
	CDrawObjList* GetObjects();
	CDrawObjList* GetPageObjects(int nPage);

	inline CGroupList* GetGroupList() {	return &m_groups; };
	inline CStyleList* GetStyleList() {	return &m_styles; };
	inline const CSize& GetPageSize() const { return m_PageSize; }
	inline const CSize& GetPaperSize() const { return m_PaperSize; }
	inline const CSize& GetPhyMarginsSize() const { return m_PhyMargins; };
	
	void ComputePageSize();
	inline int GetMapMode() const { return m_nMapMode; }
	inline COLORREF GetPaperColor() const { return m_paperColor; }
	inline int GetNumPages() { return m_NumPages; };
	bool SetCurrPage(int nPage);
	void Draw(CDC* pDC, CDrawView* pView);
	CDrawObj* GetObjByLabel(const char *label);
	inline CSize GetMarginOffset() { return m_MargOff; };
protected:
	CSummInfo *m_pSummInfo;
	int m_orientation;
	int m_paperSize;
	CSize m_MargOff;
#if (defined(FOR_CATTLEBASE))
public:
	virtual bool CreateDocument(CPtrArray &inputDataArr,LPCSTR templatePath,UINT nCfgID);
	//those functions has to be overloaded
	virtual int GetPrintPages()  { return GetNumPages(); };
	virtual void PrintPage(int page) {};
	virtual void SetDocOptions(CWnd *pParent) {};
	virtual void PreparePage(int nPage) {};
	virtual int Reload()  { return RELOAD_OK_CONTINUE; };
	void DrawText(const char *szLabel,int iNumber);
	void DrawText(const char *szLabel,LPCSTR szStrFmt,...);
	void DrawText(const char *szLabel,int row,LPCSTR szStrFmt,...);
	virtual void DrawText(const char *szLabel,CString &strText);
	void PutChar(const char *szLabel,char chr);
	void ResetPrintVis(bool bPrintVis);
	bool SetGroupPrintVis(int groupId,bool bPrintVis);
	bool SetGroupPrintVis(LPCSTR groupName,bool bPrintVis);
	bool SetUserDefStyle(LPCSTR szFaceName,int Height);
	void SetPrintDC(CDC *pDC);
	inline bool InForm() { return m_printInForm; };
	void SetInForm(bool bInForm) { m_printInForm = bInForm; }; 	
	inline UINT GetCfgItemID() { return m_nCfgID; };
	void SetMargOffets(int cx,int cy) { m_MargOff.SetSize(cx,cy); };
	void SetSortBy(COWDATA_MEMBERS nSortBy) { m_nSortBy = nSortBy; };
	inline COWDATA_MEMBERS GetSortBy() { return m_nSortBy; };
	void SetSortOrder(bool bAsc) { m_bAsc = bAsc; };
	inline bool GetSortOrder() { return m_bAsc; };
	void DrawImage(const char *szLabel,COleStreamFile &pictureStream);

private:
	
	bool m_printInForm;
	UINT m_nCfgID;
	CDC *m_pDC;	
	COWDATA_MEMBERS m_nSortBy;
	bool m_bAsc;
#else
public:
	CDrawObj* ObjectAt(const CPoint& point);
	void Add(CDrawObj* pObj);
	void Remove(CDrawObj* pObj);
	void NewPage(void);
	void DelPage(void);
	void NextPage(void);
	void PrevPage(void);
	virtual void SerializeXml(xercesc::DOMElement * pParentElement);

#endif

// Implementation
public:
	virtual ~CDrawDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


#if !defined(FOR_CATTLEBASE)
protected: // create from serialization only
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
#else
public:
#endif
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);


private:	
	CGroupList m_groups;
	CStyleList m_styles;
	CPtrArray m_DocPages;
	int m_CurrPage;
	int m_NumPages;

	CSize m_PaperSize;
	CSize m_PageSize;
	CSize m_PhyMargins;
	int m_nMapMode;
	COLORREF m_paperColor;
	

// Generated message map functions
protected:

#ifndef FOR_CATTLEBASE
	//{{AFX_MSG(CDrawDoc)
	afx_msg void OnViewPaperColor();
	afx_msg void OnFileSummaryInfo();
	afx_msg void OnFilePrintSetup();
	//}}AFX_MSG
#endif
	DECLARE_MESSAGE_MAP()
};

#endif //__DRAWDOC_H__

/////////////////////////////////////////////////////////////////////////////
