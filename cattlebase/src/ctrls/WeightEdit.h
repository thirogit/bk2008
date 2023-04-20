#ifndef __WEIGHTEDIT_H__
#define __WEIGHTEDIT_H__

#include "../classes/types/Weight.h"
#include "../FilterEdit/FilterEdit.h"

class CWeightEdit : public CFilterEdit
{
public :
	CWeightEdit();
	NullDouble GetWeight() const;
	void SetWeight(const NullDouble& weight);

};


#endif