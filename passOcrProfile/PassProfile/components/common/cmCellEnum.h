// cmCellEnum.h
//

#if !defined(_CM_CELLENUM_H_)
#define _CM_CELLENUM_H_

typedef enum enCmCellType
{
	enCmCellType_Caption = 0,
	enCmCellType_TextBox,
	enCmCellType_NumTextBox,
	enCmCellType_CheckBox,
	enCmCellType_ComboBox,
	enCmCellType_HexTextBox,
	enCmCellType_BinaryWildCards,
	enCmCellType_SmartNumTextBox,
	enCmCellType_Header,
	enCmCellType_Date,
	enCmCellType_Time
} enCmCellType;

typedef enum enCmCellStatus
{
	enCmCellStatus_Normal = 0,
	enCmCellStatus_Selected = 1,
	enCmCellStatus_NormalMouseOver = 2,
	enCmCellStatus_SelectedMouseOver = 3
} enCmCellStatus;

typedef enum enCmRenderingMode
{
	enCmRenderingMode_Stock = 0,
	enCmRenderingMode_Custom
} enCmRenderingMode;

typedef enum enCmCellExtraMsgType
{
	enCmCellExtraMsgType_None = 0,		// no extra message is displayed in the info area
	enCmCellExtraMsgType_Info = 1,
	enCmCellExtraMsgType_Warning = 2,
	enCmCellExtraMsgType_Question = 3,
	enCmCellExtraMsgType_Error = 4
};

#endif //!defined(_CM_CELLENUM_H_)