#include "stdafx.h"
#include "GeoCoordinateEdit.h"
#include <stdio.h>

const char* szGEOCOORD_FORMAT = "%03d\xB0 %02d' %02d\" %c";

BEGIN_MESSAGE_MAP(CGeoCoordinateEdit, CTipEdit)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


CGeoCoordinateEdit::CGeoCoordinateEdit() : CTipEdit("###\xB0 ##' ##\" >")
{

}

bool CGeoCoordinateEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	unsigned short degrees;
	unsigned short minutes;
	unsigned short seconds;
	Hemisphere hemisphere = NullHemisphere();
	return 	GetGeoCoordinate(degrees,minutes,seconds,hemisphere);
}
void CGeoCoordinateEdit::SetGeoCoordinate(const GeoCoordinate& geoCoord)
{
	CString sGeoCoord;
	sGeoCoord.Format(szGEOCOORD_FORMAT,geoCoord.GetDegrees(),geoCoord.GetMinutes(),geoCoord.GetSeconds(),geoCoord.GetHemisphere().GetSuffix());
	SetWindowText(sGeoCoord);
}

bool CGeoCoordinateEdit::GetGeoCoordinate(unsigned short& degrees,unsigned short& minutes,unsigned short& seconds,Hemisphere& hemisphere) const
{
	if(IsInputEmpty())
		return false;

	if(!IsFullData())
		return false;

	int degs = 181,mins = 61,secs = 61;
	int cHemisphere = 0;

	CString sGeoCoord;
	GetWindowText(sGeoCoord);

	if(sscanf((LPCSTR)sGeoCoord,szGEOCOORD_FORMAT,&degs,&mins,&secs,&cHemisphere) != 4)
		return false;
	
	degrees = degs;
	minutes = mins;
	seconds = secs;
	hemisphere = HemisphereFactory::GetHemisphere(cHemisphere);

	return true;
}


BOOL CGeoCoordinateEdit::OnValidate()
{
	if(!CTipEdit::OnValidate()) return FALSE;

	if(!IsValidGeoCoordinate())
	{
		DisplayToolTip(true);
		return FALSE;
	}
	return TRUE;
}
void CGeoCoordinateEdit::PrepareToolTip(CWnd *pParent)
{
	CreateToolTip(pParent,GetValidationErrorMessage(),2,"Niepoprawna wspó³rzêdna geograficzna.");
}

void	CGeoCoordinateEdit::OnPaint() 
{
	Default();
	DrawGetCoordHint();	
}


void	CGeoCoordinateEdit::DrawGetCoordHint()
{
	CString geoCoordHint = GetGeoCoordHint();
	if( geoCoordHint.IsEmpty() )
	{
		return;					
	}
	CClientDC	dcDraw( this );
	CRect		rRect;
	int			iState = dcDraw.SaveDC();					// Save The DC State
	
	GetClientRect( &rRect );								// Get Drawing Area
//	rRect.OffsetRect( 1, 1 );								// Add Sanity Space
	
	dcDraw.SelectObject( (*GetFont()) );					// Use The Control's Current Font
	dcDraw.SetTextColor( RGB(192,192,192) );				// Set The Text Color
	dcDraw.SetBkColor( GetSysColor( COLOR_WINDOW ) );		// Set The Bk Color
	dcDraw.DrawText( geoCoordHint, geoCoordHint.GetLength(), &rRect, 
			( DT_RIGHT | DT_VCENTER ) );					// Draw The Dim Text
	
	dcDraw.RestoreDC( iState );								// Restore The DC State

	return;													// Done!
}


BOOL CGeoCoordinateEdit::OnEraseBkgnd(CDC* pDC) 
{
	BOOL	bStatus = CEdit::OnEraseBkgnd(pDC);

	if( bStatus )
	{
		DrawGetCoordHint();									
	}
	return( bStatus );										
}

void CGeoCoordinateEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CTipEdit::OnSetFocus(pOldWnd);
	DrawGetCoordHint();
}
