#include "stdafx.h"
#include "PassField.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPassField::CPassField(const PassProfilePassField &passProfileField)
{
	xyOrd.SetPoint(passProfileField.GetX(),passProfileField.GetY());
	sSize.SetSize(passProfileField.GetW(),passProfileField.GetH());
}

CPassField::CPassField(const CPoint &ord,const CSize &size) : xyOrd(ord),sSize(size)
{
}

CPassField::~CPassField()
{
}

CPassField::CPassField(CPassField &copy)
{
	operator=(copy);
}

CPassField& CPassField::operator=(CPassField &right)
{
	sSize = right.sSize;
	xyOrd = right.xyOrd;
	return *this;
}
