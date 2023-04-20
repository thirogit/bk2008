// UpdaterCfgCfgManagerDlg.cpp : implementation file
//
// generated on 2009.10.30 at 17:39:29.484
//

#pragma warning(disable:4786)
#pragma warning(disable:4060) // warning C4060: switch statement contains no 'case' or 'default' labels

#include "stdafx.h"
#include <afxcview.h>
#include "UpdaterCfgCfgManagerDlg.h"
#include "resource.h"
#include "UpdaterCfgResource.h"
#include "InputBox.h"

// classes
//
#include "UpdaterCfgRoot.h"
#include "UpdaterCfgUpdaterCfg.h"

// validators
//
#include "IUpdaterCfgRootValidator.h"
#include "IUpdaterCfgUpdaterCfgValidator.h"

// dialogs
//


#define HIGHLIGHT_TEXT_COLOR RGB(200, 0, 0)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgClass1 dialog

UpdaterCfgCfgManagerDlg::UpdaterCfgCfgManagerDlg(CWnd* pParent /*=NULL*/):
        cmSplitDialog(pParent),
        lastSelectedViewIndex_(-1),
    rootValidator_(NULL),
    updaterCfgValidator_(NULL),
        lastSelNode_(NULL)
{
    //{{AFX_DATA_INIT(UpdaterCfgCfgManagerDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

UpdaterCfgCfgManagerDlg::~UpdaterCfgCfgManagerDlg()
{


}

BEGIN_MESSAGE_MAP(UpdaterCfgCfgManagerDlg, cmSplitDialog)
    //{{AFX_MSG_MAP(UpdaterCfgCfgManagerDlg)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EDITARRAY_APPENDITEM, OnMnuAppendItem_Array)
    ON_COMMAND(ID_EDITARRAYITEM_INSERTITEM, OnMnuInsertItem_ArrayItem)
    ON_COMMAND(ID_EDITARRAYITEM_APPENDITEM, OnMnuAppendItem_ArrayItem)
    ON_COMMAND(ID_EDITARRAYITEM_REMOVEITEM, OnMnuRemoveItem_ArrayItem)
    ON_COMMAND(ID_EDITMAP_CREATEITEM, OnMnuCreateItem_Map)
    ON_COMMAND(ID_EDITMAPITEM_REMOVEITEM, OnMnuRemoveItem_MapItem)
    ON_COMMAND(ID_EDITMAPITEM_CREATEITEM, OnMnuCreateItem_MapItem)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UpdaterCfgCfgManagerDlg message handlers

BOOL UpdaterCfgCfgManagerDlg::OnInitDialog()
{
    cmSplitDialog::OnInitDialog();

    CRect rc;

    GetClientRect(rc);

    imageList_.Create(16, 16, ILC_COLOR32 | ILC_MASK, 50, 50);
    tree_.SetImageList(&imageList_, TVSIL_NORMAL);
    tree_.ModifyStyle(TVS_FULLROWSELECT | TVS_SINGLEEXPAND, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS);
    tree_.SetIndent(20);
    // create dialogs
    //

    // setup child list
    //


    InitializeTree();

    // select root node
    //
    HTREEITEM rootItem;

    rootItem = tree_.GetRootItem();
    SelectChild(viewIndex_EmptyDlg_);
    tree_.Select(rootItem, TVGN_CARET);
    OnTreeNodeSelection();

    // position and size
    //
    int left = 0;
    int top = 0;
    int width;
    int height;
    CWnd* parent;

    width = 640;
    height = 480;
    parent = GetParent();
    if (parent) {
        CRect rc;

        parent->GetWindowRect(rc);
        left = rc.CenterPoint().x - (width / 2);
        top = rc.CenterPoint().y - (height / 2);
    }
    SetWindowPos(&CWnd::wndTop, left, top, width, height, 0);
    splitterPos_ = 192;

    SetWindowText(_T(""));
    UpdateWindowSize();

    return TRUE;  // return TRUE unless you set the focus to a control
    			   // EXCEPTION: OCX Property Pages should return FALSE
}

int UpdaterCfgCfgManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (cmSplitDialog::OnCreate(lpCreateStruct) == -1)
        return -1;


    return 0;

}

BOOL UpdaterCfgCfgManagerDlg::PreTranslateMessage(MSG* pMsg)
{

    return cmSplitDialog::PreTranslateMessage(pMsg);
}
void UpdaterCfgCfgManagerDlg::InitializeTree()
{
    tree_.CleanUp();
    BuildNode_Root(root_, TVI_ROOT);
    tree_.Select(tree_.GetRootItem(), TVGN_CARET);
    OnTreeNodeSelection();
}

void UpdaterCfgCfgManagerDlg::StoreNodeData(HTREEITEM node)
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;

    it = node2InfoMap_.find(node);
    if(it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        switch(nodeInfo.GetClassId()) {
        case IDD_UPDATERCFG_0001:
            break;

        case IDD_UPDATERCFG_0002:
            break;

        }
    }

}

