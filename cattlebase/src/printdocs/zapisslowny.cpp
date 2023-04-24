#include "stdafx.h"
#include "zapisslowny.h"


char CZapisSlowny::tab1[][15] = 
{
     "zero",
     "jeden",
     "dwa",
     "trzy",
     "cztery",
     "pi��",
     "sze��",
     "siedem",
     "osiem",
     "dziewie�",
     "dzie�i��",
     "jedena�cie",
     "dwana�cie",
     "trzyna�cie",
     "czterna�cie",
     "pi�tna�cie",
     "szesna�cie",
     "siedemna�cie",
     "osiemna�cie",
     "dziewi�tna�cie"     
};

char CZapisSlowny::tab10[][20] = 
{
     "",
     "",
     "dwadzie�cia",
     "trzydzie�ci",
     "czterdzie�ci",
     "pi��dziesi�t",
     "sze��dziesi�t",
     "siedemdziesi�t",
     "osiemdziesi�t", 
     "dziewi��dziesi�t"
};

char CZapisSlowny::tab100[][15] = 
{
   "",
   "sto",
   "dwie�cie",
   "trzysta",
   "czterysta",
   "pi��set",
   "sze��set",
   "siedemset",
   "osiemset",
   "dziewi��set"
 };
 
char CZapisSlowny::tab10_3[][12] = 
 {
  "tysi�c",
  "tysi�ce",
  "tysi�cy",
  };
char CZapisSlowny::tab10_6[][12] = 
{
     "milion",
     "miliony",
     "milion�w"
};

char CZapisSlowny::tab10_9[][12] = 
{
     "miliard",
     "miliardy",
     "miliard�w"
};
 
int CZapisSlowny::zapisslowny999(UINT x, CStringList &out,bool allow0)
{
   if(x >= 1000) return -1;
	UINT a,bc,b,c;
	int level = 0;
    a = x / 100;
    bc = x %100;
	if(a > 0) out.AddTail(tab100[a]);
   
    b = bc/10;
    c = bc%10;
    
    if(bc > 0 || allow0)
  {
    
         if(b > 1)
          {
            out.AddTail(tab10[b]);
             if(c > 0) out.AddTail(tab1[c]); //c > 0
          }
          else out.AddTail(tab1[bc]);
    }
    
    if(x == 1) return 0;
    else
    {
        //if(c == 1) return 2;
        if(b !=  1 && c >= 2 && c <= 4) return 1;
    }
    
    return 2;
    
}
/*
void cyfrowo(UINT x,CStringList &out)
{
	int i,len;
	CString s;
	s.Format("%u",x);

	len = s.GetLength();
	for(i=0;i<len;i++)
		out.AddHead(tab1[s.GetAt(i)-'0']);
	
}*/
void CZapisSlowny::ZapisSlowny(UINT number,CString &output,bool allow0/* = false*/)
{
	CStringList out;
	ZapisSlownyList(number,out,allow0);
	joinStringList(out,output);
}


void CZapisSlowny::ZapisSlownyList(UINT number,CStringList &out,bool allow0/* = false*/)
{
   UINT a,b,c,d,tb,tc;
  int l;
  
  a = number / 1000000000; //ile miliardow
  tb = (number % 1000000000);
  b = tb/1000000; //ile milionow
  tc = tb%1000000;
  c = tc/1000; // ile tysiecy
  d = tc%1000;
    
if(a > 0)  
{
	l = zapisslowny999(a,out,allow0);
	if(l >= 0)	out.AddTail(tab10_9[l]);
}
if(b > 0)
{
	l = zapisslowny999(b,out,allow0);
	if(l >= 0) out.AddTail(tab10_6[l]);
}

if(c > 0) 
{ 
	l = zapisslowny999(c,out,allow0);
	if(l >= 0) out.AddTail(tab10_3[l]);
}

zapisslowny999(d,out,allow0);
     
}

void CZapisSlowny::ZapisSlowny(Money& value,CString &output,LPCSTR mantfix,LPCSTR fracfix)
{
	CStringList out;

	
	ZapisSlownyList(int(value.MoneyInt()),out);
	if(mantfix) out.AddTail(mantfix);
	ZapisSlownyList(int(value.MoneyCents()),out,true);
	if(fracfix) out.AddTail(fracfix);
	joinStringList(out,output);
}


void CZapisSlowny::joinStringList(CStringList &list,CString &out)
{
	out.Empty();
	POSITION pos = list.GetHeadPosition();
	int i = list.GetSize()-1,j = 0;
	char separator = ' ';
	while(pos)
	{
		out += list.GetNext(pos);
		if(j != i) out += separator;
	}
}




