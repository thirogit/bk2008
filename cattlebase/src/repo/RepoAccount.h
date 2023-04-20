#ifndef __REPOACCOUNT_H__
#define __REPOACCOUNT_H__

#define NULL_ACCOUNT_ID (0)

class RepoAccount
{
public:
	RepoAccount(UINT Id);
	UINT GetId() const;
	void CopyFrom(const RepoAccount& src);

	CString GetLogin() const;
	CString GetPassword() const;
	CString GetAddress() const;
	CString GetName() const;

	void SetLogin(const CString& sLogin) ;
	void SetPassword(const CString& sPassword) ;
	void SetAddress(const CString& sAddress) ;
	void SetName(const CString& sName) ;

private:
	UINT accountId;
	CString login;
	CString password;
	CString address;
	CString name;

};
	

#endif