// AsFontInfo.cpp
//

#include "stdafx.h"
#include "cmFontInfo.h"


cmFontInfo::cmFontInfo():
	fontName_(_T("tahoma")),
	fontSize_(8),
	fontBold_(false),
	font_(NULL),
	updatePending_(true)
{}

cmFontInfo::cmFontInfo(const cmFontInfo& fontInfo):
	fontName_(_T("tahoma")),
	fontSize_(8),
	fontBold_(false),
	font_(NULL),
	updatePending_(true)
{
	Copy(fontInfo);
}

cmFontInfo::~cmFontInfo()
{
	ReleaseFont();
	
}

cmFontInfo& cmFontInfo::operator=(const cmFontInfo& fontInfo)
{
	Copy(fontInfo);
	
	return *this;
}

void cmFontInfo::SetFontName(const cmString& fontName)
{
	fontName_ = fontName;
	updatePending_ = true;
}

cmString cmFontInfo::GetFontName() const
{
	return fontName_;
}

void cmFontInfo::SetFontSize(int fontSize)
{
	fontSize_ = fontSize;
	updatePending_ = true;
}

int cmFontInfo::GetFontSize() const
{
	return fontSize_;
}

void cmFontInfo::SetFontBold(bool fontBold)
{
	fontBold_ = fontBold;
	updatePending_ = true;
}

bool cmFontInfo::GetFontBold() const
{
	return fontBold_;
}

void cmFontInfo::Copy(const cmFontInfo& fontInfo)
{
	fontName_ = fontInfo.fontName_;
	fontSize_ = fontInfo.fontSize_;
	fontBold_ = fontInfo.fontBold_;
	ReleaseFont();

	updatePending_ = true;
}

CFont* cmFontInfo::CreateFont()
{
	if (!updatePending_ && font_)
		return font_;

	ReleaseFont();

	font_ = new CFont;
	
	font_->CreateFont(-fontSize_, 
					 0,										/* width */ 
					 0,										/* escapement */
					 0,										/* orientation */
					 fontBold_ ? FW_BOLD : FW_NORMAL,		/* weight*/
					 0,										/* italic*/
					 0,										/* underline */
					 0,										/* strikeout */
					 ANSI_CHARSET,							/* charset */
					 OUT_DEFAULT_PRECIS,					/* out precision */
					 CLIP_DEFAULT_PRECIS,					/* clip precision */
					 DEFAULT_QUALITY,						/* quality */
					 DEFAULT_PITCH,							/* pitch family */
					 fontName_.c_str());

	updatePending_ = false;

	return font_;
}

void cmFontInfo::ReleaseFont()
{
	if (font_) {
		font_->DeleteObject();
		delete font_;
		font_ = NULL;
	}
}	


CFont* cmFontInfo::GetFont()
{
	return font_;
}