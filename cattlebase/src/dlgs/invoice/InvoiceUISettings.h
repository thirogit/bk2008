#ifndef __INVOICEUISETTINGS_H__
#define __INVOICEUISETTINGS_H__

#include "../../grids/GridHeaders.h"

class InvoiceUISettings
{
public:
	InvoiceUISettings(UINT nListCfgItem,const GRIDHEADERS *ppHeaders);

	UINT GetListCfgItem() const;
	const GRIDHEADERS* GetListHeaderColumns() const;	
protected:
	UINT				m_nListCfgItem;
	const GRIDHEADERS*	m_pListHeaderColumns;
	
};

#endif