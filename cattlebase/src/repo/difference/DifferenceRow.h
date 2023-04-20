#ifndef __DIFFERENCEROW_H__
#define __DIFFERENCEROW_H__

class DifferenceRow : public CStatic
{
public:
	virtual CString GetName() const = 0;
	virtual CString GetLeft() const = 0;
	virtual CString GetRight() const = 0;
	virtual bool IsEqual() const = 0;
protected:
	

};

#endif