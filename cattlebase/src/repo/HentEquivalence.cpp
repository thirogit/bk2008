#include "stdafx.h"
#include "HentEquivalence.h"

HentEquivalence::HentEquivalence(RepoHent* pLocalHent,RepoHent* pRemoteHent) : TypedEquivalence<RepoHent>(pLocalHent,pRemoteHent)
{
	
}

bool HentEquivalence::IsEqual()
{
	const UINT comparedMembers[] = 
	{
		REPOHENT_MEMBER_NAME,	
		REPOHENT_MEMBER_ALIAS,
		REPOHENT_MEMBER_STREET,
		REPOHENT_MEMBER_POBOX,
		REPOHENT_MEMBER_CITY,
		REPOHENT_MEMBER_ZIP,
		REPOHENT_MEMBER_PHONE,
		REPOHENT_MEMBER_NIP,
		REPOHENT_MEMBER_FARMNO,
		REPOHENT_MEMBER_WETNO,
		REPOHENT_MEMBER_EXTRAS,
		REPOHENT_MEMBER_PLATE,
		REPOHENT_MEMBER_HENTTYPE,
		REPOHENT_MEMBER_PESEL,
		REPOHENT_MEMBER_REGON,
		REPOHENT_MEMBER_IDNO,
		REPOHENT_MEMBER_ISSUEDATE,
		REPOHENT_MEMBER_ISSUEPOST,
		REPOHENT_MEMBER_ACCOUNTNO,
		REPOHENT_MEMBER_BANKNAME,
		REPOHENT_MEMBER_WETLICENCENO,
		REPOHENT_MEMBER_CELLPHONENO,
		REPOHENT_MEMBER_EMAILADDRESS,
		REPOHENT_MEMBER_LATITUDE,
		REPOHENT_MEMBER_LONGITUDE,
		0
	};

	RepoHent* pLocalHent = GetLocal(),*pRemoteHent = GetRemote(); 
	if(pLocalHent != NULL && pRemoteHent != NULL)
	{
		const UINT* pMemberToCompare = comparedMembers;
		while(*pMemberToCompare)
		{
			if(m_HentComparator.CompareBy(*pMemberToCompare,pLocalHent,pRemoteHent) != COMPARE_EQUAL)
			{
				return false;
			}
			pMemberToCompare++;
		}
		return true;
	}
	
	return false;
}

