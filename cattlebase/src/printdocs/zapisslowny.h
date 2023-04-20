#ifndef __ZAPISSLOWNY_H__
#define __ZAPISSLOWNY_H__

#include "../classes/types/Money.h"

class CZapisSlowny
{
public:
	void ZapisSlowny(UINT number,CString &output,bool allow0 = false);
	void ZapisSlowny(Money &value,CString &output,LPCSTR mantfix,LPCSTR fracfix);
	void ZapisSlownyList(UINT number,CStringList &out,bool allow0 = false);
private:
	int zapisslowny999(UINT x, CStringList &out,bool allow0);
	void joinStringList(CStringList &list,CString &out);

	static char tab1[][15];
	static char tab10[][20]; 
	static char tab100[][15]; 
	static char tab10_3[][12]; 
	static char tab10_6[][12]; 
	static char tab10_9[][12]; 

};



#endif
