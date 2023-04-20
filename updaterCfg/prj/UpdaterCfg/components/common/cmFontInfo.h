// AsFontInfo.h
//

#if !defined(_ASFONTINFO_H_)
#define _ASFONTINFO_H_

#include "cmString.h"

class cmFontInfo
{
public:
	cmFontInfo();
	cmFontInfo(const cmFontInfo& fontInfo);

	virtual ~cmFontInfo();

	cmFontInfo& operator=(const cmFontInfo& fontInfo);

	void SetFontName(const cmString& fontName);
	cmString GetFontName() const;

	void SetFontSize(int fontSize);
	int GetFontSize() const;

	void SetFontBold(bool fontBold);
	bool GetFontBold() const;

	CFont* CreateFont();
	
	CFont* GetFont();
	void ReleaseFont();

protected:
	void Copy(const cmFontInfo& fontInfo);

	cmString fontName_;
	int fontSize_;
	bool fontBold_;
	CFont* font_;
	bool updatePending_;
};

#endif //!defined(_ASFONTINFO_H_)
