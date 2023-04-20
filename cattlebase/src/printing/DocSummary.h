#ifndef __DOCSUMMARY_H__
#define __DOCSUMMARY_H__

#include <objidl.h>

#include "PropertySet.h"

extern const OLECHAR szSummInfo[];

DEFINE_GUID( FMTID_SummaryInformation, 0xF29F85E0, 0x4FF9, 0x1068,
	0xAB, 0x91, 0x08, 0x00, 0x2B, 0x27, 0xB3, 0xD9 );

class CDocSummary
{
public:
	CDocSummary();
	BOOL SetAuthor(LPCTSTR szAuthor);
	CString GetAuthor();
	BOOL SetComments(LPCTSTR szComments);
	CString GetComments();
	BOOL IncrRevNum();
	CString GetRevNum();
	BOOL RecordCreateDate();
	CString GetCreateDate();
	BOOL RecordSaveDate();
	CString GetLastSaveDate();
	BOOL WriteToStorage(LPSTORAGE lpRootStg);
	BOOL ReadFromStorage(LPSTORAGE lpRootStg);
protected:
	CDocPropertySet m_propSet;
	CDocPropertySection* m_pSection;
};


#endif //__DOCSUMMARY_H__