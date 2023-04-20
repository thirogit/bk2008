#include "stdafx.h"
#include "PassportRecognitionResult.h"


PassportRecognitionResult::PassportRecognitionResult(const RecognitionOperationResult& operationResult,
						  boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext) :
							m_operationResult(operationResult),m_ptrContext(ptrRecognitionContext)
{
}

PassportRecognitionResult::PassportRecognitionResult(const PassportRecognitionResult &src) : 
								m_operationResult(RecognitionStatus_Failure)
{
	CopyFrom(src);
}

PassportRecognitionResult& PassportRecognitionResult::operator=(const PassportRecognitionResult &right)
{
	CopyFrom(right);
	return *this;
}

void PassportRecognitionResult::CopyFrom(const PassportRecognitionResult& src)
{
	m_passDate = src.m_passDate;
	m_passNo = src.m_passNo;
	m_cowNoBC = src.m_cowNoBC;
	m_cowNo = src.m_cowNo;
	m_farmNoBC = src.m_farmNoBC;
	m_birthDatePlace = src.m_birthDatePlace;
	m_cowSex = src.m_cowSex;
	m_cowStock = src.m_cowStock;
	m_motherNo = src.m_motherNo;
	m_fstOwner = src.m_fstOwner;
	m_operationResult = src.m_operationResult;
	m_ptrContext = src.m_ptrContext;
}


void PassportRecognitionResult::SetPassDate(const CString& sValue)
{
	m_passDate = sValue;
}

void PassportRecognitionResult::SetPassNo(const CString& sValue)
{
	m_passNo = sValue;
}

void PassportRecognitionResult::SetCowNoBC(const CString& sValue)
{
	m_cowNoBC = sValue;
}

void PassportRecognitionResult::SetCowNo(const CString& sValue)
{
	m_cowNo = sValue;
}

void PassportRecognitionResult::SetFarmNoBC(const CString& sValue)
{
	m_farmNoBC = sValue;
}

void PassportRecognitionResult::SetBirthDatePlace(const CString& sValue)
{
	m_birthDatePlace = sValue;
}

void PassportRecognitionResult::SetCowStock(const CString& sValue)
{
	m_cowStock  = sValue;
}

void PassportRecognitionResult::SetMotherNo(const CString& sValue)
{
	m_motherNo = sValue;
}

void PassportRecognitionResult::SetFstOwner(const CString& sValue)
{
	m_fstOwner = sValue;
}

void PassportRecognitionResult::SetCowSex(const CString& sValue)
{
	m_cowSex = sValue;
}

const CString& PassportRecognitionResult::GetPassDate() const
{
	return m_passDate;
}

const CString& PassportRecognitionResult::GetPassNo() const
{
	return m_passNo;
}

const CString& PassportRecognitionResult::GetCowNoBC() const
{
	return m_cowNoBC;
}

const CString& PassportRecognitionResult::GetCowNo() const
{
	return m_cowNo;
}

const CString& PassportRecognitionResult::GetFarmNoBC() const
{
	return m_farmNoBC;
}

const CString& PassportRecognitionResult::GetBirthDatePlace() const
{
	return m_birthDatePlace;
}

const CString& PassportRecognitionResult::GetCowStock() const
{
	return m_cowStock;
}

const CString& PassportRecognitionResult::GetMotherNo() const
{
	return m_motherNo;
}

const CString& PassportRecognitionResult::GetFstOwner() const
{
	return m_fstOwner;
}

const CString& PassportRecognitionResult::GetCowSex() const
{
	return m_cowSex;
}

boost::shared_ptr<const PassportRecognitionContext> PassportRecognitionResult::GetRecognitionContext() const
{
	return m_ptrContext;
}

const RecognitionOperationResult& PassportRecognitionResult::GetOperationResult() const
{
	return m_operationResult;
}
 