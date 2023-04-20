#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include "../../DatabasePool/fbserver/DbConnection.h"
#include <boost/shared_ptr.hpp>

class BaseDAO;
class SQLSelect;

class TransactionObject
{
protected:
	TransactionObject();	
public:
	bool Delete(const char *stmt,bool commit = false);
	bool Insert(const char *stmt,bool commit = false);
	bool Update(const char *stmt,bool commit = false);
	void Commit();
	void Rollback();
	~TransactionObject();
protected:
	DbConnection* GetConnection();


	DbConnection* m_pDbConnection;

	friend class BaseDAO;
	friend class SQLSelect;
};


typedef boost::shared_ptr<TransactionObject> Transaction;

#endif