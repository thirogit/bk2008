#ifndef __DIFFERENCEROWVIEW_H__
#define __DIFFERENCEROWVIEW_H__

class DifferenceRowView : public CStatic
{
public:
	DifferenceRowView(DifferenceList* pParent);
	void Create(const DifferenceRow* pRow);
};

#endif