// cmRendererSet.h: interface for the cmRendererSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMRENDERERSET_H_)
#define _CMRENDERERSET_H_

#include "cmFontInfo.h"
#include <vector>

using namespace std;

class cmBaseCell;
class cmCellBaseRenderer;

class cmRendererSet
{
public:
	cmRendererSet();
	virtual ~cmRendererSet();

	void AppendRenderer(cmCellBaseRenderer* renderer);
	void Draw(CDC* pDC, cmBaseCell* cell);
	cmFontInfo GetTopFontInfo();

	void CleanUp(bool deleteRenderers);

protected:
	vector<cmCellBaseRenderer*> list_;
};

#endif // !defined(_CMRENDERERSET_H_)
