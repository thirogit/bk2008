// UpdaterCfgTreeNodeInfo.cpp
// 
// generated on 2009.10.30 at 17:39:29.484
//
#include "stdafx.h"
#include "UpdaterCfgTreeNodeInfo.h"

UpdaterCfgTreeNodeInfo::UpdaterCfgTreeNodeInfo():
    treeItem_(NULL),
    classId_(0),
    arrayIndex_(-1),
    iconExpandedSelected_(-1),
    iconExpandedNotSelected_(-1),
    iconCollapsedSelected_(-1),
    iconCollapsedNotSelected_(-1)
{
}

UpdaterCfgTreeNodeInfo::UpdaterCfgTreeNodeInfo(const UpdaterCfgTreeNodeInfo& updaterCfgTreeNodeInfo)
{
    Copy(updaterCfgTreeNodeInfo);
}

UpdaterCfgTreeNodeInfo::~UpdaterCfgTreeNodeInfo()
{
}

void UpdaterCfgTreeNodeInfo::SetTreeItem(HTREEITEM item)
{
    treeItem_ = item;
}

HTREEITEM UpdaterCfgTreeNodeInfo::GetTreeItem()
{
    return treeItem_;
}

void UpdaterCfgTreeNodeInfo::SetClassId(DWORD classId)
{
    classId_ = classId;
}

DWORD UpdaterCfgTreeNodeInfo::GetClassId()
{
    return classId_;
}

void UpdaterCfgTreeNodeInfo::SetArrayIndex(int index)
{
    arrayIndex_ = index;
}

int UpdaterCfgTreeNodeInfo::GetArrayIndex()
{
    return arrayIndex_;
}

void UpdaterCfgTreeNodeInfo::SetMapKey(const cmString& key)
{
    mapKey_ = key;
}

cmString UpdaterCfgTreeNodeInfo::GetMapKey()
{
    return mapKey_;
}


void UpdaterCfgTreeNodeInfo::Copy(const UpdaterCfgTreeNodeInfo& nodeInfo)
{
    treeItem_ = nodeInfo.treeItem_;
    classId_ = nodeInfo.classId_;
    arrayIndex_ = nodeInfo.arrayIndex_;
    mapKey_ = nodeInfo.mapKey_;
    iconExpandedSelected_ = nodeInfo.iconExpandedSelected_;
    iconExpandedNotSelected_ = nodeInfo.iconExpandedNotSelected_;
    iconCollapsedSelected_ = nodeInfo.iconCollapsedSelected_;
    iconCollapsedNotSelected_ = nodeInfo.iconCollapsedNotSelected_;
}

void UpdaterCfgTreeNodeInfo::SetIconExpandedSelected(int iconIndex)
{
    iconExpandedSelected_ = iconIndex;
}

int UpdaterCfgTreeNodeInfo::GetIconExpandedSelected() const
{
    return iconExpandedSelected_;
}

void UpdaterCfgTreeNodeInfo::SetIconExpandedNotSelected(int iconIndex)
{
    iconExpandedNotSelected_ = iconIndex;
}

int UpdaterCfgTreeNodeInfo::GetIconExpandedNotSelected() const
{
    return iconExpandedNotSelected_;
}

void UpdaterCfgTreeNodeInfo::SetIconCollapsedSelected(int iconIndex)
{
    iconCollapsedSelected_ = iconIndex;
}

int UpdaterCfgTreeNodeInfo::GetIconCollapsedSelected() const
{
    return iconCollapsedSelected_;
}

void UpdaterCfgTreeNodeInfo::SetIconCollapsedNotSelected(int iconIndex)
{
    iconCollapsedNotSelected_ = iconIndex;
}

int UpdaterCfgTreeNodeInfo::GetIconCollapsedNotSelected() const
{
    return iconCollapsedNotSelected_;
}

