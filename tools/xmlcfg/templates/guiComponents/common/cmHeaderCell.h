// cmHeaderCell.h: interface for the cmHeaderCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMHEADERCELL_H_)
#define _CMHEADERCELL_H_

#include "cmBaseCell.h"

class cmHeaderCell: public cmBaseCell  
{
public:
	cmHeaderCell();
	cmHeaderCell(bool createStockRenderers);
	virtual ~cmHeaderCell();

protected:
	virtual void Initialize(bool createStockRenderers);
};

#endif // !defined(_CMHEADERCELL_H_)
