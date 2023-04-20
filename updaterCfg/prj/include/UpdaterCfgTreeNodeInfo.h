// UpdaterCfgTreeNodeInfo.h
// 
// generated on 2009.10.30 at 17:39:29.484
//
#if !defined(_UPDATERCFGTREENODEINFO_H_)
#define _UPDATERCFGTREENODEINFO_H_

#include "stdafx.h"
#include "cmString.h"
#include "XmlSerializable.h"

class UpdaterCfgTreeNodeInfo
{
public:
    UpdaterCfgTreeNodeInfo();
    UpdaterCfgTreeNodeInfo(const UpdaterCfgTreeNodeInfo& copy);
    virtual ~UpdaterCfgTreeNodeInfo();

    // Get/Set methods
    //
public:

    void SetTreeItem(HTREEITEM item);
    HTREEITEM GetTreeItem();

    void SetClassId(DWORD classId);
    DWORD GetClassId();

    void SetArrayIndex(int index);
    int GetArrayIndex();

    void SetMapKey(const cmString& key);
    cmString GetMapKey();

    void SetIconExpandedSelected(int iconIndex);
    int GetIconExpandedSelected() const;

    void SetIconExpandedNotSelected(int iconIndex);
    int GetIconExpandedNotSelected() const;

    void SetIconCollapsedSelected(int iconIndex);
    int GetIconCollapsedSelected() const;

    void SetIconCollapsedNotSelected(int iconIndex);
    int GetIconCollapsedNotSelected() const;

protected:
    void Copy(const UpdaterCfgTreeNodeInfo& nodeInfo);

    HTREEITEM treeItem_;
    DWORD classId_;
    int arrayIndex_;
    cmString mapKey_;
    int iconExpandedSelected_;
    int iconExpandedNotSelected_;
    int iconCollapsedSelected_;
    int iconCollapsedNotSelected_;


};

#endif //!defined(_UPDATERCFGTREENODEINFO_H_)