void UpdaterCfgCfgManagerDlg::OnOK()
{
    if (embedded_)
        return;

    if (tree_.GetSelectedItem())
        StoreNodeData(tree_.GetSelectedItem());

    cmSplitDialog::OnOK();
}

void UpdaterCfgCfgManagerDlg::OnCancel()
{
    if (embedded_)
        return;

    cmSplitDialog::OnCancel();
}

void UpdaterCfgCfgManagerDlg::OnTreeNodeSelection()
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;
    UpdaterCfgTreeNodeInfo parentNodeInfo;
    HTREEITEM parentNode;
    HTREEITEM selNode;

    selNode = tree_.GetSelectedItem();

    // store data for last selected dialog and hide it
    //
    if (lastSelNode_)
        StoreNodeData(lastSelNode_);

    // get parent node info
    //
    it = node2InfoMap_.find(parentNode);
    if(it != node2InfoMap_.end())
        parentNodeInfo = it->second;

    it = node2InfoMap_.find(selNode);
    if(it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        switch(nodeInfo.GetClassId()) {
        }
    }

    RedrawWindow();

}

void UpdaterCfgCfgManagerDlg::SetData(UpdaterCfgRoot& root)
{
    root_ = root;
    lastSelNode_ = NULL;
    if (embedded_)
        InitializeTree();
}

UpdaterCfgRoot& UpdaterCfgCfgManagerDlg::GetData()
{
    if (embedded_ && tree_.GetSelectedItem())
        StoreNodeData(tree_.GetSelectedItem());

    return root_;
}

cmTreeNodeDescriptor* UpdaterCfgCfgManagerDlg::BuildNode_Root(UpdaterCfgRoot& root, HTREEITEM parentItem)
{
    cmTreeNodeDescriptor* newItem = NULL;

    return newItem;
}

cmTreeNodeDescriptor* UpdaterCfgCfgManagerDlg::BuildNode_Root_UpdaterCfg(UpdaterCfgUpdaterCfg& updaterCfg, HTREEITEM parentItem)
{
    cmTreeNodeDescriptor* newItem = NULL;

    return newItem;
}

UpdaterCfgRoot* UpdaterCfgCfgManagerDlg::GetItem_Root(HTREEITEM treeItem)
{
    return &root_;
}

UpdaterCfgUpdaterCfg* UpdaterCfgCfgManagerDlg::GetItem_Root_UpdaterCfg(HTREEITEM treeItem)
{
    UpdaterCfgRoot* parentItem;
    UpdaterCfgUpdaterCfg* ret;

    parentItem = GetItem_Root(tree_.GetParentItem(treeItem));
    ret = &parentItem->GetUpdaterCfg();

    return ret;
}

UpdaterCfgTreeNodeInfo* UpdaterCfgCfgManagerDlg::GetNodeInfo(HTREEITEM treeNode)
{
    return &(node2InfoMap_.find(treeNode)->second);
}

void UpdaterCfgCfgManagerDlg::OnSize(UINT nType, int cx, int cy)
{
    cmSplitDialog::OnSize(nType, cx, cy);

}

void UpdaterCfgCfgManagerDlg::OnMnuAppendItem_Array()
{
}

void UpdaterCfgCfgManagerDlg::OnMnuInsertItem_ArrayItem()
{
}

void UpdaterCfgCfgManagerDlg::OnMnuAppendItem_ArrayItem()
{
}

void UpdaterCfgCfgManagerDlg::OnMnuRemoveItem_ArrayItem()
{
}

void UpdaterCfgCfgManagerDlg::OnMnuCreateItem_Map()
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;
    HTREEITEM selNode;

    selNode = tree_.GetSelectedItem();
    it = node2InfoMap_.find(selNode);
    if(it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        switch(nodeInfo.GetClassId()) {
        }
    }

}

void UpdaterCfgCfgManagerDlg::OnMnuRemoveItem_MapItem()
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;
    HTREEITEM selNode;

    selNode = tree_.GetSelectedItem();
    it = node2InfoMap_.find(selNode);
    if(it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        switch(nodeInfo.GetClassId()) {
        }
    }

}

void UpdaterCfgCfgManagerDlg::OnMnuCreateItem_MapItem()
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;
    HTREEITEM selNode;

    selNode = tree_.GetSelectedItem();
    it = node2InfoMap_.find(selNode);
    if(it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        switch(nodeInfo.GetClassId()) {
        }
    }

}

