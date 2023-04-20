// cmGroupHeaderCell.h: interface for the cmGroupHeaderCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMGROUPHEADERCELL_H_)
#define _CMGROUPHEADERCELL_H_

#include "cmBaseCell.h"

class cmGroupHeaderCell: public cmBaseCell  
{
public:
	cmGroupHeaderCell();
	cmGroupHeaderCell(bool createStockRenderers);
	virtual ~cmGroupHeaderCell();

protected:
	virtual void Initialize(bool createStockRenderers);
	
};

#endif // !defined(_CMGROUPHEADERCELL_H_)
