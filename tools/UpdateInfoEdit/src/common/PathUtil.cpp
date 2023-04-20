#include "stdafx.h"
#include "PathUtil.h"
#include "StringPart.h"

CString CPathUtil::GetRelativePath(const CString& strPath, const CString& strReference)
{
	int iPathParts = CStringPart::StringPartCount(strPath, '\\');
	int iPart;
	int iRemain;
	CString strOut,pathPart,referencePart;


	for (iPart = 0; iPart < iPathParts; iPart++)
	{
		pathPart = CStringPart::StringPart(strPath, '\\', iPart);
		referencePart = CStringPart::StringPart(strReference, '\\', iPart);
		if (pathPart.CompareNoCase(referencePart))
		{
			// both strings differ in part# iPart
			if (iPart == 0)
			{
				// other drive must be specified entirely
				return strPath;
			}
			else
			{
				// first difference anywhere in a subfolder

				// determine number of remaining directory layers to the reference
				iRemain = CStringPart::StringPartCount(strReference, '\\') - iPart;
				if(iRemain > 0)
				{
					while (iRemain--)
						strOut += "..\\";
				}
				else
				{
				//	if(iRemain == 0)
				//		strOut += "..\\";
				//	else
						strOut += ".\\";
				}

				// differing path
				strOut += strPath.Mid(CStringPart::StringPartBegin(strPath, '\\', iPart));

				return strOut;
			}
		}
	}

	// no difference
	return CString("");

}
bool CPathUtil::IsSubPath(const CString& strPath, const CString& basePath)
{
	int iPathParts = min(CStringPart::StringPartCount(strPath, '\\'),CStringPart::StringPartCount(basePath, '\\'));
	int iPart;
	
	CString strOut,pathPart,referencePart;


	for (iPart = 0; iPart < iPathParts; iPart++)
	{
		pathPart = CStringPart::StringPart(strPath, '\\', iPart);
		referencePart = CStringPart::StringPart(basePath, '\\', iPart);
		if (pathPart.CompareNoCase(referencePart))
			return false;
	}
	return true;
}