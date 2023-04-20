#ifndef __PASSPORTRECOGNITIONRESULT_H__
#define __PASSPORTRECOGNITIONRESULT_H__

#include <boost/shared_ptr.hpp>
#include "PassportRecognitionContext.h"
#include "RecognitionOperationResult.h"
#include "PassportRecognizer.h"

class PassportRecognitionResult
{
	friend class PassportRecognizer;
private:
	PassportRecognitionResult(const RecognitionOperationResult& operationResult,boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext);
	
	void SetPassDate(const CString& sValue);
	void SetPassNo(const CString& sValue);
	void SetCowNoBC(const CString& sValue);
	void SetCowNo(const CString& sValue);
	void SetFarmNoBC(const CString& sValue);
	void SetBirthDatePlace(const CString& sValue);
	void SetCowStock(const CString& sValue);
	void SetMotherNo(const CString& sValue);
	void SetFstOwner(const CString& sValue);
	void SetCowSex(const CString& sValue);
	void CopyFrom(const PassportRecognitionResult& src);

public:
	PassportRecognitionResult(const PassportRecognitionResult &src);
	PassportRecognitionResult& operator=(const PassportRecognitionResult &right);


	const CString& GetPassDate() const;
	const CString& GetPassNo() const;
	const CString& GetCowNoBC() const;
	const CString& GetCowNo() const;
	const CString& GetFarmNoBC() const;
	const CString& GetBirthDatePlace() const;	
	const CString& GetCowStock() const;
	const CString& GetMotherNo() const;
	const CString& GetFstOwner() const;
	const CString& GetCowSex() const;
	
	boost::shared_ptr<const PassportRecognitionContext> GetRecognitionContext() const;
	const RecognitionOperationResult& GetOperationResult() const;

private:
	CString m_passDate;
	CString m_passNo;
	CString m_cowNoBC;
	CString m_cowNo;
	CString m_farmNoBC;
	CString m_birthDatePlace;
	CString m_cowSex;
	CString m_cowStock;
	CString m_motherNo;
	CString m_fstOwner;	

	RecognitionOperationResult m_operationResult;
	boost::shared_ptr<const PassportRecognitionContext> m_ptrContext;
};



#endif 
