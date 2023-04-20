// cattlebasefrmDoc.h : interface of the CCattlebaseDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATTLEBASEFRMDOC_H__BBC1EE1F_4436_45B0_84A1_69CC268BCCEC__INCLUDED_)
#define AFX_CATTLEBASEFRMDOC_H__BBC1EE1F_4436_45B0_84A1_69CC268BCCEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCattlebaseDoc : public CDocument
{
protected: // create from serialization only
	CCattlebaseDoc();
	DECLARE_DYNCREATE(CCattlebaseDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCattlebaseDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCattlebaseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCattlebaseDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATTLEBASEFRMDOC_H__BBC1EE1F_4436_45B0_84A1_69CC268BCCEC__INCLUDED_)
