#ifndef __USERDATA_H__
#define __USERDATA_H__

#include "../classes/types/Password.h"
#include "../classes/types/DateTime.h"
#include "UpdateData.h"
//#include "../DataArray.h"

#define HAS_PRIV(usrd,enm) ((BOOL)(usrd->privledges & (1 << enm)) != 0)
#define HAS_PRIVLEDGE(priv) (UserData::HasCurUserPriv(UserData::priv))



class UserData
{
public:

	enum SQL_UPDATE_VALUES
	{
		update_username = 0,
		update_fname,
		update_lname,
		update_pass,
		update_privledges,
		sql_update_size
	};

	enum PRIVLEDGES
	{
		ADDMODOWN = 0,
		MODFORG,
		DELOWN,
		DELFORG,
		PRINTDOC,
		ADMIN,
		INVOICE,
		PRIVS_COUNT
	};
	enum PRIVACTION 
	{
		CANDELETE = -1,
		CANEDIT = 0,
		CANADD = 1
	};



	UserData();
	virtual void Copy(const UserData &copy);
	UserData(UserData &copy);
	UserData& operator=(const UserData& rv);


	CString username;
	int userid;
	CString fname;
	CString lname;
	CPassword pass;
	CDateTime lastlogin;
	UINT privledges;	

	bool InsertThisUser(bool bCommit = true);
	bool DeleteThisUser(bool bCommit = true);
	bool UpdateLastLogin();
	bool UpdateThisUser(bool bCommit = true);

	
	
	bool HasPriv(PRIVLEDGES priv);


	typedef  UpdateData<sql_update_size,SQL_UPDATE_VALUES> UpdateUserData;

	UpdateUserData SQLUpdate;

	static UserData* GetCurrentUser();
	static void SetCurrentUser(UserData*pUser);
	static bool HasCurUserPriv(PRIVLEDGES priv);
private:
	static UserData *m_currentUser;
};




#endif