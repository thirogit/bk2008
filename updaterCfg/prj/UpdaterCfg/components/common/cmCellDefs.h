// cmCellDefs.h
//
#pragma once

#define CHK_ICON_SIZE		16
#define CHK_LEFT_BORDER		6
#define CHK_TOP_BORDER		2

#define CMC_LOSTFOCUS   WM_USER + 1	// notifies that a cell has lost the focus
#define CMC_SETFOCUS	WM_USER + 2	// notifies that a cell has got the focus
#define CMC_LOSTSEL		WM_USER + 3	// notifies that a cell has lost selection
#define CMC_SETSEL		WM_USER + 4	// notifies that a cell has been selected