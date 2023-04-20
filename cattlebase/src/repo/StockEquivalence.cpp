#include "stdafx.h"
#include "StockEquivalence.h"

StockEquivalence::StockEquivalence(RepoStock* pLocalStock,RepoStock* pRemoteStock) : TypedEquivalence<RepoStock>(pLocalStock,pRemoteStock)
{	
}


bool StockEquivalence::IsEqual()
{
	const UINT comparedMembers[] = 
	{
		REPOSTOCK_MEMBER_NAME,	
		REPOSTOCK_MEMBER_CODE,
		REPOSTOCK_MEMBER_MYPRICE,
		REPOSTOCK_MEMBER_PREDEFSEX,
		0
	};

	RepoStock* pLocalStock = GetLocal(),*pRemoteStock = GetRemote(); 
	if(pLocalStock != NULL && pRemoteStock != NULL)
	{
		const UINT* pMemberToCompare = comparedMembers;
		while(*pMemberToCompare)
		{
			if(m_Comparator.CompareBy(*pMemberToCompare,pLocalStock,pRemoteStock) != COMPARE_EQUAL)
			{
				return false;
			}
			pMemberToCompare++;
		}
		return true;
	}
	
	return false;
}

