#ifndef __FORWARDREPOSESSIONOBSERVER_H__
#define __FORWARDREPOSESSIONOBSERVER_H__

#include "RepoSessionObserver.h"
#include <boost/function.hpp>

class ForwardRepoSessionObserver : public RepoSessionObserver
{
public:

	void SetOnDisconnect(boost::function<void ()> &onDisconnect)
	{
		m_onDisconnect = onDisconnect;
	}
	
	void SetOnConnected(boost::function<void ()> &onConnected)
	{
		m_onConnected = onConnected;
	}

	virtual void OnConnected()
	{
		m_onConnected();
	}
	virtual void OnDisconnected()
	{
		m_onDisconnect();
		
	}
private:
	boost::function<void ()> m_onDisconnect;
	boost::function<void ()> m_onConnected;
};

#endif