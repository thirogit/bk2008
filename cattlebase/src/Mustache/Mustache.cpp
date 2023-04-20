#include "stdafx.h"
#include "Mustache.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(MustacheException, CException)

MustacheException::MustacheException(const MustacheException& src)
{
	m_sError = src.m_sError;
}

MustacheException::MustacheException(const CString& sError) : m_sError(sError)
{
}

const CString& MustacheException::GetError() const
{
	return m_sError;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MustacheContext::MustacheContext() : m_Slices(new MustacheSlices())
{	
}

MustacheContext::MustacheContext(const MustacheContext& src)
{
	m_Slices = src.m_Slices;
}

MustacheContext::~MustacheContext()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariable::MustacheVariable(const MustacheVariable& src)
{
	m_sVariableValue = src.m_sVariableValue;
	m_sVariableName = src.m_sVariableName;
}

MustacheVariable::MustacheVariable(const CString& sVariableName,const CString& sValue) : 
	m_sVariableName(sVariableName),m_sVariableValue(sValue)
{
}

const CString& MustacheVariable::GetValue() const
{
	return m_sVariableValue;
}

const CString& MustacheVariable::GetName() const
{
	return m_sVariableName;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariables::MustacheVariables()
{
}

MustacheVariables::~MustacheVariables()
{
	Clear();
}

void MustacheVariables::Clear()
{
	CString sVariableName;
	MustacheVariable* pVariable = NULL;
	POSITION pos = m_VariablesMap.GetStartPosition();
	while(pos)
	{
		m_VariablesMap.GetNextAssoc(pos,sVariableName,pVariable);
		delete pVariable;
	}
}


MustacheVariables& MustacheVariables::Put(const MustacheVariable& variable)
{
	CString sVariableName = variable.GetName();

	MustacheVariable* pAlreadyPresentVariable = NULL;

	if(m_VariablesMap.Lookup(sVariableName,pAlreadyPresentVariable))
	{
		delete pAlreadyPresentVariable;
	}

	m_VariablesMap.SetAt(sVariableName,new MustacheVariable(variable));

	return *this;
}

const MustacheVariable* MustacheVariables::Get(const CString& sVariableName) const
{	
	MustacheVariable* pVariable = NULL;
	m_VariablesMap.Lookup(sVariableName,pVariable);
	return pVariable;
}

MustacheVariables::MustacheVariables(const MustacheVariables& src)
{
	Clear();
	CString sKey;
	MustacheVariable* pValue;

	POSITION pos = src.m_VariablesMap.GetStartPosition();
	while(pos)
	{
		src.m_VariablesMap.GetNextAssoc(pos,sKey,pValue);
		m_VariablesMap.SetAt(sKey,new MustacheVariable(*pValue));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MustacheStringStream::MustacheStringStream(const CString& str) : m_String(str),m_Position(0)
{

}

bool MustacheStringStream::IsOneOf(char c,char *chs)
{
	char* characterSet = chs;
	while(*characterSet != 0)
	{
		if(*characterSet == c)
			return true;
		characterSet++;
	}

	return false;
}

CString MustacheStringStream::ReadUntil(char* stopCharacters)
{
	CString result;
	char c;
	while(m_Position < m_String.GetLength())
	{
		c = m_String.GetAt(m_Position);
		if(!IsOneOf(c,stopCharacters))
		{
			result += c;
			m_Position++;
		}
		else
		{
			break;
		}
	}
	return result;
}


char MustacheStringStream::GetChar()
{
	if(m_Position < m_String.GetLength())
	{
		return m_String.GetAt(m_Position++);
	}
	return '\0';
}

char MustacheStringStream::LookAhead()
{
	if(m_Position+1 < m_String.GetLength())
	{
		return m_String.GetAt(m_Position);
	}
	return '\0';
}

bool MustacheStringStream::IsEnd() const
{
	return !(m_Position < m_String.GetLength());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheVariableSlice::MustacheVariableSlice(const CString& sSliceVariableName) : m_SliceVariableName(sSliceVariableName)
{
}

CString MustacheVariableSlice::GetSliceValue(const MustacheVariables& variables)
{
	const MustacheVariable* pVariable = variables.Get(m_SliceVariableName);
	if(pVariable)
		return pVariable->GetValue();

	return "###";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheConstantSlice::MustacheConstantSlice(const CString& sSliceValue) : m_SliceValue(sSliceValue)
{
}

CString MustacheConstantSlice::GetSliceValue(const MustacheVariables& variables)
{
	return m_SliceValue;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheSlices::MustacheSlices()
{
}

MustacheSlices::~MustacheSlices()
{
	m_Slices.FreeAndFlush();
}

void MustacheSlices::AddSlice(MustacheSlice* pSlice)
{
	m_Slices.Add(pSlice);
}

int MustacheSlices::GetSliceCount() const
{
	return m_Slices.GetCount();
}

MustacheSlice* MustacheSlices::GetSlice(int iIndex)
{
	return m_Slices[iIndex];
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MustacheContext Mustache::Parse(const CString& str)
{
	MustacheContext context;

	MustacheStringStream stream(str);
	char c,nextc;
	CString sVariableName,sSlice;

	while(!stream.IsEnd())
	{
		sSlice += stream.ReadUntil("\\{");
		c = stream.GetChar();
		if(c != '\0')
		{
			switch(c)
			{
				case '{':
					if(!sSlice.IsEmpty())
					{
						context.m_Slices->AddSlice(new MustacheConstantSlice(sSlice));
						sSlice.Empty();
					}

					sVariableName = stream.ReadUntil("}");
					nextc = stream.GetChar();
					if(nextc != '}')
					{
						throw new MustacheException(CTempStringLoader(IDS_MUSTACHEERRNOTCLOSEDBRACKET));
					}
					else
					{
						context.m_Slices->AddSlice(new MustacheVariableSlice(sVariableName));		
					}
				break;

				case '\\':
					nextc = stream.GetChar();
					if(nextc != '\0' && (nextc == '\\' || nextc == '{'))
					{
						sSlice += nextc;
					}
					else
					{
						throw new MustacheException(CTempStringLoader(IDS_MUSTACHEERRINVALIDESCAPE));
					}
				break;

			}
		}
		else
		{
			if(!sSlice.IsEmpty())
			{
				context.m_Slices->AddSlice(new MustacheConstantSlice(sSlice));					
				sSlice.Empty();
			}

		}
	}

	return context;

}

CString Mustache::Format(const MustacheContext& context, const MustacheVariables& variables)
{
	CString sResult;

	MustacheSlice* pSlice = NULL;
	for(int iSlice = 0,sliceCount = context.m_Slices->GetSliceCount();iSlice<sliceCount;iSlice++)
	{
		pSlice = context.m_Slices->GetSlice(iSlice);
		sResult += pSlice->GetSliceValue(variables);
	}

	return sResult;
}
