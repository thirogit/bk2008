#ifndef __REPOSESSIONOBSERVER_H__
#define __REPOSESSIONOBSERVER_H__

class RepoSessionObserver
{
public:
	virtual void OnConnected() = 0;
	virtual void OnDisconnected() = 0;
};


#endif