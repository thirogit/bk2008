#include "stdafx.h"
#include "AccountNoEdit.h"
#include "global_arrays.h"



CAccountNoEdit::CAccountNoEdit() : CTipEdit("\\P\\L## #### #### #### #### #### ####")
{
	
}

bool CAccountNoEdit::IsValidAccountNo()
{
	int i,mod;
	char szIBAN[31],szIBANpart[9];
	CString data;
	char iChar;

	if(IsInputEmpty()) return true;

	data = GetInputData();
	memset(szIBAN,0,sizeof(szIBAN));
	
	for(i = 0;i < 26;i++)
	{
		 iChar = data.GetAt(i);
		 if(iChar > '9' || iChar < '0')
		 		return false;
	}

	char *szDataPtr = (char*)(LPCSTR)data;
	sprintf(szIBAN,"%.24s%d%d%.2s",&szDataPtr[2],'P'-55,'L'-55,szDataPtr);

	
	for(i=0,mod=0;i<30;i+=6)
	{
		sprintf(szIBANpart,"%d%.6s",mod,&szIBAN[i]);
		mod = atoi(szIBANpart)%97;
    }

	return (mod == 1);
}


BOOL CAccountNoEdit::OnValidate()
{
	if(!COXMaskedEdit::OnValidate()) return FALSE;

	if(!IsValidAccountNo())
	{
		DisplayToolTip(true);
		return FALSE;
	}
	return TRUE;
}
void CAccountNoEdit::PrepareToolTip(CWnd *pParent)
{
	CreateToolTip(pParent,"Uwaga! Program przyjmuje tylko polskie\nnumery kont bankowych",2,"B³êdny numer konta");
}

