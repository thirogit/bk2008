// cmRendererSet.cpp: implementation of the cmRendererSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmRendererSet.h"
#include "cmCellBaseRenderer.h"
#include "cmBaseCell.h"
#include "cmCellFrRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmRendererSet::cmRendererSet()
{

}

cmRendererSet::~cmRendererSet()
{

}

void cmRendererSet::AppendRenderer(cmCellBaseRenderer* renderer)
{
	list_.push_back(renderer);
}

void cmRendererSet::Draw(CDC* pDC, cmBaseCell* cell)
{
	for (int i = 0; i < list_.size(); i++)
		list_[i]->Draw(pDC, cell);
}

cmFontInfo cmRendererSet::GetTopFontInfo()
{
	cmFontInfo ret;

	if (list_.size()) {
		for (int i = list_.size() - 1; i >= 0; i--) {
			cmCellFrRenderer* r;

			r = dynamic_cast<cmCellFrRenderer*>(list_[i]);
			if (r) {
				ret = r->GetFontInfo();
				break;
			}
		}
	}

	return ret;
}

void cmRendererSet::CleanUp(bool deleteRenderers)
{
	if (deleteRenderers) {
		for (int i = 0; i < list_.size(); i++)
			delete list_[i];
	}

	list_.clear();
}