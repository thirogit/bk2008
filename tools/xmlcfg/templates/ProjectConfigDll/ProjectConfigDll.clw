; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=1
ResourceCount=10
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_CFG_MGR_TEMPLATE
Resource2=IDR_MNU_EDIT_ARRAY_ITEM
Resource3=IDR_MNU_EDIT_MAP
Resource4=IDR_MNU_EDIT_ARRAY
Resource5=IDD_INPUT_BOX
Resource6=IDD_CFG_TEMPLATE
Resource7=IDD_SPLITTERDLG_EMBED
Class1=CInputBox
LastClass=CInputBox
LastTemplate=CDialog
Resource8=IDD_CONTAINER_TEMPLATE
Resource9=IDD_SPLITTERDLG
Resource10=IDR_MNU_EDIT_MAP_ITEM

[DLG:IDD_CFG_MGR_TEMPLATE]
Type=1
Class=?
ControlCount=0

[DLG:IDD_CFG_TEMPLATE]
Type=1
Class=?
ControlCount=0

[MNU:IDR_MNU_EDIT_ARRAY]
Type=1
Class=?
Command1=ID_EDITARRAY_APPENDITEM
CommandCount=1

[MNU:IDR_MNU_EDIT_ARRAY_ITEM]
Type=1
Class=?
Command1=ID_EDITARRAYITEM_INSERTITEM
Command2=ID_EDITARRAYITEM_APPENDITEM
Command3=ID_EDITARRAYITEM_REMOVEITEM
CommandCount=3

[MNU:IDR_MNU_EDIT_MAP]
Type=1
Class=?
Command1=ID_EDITMAP_CREATEITEM
CommandCount=1

[MNU:IDR_MNU_EDIT_MAP_ITEM]
Type=1
Class=?
Command1=ID_EDITMAPITEM_CREATEITEM
Command2=ID_EDITMAPITEM_REMOVEITEM
CommandCount=2

[DLG:IDD_SPLITTERDLG]
Type=1
Class=?
ControlCount=3
Control1=IDC_TREE,SysTreeView32,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDOK,button,1342242816

[DLG:IDD_INPUT_BOX]
Type=1
Class=CInputBox
ControlCount=4
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_TXT_VALUE,edit,1073807488
Control4=IDC_LBL_CAPTION,static,1342308352

[CLS:CInputBox]
Type=0
HeaderFile=InputBox.h
ImplementationFile=InputBox.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInputBox

[DLG:IDD_CONTAINER_TEMPLATE]
Type=1
Class=?
ControlCount=5
Control1=IDC_CMD_INSERT,button,1342242816
Control2=IDC_CMD_DELETE,button,1342242816
Control3=IDC_CMD_APPEND,button,1342242816
Control4=IDC_CMD_EDIT,button,1342242816
Control5=IDC_ITEMLIST,SysListView32,1342259209

[DLG:IDD_SPLITTERDLG_EMBED]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1073807361
Control2=IDCANCEL,button,1073807360
Control3=IDC_TREE,SysTreeView32,1342242816

