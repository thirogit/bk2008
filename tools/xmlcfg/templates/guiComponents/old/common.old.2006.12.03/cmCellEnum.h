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
		enCmCellType_SmartNumTextBox
	} enCmCellType;

#endif //!defined(_CM_CELLENUM_H_)