void UpdaterCfgCfgManagerDlg::DeleteNode(HTREEITEM treeItem)
{
    HTREEITEM nextItem;
    HTREEITEM childItem;

    childItem = tree_.GetChildItem(treeItem);
    while (childItem != NULL) {
        nextItem = tree_.GetNextItem(childItem, TVGN_NEXT);
        DeleteNode(childItem);
        childItem = nextItem;
    };

    node2InfoMap_.erase(treeItem);
    tree_.DeleteNode(treeItem);
}
void UpdaterCfgCfgManagerDlg::UpdateNodeImages(HTREEITEM node)
{
    map<HTREEITEM, UpdaterCfgTreeNodeInfo>::iterator it;
    UpdaterCfgTreeNodeInfo nodeInfo;
    UINT itemState;

    it = node2InfoMap_.find(node);
    if (it != node2InfoMap_.end()) {
        nodeInfo = it->second;
        itemState = tree_.GetItemState(node, TVIF_STATE);
        if (itemState & TVIS_EXPANDED)
            tree_.SetItemImage(node, nodeInfo.GetIconExpandedNotSelected(), nodeInfo.GetIconExpandedSelected());
        else
            tree_.SetItemImage(node, nodeInfo.GetIconCollapsedNotSelected(), nodeInfo.GetIconCollapsedSelected());
    }
}
void UpdaterCfgCfgManagerDlg::OnTreeNodeSelection(HTREEITEM oldItem, HTREEITEM newItem)
{
    UpdateNodeImages(oldItem);
    UpdateNodeImages(newItem);
    OnTreeNodeSelection();
    lastSelNode_ = newItem;
}

void UpdaterCfgCfgManagerDlg::OnTreeNodeExpansion(HTREEITEM item)
{
    UpdateNodeImages(item);
}

void UpdaterCfgCfgManagerDlg::DeleteArrayItemNode(HTREEITEM parentItem, int itemIndex, const cmString& caption)
{
    cmString buf;
    HTREEITEM toDelete;
    HTREEITEM tmp;
    cmString newCaption;
    UpdaterCfgTreeNodeInfo newNodeInfo;

    toDelete = tree_.GetChildItem(parentItem);
    for (unsigned int i = 1; i <= itemIndex; i++)
        toDelete = tree_.GetNextItem(toDelete, TVGN_NEXT);

    // renumber items
    //
    tmp = toDelete;
    while (true) {
        tmp = tree_.GetNextSiblingItem(tmp);
        if (tmp == NULL)
            break;
        GetNodeInfo(tmp)->SetArrayIndex(itemIndex);
        buf.Format(_T(" %d"), itemIndex + 1);
        newCaption = caption;
        newCaption += buf;
        tree_.SetItemText(tmp, newCaption);
        itemIndex++;
    }

    node2InfoMap_.erase(toDelete);

    tree_.DeleteNode(toDelete);
    tree_.RedrawWindow();
}

void UpdaterCfgCfgManagerDlg::DeleteArrayItemNode(HTREEITEM parentItem, vector<int>& itemIndexList, const cmString& caption)
{
    for (unsigned int i = 0; i < itemIndexList.size(); i++)
        DeleteArrayItemNode(parentItem, itemIndexList[i], caption);
}

void UpdaterCfgCfgManagerDlg::SelectArrayItemNode(HTREEITEM parentItem, int itemIndex)
{
    HTREEITEM tmp;

    tmp = tree_.GetChildItem(parentItem);
    for (int i = 1; i <= itemIndex; i++)
        tmp = tree_.GetNextItem(tmp, TVGN_NEXT);

    tree_.SelectItem(tmp);
}

void UpdaterCfgCfgManagerDlg::DeleteMapItemNode(HTREEITEM parentItem, const cmString& itemKey)
{
    HTREEITEM childNode;
    cmString newCaption;
    UpdaterCfgTreeNodeInfo newNodeInfo;

    childNode = tree_.GetChildItem(parentItem);
    while (childNode) {
        if (tree_.GetItemText(childNode) == itemKey) {
            node2InfoMap_.erase(childNode);
            tree_.DeleteNode(childNode);
            tree_.RedrawWindow();
            break;
        }
        childNode = tree_.GetNextItem(childNode, TVGN_NEXT);
    }
}

void UpdaterCfgCfgManagerDlg::DeleteMapItemNode(HTREEITEM parentItem, vector<cmString>& itemKeyList)
{
    for (unsigned int i = 0; i < itemKeyList.size(); i++)
        DeleteMapItemNode(parentItem, itemKeyList[i]);
}

void UpdaterCfgCfgManagerDlg::SelectMapItemNode(HTREEITEM parentItem, const cmString& itemKey)
{
    HTREEITEM childNode;

    childNode = tree_.GetChildItem(parentItem);
    while (childNode) {
        if (tree_.GetItemText(childNode) == itemKey) {
            tree_.SelectItem(childNode);
            break;
        }

        childNode = tree_.GetNextItem(childNode, TVGN_NEXT);
    }
}

void UpdaterCfgCfgManagerDlg::CreateAndEmbed(CWnd* parentWnd, const CRect& rc)
{
    embedded_ = true;
    Create(IDD_SPLITTERDLG_EMBED, parentWnd);
    SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
}
