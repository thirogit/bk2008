// cmCellBaseRenderer.cpp: implementation of the cmCellBaseRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellBaseRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmCellBaseRenderer::cmCellBaseRenderer()
{

}

cmCellBaseRenderer::cmCellBaseRenderer(const cmCellBaseRenderer& rend)
{
	Copy(rend);
}

cmCellBaseRenderer::~cmCellBaseRenderer()
{

}

cmCellBaseRenderer& cmCellBaseRenderer::operator=(const cmCellBaseRenderer& rend)
{
	return Copy(rend);
}

cmCellBaseRenderer& cmCellBaseRenderer::Copy(const cmCellBaseRenderer& rend)
{
	fontInfo_ = rend.fontInfo_;

	return *this;
}
