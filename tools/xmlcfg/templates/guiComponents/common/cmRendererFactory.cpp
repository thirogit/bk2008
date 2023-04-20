// cmRendererFactory.cpp: implementation of the cmRendererFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmRendererFactory.h"
#include "cmCellBaseRenderer.h"
#include "cmRenderer_CaptionCell.h"
#include "cmRenderer_GradientCell.h"
#include "cmRenderer_GlassCell.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmRendererFactory::cmRendererFactory()
{

}

cmRendererFactory::~cmRendererFactory()
{
	CleanUp();
}

void cmRendererFactory::CleanUp()
{
	set<cmCellBaseRenderer*>::iterator it;
		
	for (it = renderers_.begin(); it != renderers_.end(); it++)
		delete *it;
	
	renderers_.clear();
}

void cmRendererFactory::ReleaseRenderer(cmCellBaseRenderer* rend)
{
	set<cmCellBaseRenderer*>::iterator it;

	it = renderers_.find(rend);
	assert(it != renderers_.end());

	delete *it;

	renderers_.erase(it);
}

cmRenderer_CaptionCell* cmRendererFactory::GetCaptionRenderer()
{
	cmRenderer_CaptionCell* ret;

	ret = new cmRenderer_CaptionCell;

	AddRenderer(ret);
	
	return ret;
}

cmRenderer_CaptionCell* cmRendererFactory::GetCaptionRenderer(const cmRenderer_CaptionCell& templateRenderer)
{
	cmRenderer_CaptionCell* ret;

	ret = GetCaptionRenderer();

	*ret = templateRenderer;
	
	return ret;
}
	
cmRenderer_GradientCell* cmRendererFactory::GetGradientRenderer()
{
	cmRenderer_GradientCell* ret;

	ret = new cmRenderer_GradientCell;

	AddRenderer(ret);

	return ret;
}

cmRenderer_GradientCell* cmRendererFactory::GetGradientRenderer(const cmRenderer_GradientCell& templateRenderer)
{
	cmRenderer_GradientCell* ret;

	ret = GetGradientRenderer();

	*ret = templateRenderer;

	return ret;
}


void cmRendererFactory::AddRenderer(cmCellBaseRenderer* rend)
{
	set<cmCellBaseRenderer*>::iterator it;

	it = renderers_.find(rend);
	assert(it == renderers_.end());

	renderers_.insert(rend);
}

cmRenderer_GlassCell* cmRendererFactory::GetGlassRenderer()
{
	cmRenderer_GlassCell* ret;

	ret = new cmRenderer_GlassCell;

	AddRenderer(ret);

	return ret;
}

cmRenderer_GlassCell* cmRendererFactory::GetGlassRenderer(const cmRenderer_GlassCell& templateRenderer)
{
	cmRenderer_GlassCell* ret;

	ret = GetGlassRenderer();

	*ret = templateRenderer;

	return ret;
}

