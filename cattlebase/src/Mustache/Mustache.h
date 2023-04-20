#ifndef __MUSTACHE_H__
#define __MUSTACHE_H__

#include <map>
#include "../PtrFlushArray.h"
#include "../missing/MapEx.h"
#include <boost/shared_ptr.hpp>



class MustacheException : public CException  
{
	DECLARE_DYNAMIC(MustacheException)
public:
	MustacheException(const MustacheException& src);
	MustacheException(const CString& sError);
	const CString& GetError() const;
	//int GetPosition() const;
	//const CString& GetMustacheString() const;
private:
	CString m_sError;
};


class MustacheStringStream
{
public:
	MustacheStringStream(const CString& str);	
	CString ReadUntil(char* stopCharacters);
	char GetChar();
	char LookAhead();
	bool IsEnd() const;
private:
	bool IsOneOf(char c,char *chs);
private:
	int m_Position;
	CString m_String;

};


class MustacheVariable
{
public:
	MustacheVariable(const MustacheVariable& src);
	MustacheVariable(const CString& sVariableName,const CString& sValue);
	const CString& GetValue() const;
	const CString& GetName() const;
private:
	CString m_sVariableValue;
	CString m_sVariableName;
};

class MustacheVariables
{	
public:
	MustacheVariables();
	MustacheVariables(const MustacheVariables& src);
	~MustacheVariables();
	void Clear();

	MustacheVariables& Put(const MustacheVariable& variable);
	const MustacheVariable* Get(const CString& sVariableName) const;
private:
	CMap<CString,const CString&,MustacheVariable*,MustacheVariable*> m_VariablesMap;
};


class MustacheSlice
{
public:
	virtual CString GetSliceValue(const MustacheVariables& variables) = 0;
	virtual ~MustacheSlice() {};
};


class MustacheVariableSlice : public MustacheSlice
{
public:
	MustacheVariableSlice(const CString& sSliceVariableName);
	virtual CString GetSliceValue(const MustacheVariables& variables);
	virtual ~MustacheVariableSlice() {};
private:
	CString m_SliceVariableName;
};

class MustacheConstantSlice : public MustacheSlice
{
public:
	MustacheConstantSlice(const CString& sSliceValue);
	virtual CString GetSliceValue(const MustacheVariables& variables);
	virtual ~MustacheConstantSlice() {};
private:
	CString m_SliceValue;
};


class MustacheSlices
{
public:
	MustacheSlices();
	~MustacheSlices();
	void AddSlice(MustacheSlice* pSlice);
	int GetSliceCount() const;
	MustacheSlice* GetSlice(int iIndex);

private:
	PtrFlushArray<MustacheSlice> m_Slices;
};

class Mustache;

class MustacheContext
{
public:
	MustacheContext();
	~MustacheContext();
	MustacheContext(const MustacheContext& src);
private:
	boost::shared_ptr<MustacheSlices> m_Slices;

	friend class Mustache;
};

class Mustache
{
public:
	static MustacheContext Parse(const CString& str);
	static CString Format(const MustacheContext& context, const MustacheVariables& variables);
};








#endif
