#include "stdafx.h"
#include "RepoAccount.h"

RepoAccount::RepoAccount(UINT Id)
{
	accountId = Id;
}

void RepoAccount::CopyFrom(const RepoAccount& src)
{
	login = src.login;
	password = src.password;
	address = src.address;
	name = src.name;
}

UINT RepoAccount::GetId() const
{
	return accountId;
}

CString RepoAccount::GetLogin() const
{
	return login;
}

CString RepoAccount::GetPassword() const
{
	return password;
}

CString RepoAccount::GetAddress() const
{
	return address;
}

CString RepoAccount::GetName() const
{
	return name;
}

void RepoAccount::SetLogin(const CString& sLogin) 
{
	login = sLogin;
}

void RepoAccount::SetPassword(const CString& sPassword) 
{
	password = sPassword;
}

void RepoAccount::SetAddress(const CString& sAddress) 
{
	address = sAddress;
}

void RepoAccount::SetName(const CString& sName) 
{
	name = sName;
}

