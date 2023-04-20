#include "stdafx.h"
#include "RepoAccountToolBar.h"
#include "../../image/Png.h"
#include <boost/bind.hpp>

void CRepoAccountToolBar::PreSubclassWindow()
{
	CRepoToolBar::PreSubclassWindow();

	CPng editImg,addImg,deleteImg;

	editImg.LoadPNG(IDPNG_REPOACCOUNT_EDIT);	
	addImg.LoadPNG(IDPNG_REPOACCOUNT_ADD);
	deleteImg.LoadPNG(IDPNG_REPOACCOUNT_DELETE);

//	boost::function<void()> onConnectFn = boost::bind(&CRepoAccountToolBar::OnConnect,this);
//	boost::function<void()> onDisconnectFn = boost::bind(&CRepoAccountToolBar::OnDisconnect,this);
	boost::function<void()> onAddFn = boost::bind(&CRepoAccountToolBar::OnAdd,this);
	boost::function<void()> onDeleteFn = boost::bind(&CRepoAccountToolBar::OnDelete,this);
	boost::function<void()> onEditFn = boost::bind(&CRepoAccountToolBar::OnEdit,this);
	
	COLORREF bkgColor = ::GetSysColor(COLOR_3DFACE);
//	AddButtonNoReToolBar(onConnectFn,connectImg.GetBitmap24(bkgColor),true);
//	AddButtonNoReToolBar(onDisconnectFn,disconnectImg.GetBitmap24(bkgColor),true);
	AddButtonNoReToolBar(onAddFn,addImg.GetBitmap24(bkgColor),true);
	AddButtonNoReToolBar(onDeleteFn,deleteImg.GetBitmap24(bkgColor),true);
	AddButtonNoReToolBar(onEditFn,editImg.GetBitmap24(bkgColor),true);
	ReButtonToolBar();
}

//void CRepoAccountToolBar::EnableConnectButton(BOOL bEnable)
//{
//	EnableButton(0,bEnable);
//}
//
//void CRepoAccountToolBar::EnableDisconnectButton(BOOL bEnable)
//{
//	EnableButton(1,bEnable);
//}

void CRepoAccountToolBar::EnableAddButton(BOOL bEnable)
{
	EnableButton(0,bEnable);
}

void CRepoAccountToolBar::EnableDeleteButton(BOOL bEnable)
{
	EnableButton(1,bEnable);
}

void CRepoAccountToolBar::EnableEditButton(BOOL bEnable)
{
	EnableButton(2,bEnable);
}
//
//void CRepoAccountToolBar::SetOnConnect(boost::function<void()>& onConnectClick)
//{
//	m_onConnectClick = onConnectClick;
//}
//
//void CRepoAccountToolBar::SetOnDisconnect(boost::function<void()>& onDisconnectClick)
//{
//	m_onDisconnectClick = onDisconnectClick;
//}

void CRepoAccountToolBar::SetOnAdd(boost::function<void()>& onAddClick)
{
	m_onAddClick = onAddClick;
}

void CRepoAccountToolBar::SetOnDelete(boost::function<void()>& onDeleteClick)
{
	m_onDeleteClick = onDeleteClick;
}

void CRepoAccountToolBar::SetOnEdit(boost::function<void()>& onEditClick)
{
	m_onEditClick = onEditClick;
}
//
//
//void CRepoAccountToolBar::OnConnect()
//{
//	m_onConnectClick();
//}
//
//void CRepoAccountToolBar::OnDisconnect()
//{
//	m_onDisconnectClick();
//}
//
void CRepoAccountToolBar::OnAdd()
{
	m_onAddClick();
}

void CRepoAccountToolBar::OnDelete()
{
	m_onDeleteClick();
}

void CRepoAccountToolBar::OnEdit()
{
	m_onEditClick();
}



