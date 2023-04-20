// cmRendererFactory.h: interface for the cmRendererFactory class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <set>

using namespace std;

class cmCellBaseRenderer;
class cmRenderer_CaptionCell;
class cmRenderer_GradientCell;
class cmRenderer_GlassCell;

class cmRendererFactory  
{
public:
	cmRendererFactory();
	virtual ~cmRendererFactory();

	void CleanUp();
	void ReleaseRenderer(cmCellBaseRenderer* rend);

	cmRenderer_CaptionCell* GetCaptionRenderer();
	cmRenderer_CaptionCell* GetCaptionRenderer(const cmRenderer_CaptionCell& templateRenderer);

	cmRenderer_GradientCell* GetGradientRenderer();
	cmRenderer_GradientCell* GetGradientRenderer(const cmRenderer_GradientCell& templateRenderer);

	cmRenderer_GlassCell* GetGlassRenderer();
	cmRenderer_GlassCell* GetGlassRenderer(const cmRenderer_GlassCell& templateRenderer);

protected:

	void AddRenderer(cmCellBaseRenderer* rend);

	set<cmCellBaseRenderer*> renderers_;
};
