#ifndef __PASSFILED_H__
#define __PASSFILED_H__

#include "../../passOcrProfile/include/PassProfilePassField.h"

class CPassField
{
public:
	CPassField(const PassProfilePassField &passProfileField);
	CPassField(const CPoint &ord,const CSize &size);
	~CPassField();
	CPassField(CPassField &copy);
	CPassField& operator=(CPassField &right);

public:
	CPoint xyOrd; //in mm
	CSize  sSize; //in mm
};



#endif // __PASSFILED_H__
