#ifndef __HENTTYPE_H__
#define __HENTTYPE_H__


#define HTYPE_COMPANY 1
#define HTYPE_INDIVIDUAL 2

#define HTYPE_VAT HTYPE_COMPANY
#define HTYPE_RR HTYPE_INDIVIDUAL

class HentType
{
public:
	HentType();
	bool IsIndividual()	;
	bool IsCompany();
	
	CString henttypename;
	int henttypeid;
};






#endif