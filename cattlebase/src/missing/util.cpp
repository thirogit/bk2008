#include "stdafx.h"

#include "util.h"

CUtil::CUtil() {}

CString CUtil::GetAppDir()
{
	char    szAppPath[MAX_PATH] = "";
	CString strAppDirectory;

	::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);

	// Extract directory
	strAppDirectory = szAppPath;
	strAppDirectory = strAppDirectory.Left(strAppDirectory.ReverseFind('\\'));
	return strAppDirectory;
}

double CUtil::myatof(LPCSTR szfloatStr)
{
	CString floatStr(szfloatStr);
	floatStr.Replace(',','.');
	return atof(floatStr);
}

bool CUtil::IsDots(LPCSTR str) 
{
	if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
	return TRUE;
}


bool CUtil::DeleteDirectory(LPCSTR sPath) 
{
	HANDLE hFind;    // file handle
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	_tcscpy(DirPath,sPath);
	_tcscat(DirPath,"\\*");    // searching all files
	_tcscpy(FileName,sPath);
	_tcscat(FileName,"\\");

	// find the first file
	hFind = FindFirstFile(DirPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath,FileName);

	bool bSearch = true;
	while(bSearch) {    // until we find an entry
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName,FindFileData.cFileName);
			if((FindFileData.dwFileAttributes &
				FILE_ATTRIBUTE_DIRECTORY)) {

					// we have found a directory, recurse
					if(!DeleteDirectory(FileName)) {
						FindClose(hFind);
						return FALSE;    // directory couldn't be deleted
					}
					// remove the empty directory
					RemoveDirectory(FileName);
					_tcscpy(FileName,DirPath);
				}
			else {
				//if(FindFileData.dwFileAttributes &
				// FILE_ATTRIBUTE_READONLY)
				// change read-only file mode
				//_chmod(FileName, _S_IWRITE);
				if(!DeleteFile(FileName)) {    // delete the file
					FindClose(hFind);
					return FALSE;
				}
				_tcscpy(FileName,DirPath);
			}
		}
		else {
			// no more files there
			if(GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
			else {
				// some error occurred; close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}

		}

	}
	FindClose(hFind);                  // close the file handle

	return RemoveDirectory(sPath) == TRUE;     // remove the empty directory

}

CString CUtil::ByteSize2Str(ULONGLONG size)
{
	
	const char units[] = "BKMGTPE";
	const ULONGLONG scale = 1024;
	const char *up = units;
	ULONGLONG osize = 0;
	CString outStr;

	/*
	* Convert the input size to a round number of the appropriately
	* scaled units (saved in 'size') and a remainder (saved in 'osize').
	*/
	while (size >= scale && up < (units + sizeof (units) - 2)) {
		up++;
		osize = size;
		size = (size + (scale / 2)) / scale;
	}
	/*
	* Format the result using at most one decimal place and the unit
	* depending upon the amount of remainder (same as df -h algorithm).
	*/
	if (osize != 0 && (osize / scale) < 10)
		outStr.Format("%.1f%c", (float)osize / scale, *up);
	else if (size != 0)
		outStr.Format("%u%c", (UINT)size, *up);
	else
		outStr = "0";

	return outStr;
}
bool CUtil::InCowEANValid(LPCSTR szCowEAN)
{
	LPCSTR pEAN = NULL;
	int cksum = 0,len = 0;
	if(szCowEAN)
	{
		len = (int)strlen(szCowEAN);
		switch(len)
		{
			case 12:
				pEAN = szCowEAN;
			break;
			case 14:
				if(!isalpha(szCowEAN[0]) || !isalpha(szCowEAN[1]))
					return false;
				pEAN = szCowEAN +2;
			break;
			default:
				return false;
		}

		for(int i = 0;i < 11 ;i++)
		{
			if(pEAN[i] > 0x39 || pEAN[i] < 0x30) return false;
			cksum += (pEAN[i] - 0x30) * ((i%2) ? 1 : 3);
		}
		cksum = 10 - (cksum%10);
		if((cksum+0x30) == (int)pEAN[11]) return true;
	}
	return false;
}

