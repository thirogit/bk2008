#include "stdafx.h"
#include "InvoiceUISettings.h"



InvoiceUISettings::InvoiceUISettings(UINT nListCfgItem,const GRIDHEADERS *ppHeaders) : m_nListCfgItem(nListCfgItem),m_pListHeaderColumns(ppHeaders)
{
}

UINT InvoiceUISettings::GetListCfgItem() const
{
	return m_nListCfgItem;
}

const GRIDHEADERS* InvoiceUISettings::GetListHeaderColumns() const
{
	return m_pListHeaderColumns;
}

