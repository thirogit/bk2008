#ifndef __REPOACCOUNTTOOLBAR_H__
#define __REPOACCOUNTTOOLBAR_H__

#include "RepoToolBar.h"

class CRepoAccountToolBar : public CRepoToolBar
{	
public:

	//void EnableConnectButton(BOOL bEnable);
	//void EnableDisconnectButton(BOOL bEnable);
	void EnableAddButton(BOOL bEnable);
	void EnableDeleteButton(BOOL bEnable);
	void EnableEditButton(BOOL bEnable);


	//void SetOnConnect(boost::function<void()>& onConnectClick);
	//void SetOnDisconnect(boost::function<void()>& onDisconnectClick);
	void SetOnAdd(boost::function<void()>& onAddClick);
	void SetOnDelete(boost::function<void()>& onDeleteClick);
	void SetOnEdit(boost::function<void()>& onEditClick);

protected:
	virtual void PreSubclassWindow();

//	void OnConnect();
//	void OnDisconnect();
	void OnAdd();
	void OnDelete();
	void OnEdit();

//	boost::function<void()> m_onConnectClick;
//	boost::function<void()> m_onDisconnectClick;
	boost::function<void()> m_onAddClick;
	boost::function<void()> m_onDeleteClick;
	boost::function<void()> m_onEditClick;


	

};
#endif