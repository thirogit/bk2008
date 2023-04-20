#include "stdafx.h"
#include "UserData.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include  "../query/equal_list.h"

UserData *UserData::m_currentUser;

UserData::UserData() : userid(0) 
{
}
void UserData::Copy(const UserData &copy)
{
	username = copy.username;
	userid  = copy.userid;
    fname = copy.fname;
	lname = copy.lname;
	pass = copy.pass;
	lastlogin = copy.lastlogin;
	privledges = copy.privledges;
	SQLUpdate = copy.SQLUpdate;
}

UserData::UserData(UserData &copy)
{
		Copy(copy);
}

UserData& UserData::operator=(const UserData& rv)
{
	Copy(rv);
	return *this;
}


UserData* UserData::GetCurrentUser()
{
	return m_currentUser;
}

void UserData::SetCurrentUser(UserData*pUser)
{
	m_currentUser = pUser;
}

bool UserData::HasCurUserPriv(PRIVLEDGES priv)
{
	ASSERT(m_currentUser);
	return m_currentUser->HasPriv(priv);
}

bool UserData::HasPriv(PRIVLEDGES priv)
{
	if(HAS_PRIV(this,ADMIN)) return true;
	return HAS_PRIV(this,priv);
}

bool UserData::InsertThisUser(bool bCommit/* = true*/)
{
	SQLQuery q;
	const char QueryStr[] =  "INSERT INTO users(username,fname,lname,pass,lastlogin,privledges)"
							 " VALUES (%0q,%1q,%2n,%3n,NULL,%4);";
	q << QueryStr;
	q.parse();
		
	SQLQueryParms p;

		p << SQLString(username) 
		  << SQLString(fname)
		  << SQLString(lname)
		  << SQLString(pass)
		  << SQLString(privledges);
	  
		LOG("InsertThisUser() query: %s",q.str(p).c_str());
		if(!SQLDB.Insert(q.str(p).c_str()))
		{
			LOG("InsertThisUser() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

		if((userid = SQLDB.SelectScalar("SELECT MAX(userid) FROM users",1)) < 0)		
		{
			SQLDB.Rollback();
			return false;
		}

		if(bCommit) SQLDB.Commit();
		return true;
}
//***************************************************************************************
bool UserData::DeleteThisUser(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM users WHERE userid  = %0";
	q.parse();
		
	LOG("DeleteThisUser() stmt: %s",q.str(userid).c_str());
	if(SQLDB.Delete(q.str(userid).c_str(),bCommit))
	{
		LOG("DeleteThisUser() error: %s",(LPCSTR)SQLDB.GetLastError());
		userid = 0;
		return true;
	}
	return false;
}
//***************************************************************************************
bool UserData::UpdateLastLogin()
{
	SQLQuery q;
	q << "UPDATE users SET lastlogin = CURRENT_TIMESTAMP WHERE userid = " << userid;
	return SQLDB.Update(q.str().c_str(),true);
}
//***************************************************************************************
bool UserData::UpdateThisUser(bool bCommit/*=true*/)
{
	
if(SQLUpdate.NeedUpdate())
{
	SQLQuery q;
	q << "UPDATE users SET "; 
	
	SQLEqualList eql;

	CHECKUPDATE2(username,eql);
	CHECKUPDATE2(fname,eql);
	CHECKUPDATE2_0LEN2NULL(lname,eql);
	CHECKUPDATE2(privledges,eql);
	CHECKUPDATE2_0LEN2NULL(pass,eql);
	
	q << eql.equal_list();

	q << " WHERE userid = " << userid;

	LOG("UpdateThisUser() stmt %s",q.str().c_str());

	if(!SQLDB.Update(q.str().c_str(),bCommit))
	{
		LOG("UpdateThisUser() error %s",(LPCSTR)SQLDB.GetLastError());
		return false;
	}

}
return true;

}
