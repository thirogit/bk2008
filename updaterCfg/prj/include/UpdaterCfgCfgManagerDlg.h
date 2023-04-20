// UpdaterCfgCfgManagerDlg.h : header file
//

// generated on 2009.10.30 at 17:39:29.468
//
#if !defined(_UPDATERCFGCFGMANAGERDLG_H_)
#define _UPDATERCFGCFGMANAGERDLG_H_

#ifdef IDD_CFG_MGR_TEMPLATE
    #undef IDD_CFG_MGR_TEMPLATE
#endif

#define IDD_CFG_MGR_TEMPLATE                1000

#include "stdafx.h"
#include "resource.h"
#include <map>
#include <vector>
#include "UpdaterCfgRoot.h"
#include "UpdaterCfgTreeNodeInfo.h"
#include "cmSplitDialog.h"


using namespace std;

class UpdaterCfgRoot;
class UpdaterCfgUpdaterCfg;
class IUpdaterCfgRootValidator;
class IUpdaterCfgUpdaterCfgValidator;

/////////////////////////////////////////////////////////////////////////////
// UpdaterCfgCfgManagerDlg dialog

class AFX_EXT_CLASS UpdaterCfgCfgManagerDlg : public cmSplitDialog
{

public:
    UpdaterCfgCfgManagerDlg(CWnd* pParent = NULL);
    virtual ~UpdaterCfgCfgManagerDlg();

    public:
        void OnTreeNodeSelection(HTREEITEM oldItem, HTREEITEM newItem);
        void OnTreeNodeExpansion(HTREEITEM item);

// Dialog Data
    //{{AFX_DATA(UpdaterCfgCfgManagerDlg)
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    CImageList imageList_;

    int viewIndex_EmptyDlg_;


    void SetData(UpdaterCfgRoot& root);
    UpdaterCfgRoot& GetData();

    void CreateAndEmbed(CWnd* parentWnd, const CRect& rc = CRect(0, 0, 0, 0));


    // set validators
    //
    void SetRootValidator(IUpdaterCfgRootValidator* validator);
    void SetUpdaterCfgValidator(IUpdaterCfgUpdaterCfgValidator* validator);

public:
    void DeleteArrayItemNode(HTREEITEM parentItem, int itemIndex, const cmString& caption);
    void DeleteArrayItemNode(HTREEITEM parentItem, vector<int>& itemIndexList, const cmString& caption);

public:
    void SelectArrayItemNode(HTREEITEM parentItem, int itemIndex);

public:
    void DeleteMapItemNode(HTREEITEM parentItem, const cmString& itemKey);
    void DeleteMapItemNode(HTREEITEM parentItem, vector<cmString>& itemKeyList);

public:
    void SelectMapItemNode(HTREEITEM parentItem, const cmString& itemKey);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(UpdaterCfgCfgManagerDlg)
    protected:
        virtual BOOL PreTranslateMessage(MSG* pMsg);
    //virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(UpdaterCfgCfgManagerDlg)
    virtual BOOL OnInitDialog();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMnuAppendItem_Array();
    afx_msg void OnMnuInsertItem_ArrayItem();
    afx_msg void OnMnuAppendItem_ArrayItem();
    afx_msg void OnMnuRemoveItem_ArrayItem();
    afx_msg void OnMnuCreateItem_Map();
    afx_msg void OnMnuRemoveItem_MapItem();
    afx_msg void OnMnuCreateItem_MapItem();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    void OnTreeNodeSelection();

// data members
//
protected:
    map<HTREEITEM, UpdaterCfgTreeNodeInfo> node2InfoMap_;
    UpdaterCfgRoot root_;
    int lastSelectedViewIndex_;
    HTREEITEM lastSelNode_;

    IUpdaterCfgRootValidator* validator_;

    IUpdaterCfgRootValidator* rootValidator_;
    IUpdaterCfgUpdaterCfgValidator* updaterCfgValidator_;

protected:

    cmTreeNodeDescriptor* BuildNode_Root(UpdaterCfgRoot& root, HTREEITEM parentItem);
    cmTreeNodeDescriptor* BuildNode_Root_UpdaterCfg(UpdaterCfgUpdaterCfg& updaterCfg, HTREEITEM parentItem);

protected:

    UpdaterCfgRoot* GetItem_Root(HTREEITEM treeItem);
    UpdaterCfgUpdaterCfg* GetItem_Root_UpdaterCfg(HTREEITEM treeItem);
    void InitializeTree();
    void StoreNodeData(HTREEITEM node);
    UpdaterCfgTreeNodeInfo* GetNodeInfo(HTREEITEM treeNode);
    void OnOK();
    void OnCancel();
    void DeleteNode(HTREEITEM treeItem);
    void UpdateNodeImages(HTREEITEM node);
};

#endif // !defined(_UPDATERCFGCFGMANAGERDLG_H_)