bool CUtil::IsFarmNoValid(LPCSTR szFarmNo)
{
	LPCSTR pszFarmNo = NULL;
	int cksum = 0,digit,even = 0,odd = 0,evencount = 0;
	if(szFarmNo)
	{
		switch(strlen(szFarmNo))
		{
		case 12:
			pszFarmNo = szFarmNo;
			break;
		case 14:
			if(!isalpha(szFarmNo[0]) || !isalpha(szFarmNo[1]))
				return false;
			pszFarmNo = szFarmNo +2;
			break;
		default:
			return false;
		}

		for(int i = 0;i <  8;i++)
		{
			if(pszFarmNo[i] > 0x39 || pszFarmNo[i] < 0x30) return false;

			digit = pszFarmNo[i] - 0x30;
			if(digit % 2)
				odd += digit;
			else
			{
				even += digit;
				evencount++;
			}			
		}

		cksum = 23 * even + 17 * odd + evencount;
		cksum = (cksum%7);
		if((cksum+0x30) == (int)pszFarmNo[8]) return true;
	}
	return false;
}


bool CUtil::IsNumber(const CString& str)
{
	for(int i = 0;i < str.GetLength();i++)
	{
		if(!isdigit(str.GetAt(i)))
			return false;
	}
	return true;
}

bool CUtil::IsAlpha(const CString& str)
{
	for(int i = 0;i < str.GetLength();i++)
	{
		if(!isalpha(str.GetAt(i)))
			return false;
	}
	return true;
}

CString CUtil::UnPolishString(const CString &aString)
{
	const char PolishLUT[] = 
	{
		'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09',
		'\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F', '\x10', '\x11', '\x12', '\x13',
		'\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D',
		'\x1E', '\x1F', ' '   , '!'   , '"'   , '#'   , '$'   , '%'   , '&'   , '\''  ,
		'('   , ')'   , '*'   , '+'   , ','   , '-'   , '.'   , '/'   , '0'   , '1'   ,
		'2'   , '3'   , '4'   , '5'   , '6'   , '7'   , '8'   , '9'   , ':'   , ';'   ,
		'<'   , '='   , '>'   , '?'   , '@'   , 'A'   , 'B'   , 'C'   , 'D'   , 'E'   ,
		'F'   , 'G'   , 'H'   , 'I'   , 'J'   , 'K'   , 'L'   , 'M'   , 'N'   , 'O'   ,
		'P'   , 'Q'   , 'R'   , 'S'   , 'T'   , 'U'   , 'V'   , 'W'   , 'X'   , 'Y'   ,
		'Z'   , '['   , '\\'  , ']'   , '^'   , '_'   , '`'   , 'a'   , 'b'   , 'c'   ,
		'd'   , 'e'   , 'f'   , 'g'   , 'h'   , 'i'   , 'j'   , 'k'   , 'l'   , 'm'   ,
		'n'   , 'o'   , 'p'   , 'q'   , 'r'   , 's'   , 't'   , 'u'   , 'v'   , 'w'   ,
		'x'   , 'y'   , 'z'   , '{'   , '|'   , '}'   , '~'   , '\x7F', '\x80', '\x81',
		'\x82', '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B',
		'S'   , '\x8D', '\x8E', 'Z'   , '\x90', '\x91', '\x92', '\x93', '\x94', '\x95',
		'\x96', '\x97', '\x98', '\x99', '\x9A', '\x9B', 's'   , '\x9D', '\x9E', 'z'   ,
		'\xA0', '\xA1', '\xA2', 'L'   , '\xA4', 'A'   , '\xA6', '\xA7', '\xA8', '\xA9',
		'\xAA', '\xAB', '\xAC', '\xAD', '\xAE', 'Z'   , '\xB0', '\xB1', '\xB2', 'l'   ,
		'\xB4', '\xB5', '\xB6', '\xB7', '\xB8', 'a'   , '\xBA', '\xBB', '\xBC', '\xBD',
		'\xBE', 'z'   , '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', 'C'   , '\xC7',
		'\xC8', '\xC9', 'E'   , '\xCB', '\xCC', '\xCD', '\xCE', '\xCF', '\xD0', 'N'   ,
		'\xD2', 'O'   , '\xD4', '\xD5', '\xD6', '\xD7', '\xD8', '\xD9', '\xDA', '\xDB',
		'\xDC', '\xDD', '\xDE', '\xDF', '\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5',
		'c'   , '\xE7', '\xE8', '\xE9', 'e'   , '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
		'\xF0', 'n'   , '\xF2', 'o'   , '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9',
		'\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
	};

	CString aRet(aString);

	unsigned int iLetter;
	for(int i = 0,s = aRet.GetLength();i < s;i++)
	{
		iLetter = (UINT)(unsigned char)aRet[i];		
		aRet.SetAt(i,PolishLUT[iLetter]);
	}

	return aRet;
}