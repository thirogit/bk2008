#include "stdafx.h"
#include "QuestionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGE_PADDING_Y 14
#define MESSAGE_PADDING_X 14

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 23

#define MAX_BTNS_IN_ROW 4
#define BUTTON_ROW_DIVIDER_HEIGHT 5
#define BUTTON_COLUMN_DIVIDER_WIDTH 5


#define BUTTON_ROW_MARGIN 0//10
		
#define CHECKBOX_CTRL_ID 10010
#define MIN_BTN_CTRL_ID 10011

QuestionDlg::QuestionDlg(const QuestionDlgTemplate* pTemplate) : m_pDlgTemplate(pTemplate)
{	
}

QuestionDlg::~QuestionDlg()
{
	
} 

UINT QuestionDlg::Display(CWnd* pParentWnd)
{
	LPDLGTEMPLATE pdlg;

	HGLOBAL hgbl = GlobalAlloc(GMEM_ZEROINIT, sizeof(DLGTEMPLATE)*2);

	_ASSERT(hgbl);

	if (NULL == hgbl)
		return (UINT)-1;

    pdlg = (LPDLGTEMPLATE)GlobalLock(hgbl);
	_ASSERT(pdlg);
	if (NULL == hgbl)
	{
		GlobalFree(hgbl);
		return (UINT)-1;
	} 

    pdlg->x = 0;
    pdlg->y = 0;
    pdlg->cx = 100;
    pdlg->cy = 100;
    pdlg->style = DS_MODALFRAME|WS_VISIBLE|WS_POPUP|WS_BORDER|WS_CAPTION|WS_SYSMENU|DS_CENTER;
    pdlg->dwExtendedStyle = 0;
    pdlg->cdit = 0;


	

	HWND hParentWnd = NULL;
	if (pParentWnd == NULL)
	{
		hParentWnd = ::GetActiveWindow();
		if (hParentWnd != NULL)
			hParentWnd = ::GetLastActivePopup(hParentWnd);
	} 
	else
	{
		hParentWnd = pParentWnd->GetSafeHwnd();
	}

	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	int nResult = ::DialogBoxIndirectParam(hInstance, pdlg, hParentWnd, DlgProc, (LPARAM)this);
	UINT error = ::GetLastError();
	GlobalUnlock(pdlg);
    GlobalFree(hgbl);

	return nResult;
} 

BOOL CALLBACK QuestionDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	QuestionDlg * pDlg = (QuestionDlg*)::GetWindowLong(hwnd, GWL_USERDATA);

	switch (message)
	{
	case WM_INITDIALOG:
		{
			::SetWindowLong(hwnd, GWL_USERDATA, lParam);	// save it for the others
			pDlg = (QuestionDlg*)lParam;
			_ASSERTE(pDlg);
			CWnd dlgWnd;
			dlgWnd.Attach(hwnd);
			pDlg->OnInitDialog(&dlgWnd);
			dlgWnd.Detach();
			return FALSE;
		} 
	case WM_DESTROY:
		{
			pDlg->RemoveAllButtons();
			return FALSE;
		} 
	/*case WM_PAINT:
		{
			PAINTSTRUCT ps; 
			BeginPaint(hwnd, &ps);
			pDlg->DrawBox(hwnd, ps.hdc);
			EndPaint(hwnd, &ps);
			return FALSE;
		} */
	case WM_COMMAND:
		{
			DWORD dwBtnID = LOWORD(wParam);
			if(dwBtnID)
			{
				//switch (dwBtnID)
				//{
				//			
				//default:
				//	{
				//		////ENG: Handles a checkbox state
				//		//if (NULL != pDlg->m_stCheckBtn.hWnd)
				//		//{
				//		//	//ENG: Toggles a checkbox state
				//		//	//RUS: Переключаем состояние checkbox
				//		//	LRESULT nCheck = ::SendMessage(pDlg->m_stCheckBtn.hWnd, BM_GETCHECK, 0, 0);
				//		//	if (BST_CHECKED == nCheck)
				//		//	{
				//		//		dwBtnID |= pDlg->m_MsgParam.dwStyle & MB_CHECKBOXCHECKED;

				//		//	} //if
				//		//} //if

				//		//ENG: Close a message box and return code
				//		::EndDialog(hwnd, dwBtnID);
				//		return TRUE;
				//	}
				//} //switch
			} //if
		} 
		break;

	case WM_CTLCOLORSTATIC:
		{
			//ENG: For a transparent text of the checkbox
			::SetBkMode((HDC)wParam, TRANSPARENT);
			return (BOOL)::GetStockObject(NULL_BRUSH);
		} //End of case WM_CTLCOLORSTATIC
		break;
	case WM_ERASEBKGND:
		{
			return TRUE;
		} //End of case WM_ERASEBKGND
	
	} //switch

	return FALSE;
}

void QuestionDlg::OnInitDialog(CWnd* pDlgWnd)
{
	CSize size = PrepareMessageBox(pDlgWnd);
//
//	//Set a message box caption
//	BOOL bGrayedClose = m_bIsCancelExist ? FALSE : TRUE;
//	str = m_sCaption;
//	
	pDlgWnd->SetWindowText(m_pDlgTemplate->GetCaption());
//
//	//Disable a menu
//	if (bGrayedClose)
//		::EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_GRAYED);
//
	//Centering of the dialog
	RECT rcParent;
	if (NULL != pDlgWnd->GetParent())
		::GetWindowRect(pDlgWnd->GetParent()->GetSafeHwnd(), &rcParent);
	else
	{
			rcParent.left    = 0;
			rcParent.top     = 0;
			rcParent.right   = ::GetSystemMetrics(SM_CXSCREEN);
			rcParent.bottom  = ::GetSystemMetrics(SM_CYSCREEN);
	} //if

	pDlgWnd->MoveWindow(rcParent.left + (rcParent.right - rcParent.left - size.cx) / 2, 
						rcParent.top + (rcParent.bottom - rcParent.top - size.cy) / 2, 
						size.cx, size.cy, TRUE);
//
//	//ENG: Gets a size of the client area of the messagebox
//
//	RECT rcClient;
//	::GetClientRect(hwnd, &rcClient);
//
//	//ENG: Calculates a full width of all buttons
//	//RUS: Вычисляем ширину, занимаемую всеми кнопками
//	int nWidth = m_vecButtons.size() * (m_dwSizes [PPMSGBOX_SIZE_BTNX] + m_dwSizes [PPMSGBOX_SIZE_CTRLX]) - m_dwSizes [PPMSGBOX_SIZE_CTRLX];
//
//	RECT rcBtn = rcClient;
//
//	rcBtn.bottom -= m_dwSizes [PPMSGBOX_SIZE_CTRLY];
//	if ((m_MsgParam.dwStyle & MB_CHECKBOX) && (m_MsgParam.dwStyle & MB_CHECKBOXUNDERBUTTONS))
//		rcBtn.bottom -= m_dwSizes [PPMSGBOX_SIZE_CTRLY] + m_dwSizes [PPMSGBOX_SIZE_CHKY];
//	rcBtn.top = rcBtn.bottom - m_dwSizes [PPMSGBOX_SIZE_BTNY];
//
//	
//	//Create buttons
//	for (UINT i = 0; i < m_vecButtons.size(); ++i) 
//	{
//		//ENG: Get a right edge of the button
//		//RUS: Формируем координату правой границы кнопки
//		rcBtn.right = rcBtn.left + m_dwSizes [PPMSGBOX_SIZE_BTNX];
//
//		STRUCT_BUTTONS & sb = m_vecButtons [i];
//	
//		//ENG: Create a button
//		//RUS: Создаем кнопку
//		::MoveWindow(sb.hWnd, rcBtn.left, rcBtn.top, m_dwSizes [PPMSGBOX_SIZE_BTNX], m_dwSizes [PPMSGBOX_SIZE_BTNY], TRUE);
//
//		//ENG: Jump to the left edge of the next button
//		//RUS: Перемещаемся в позицию левого края следующей кнопки
//		rcBtn.left += m_dwSizes [PPMSGBOX_SIZE_BTNX] + m_dwSizes [PPMSGBOX_SIZE_CTRLX];
//	} //for
//
//	//ENG: Create a checkbox
//	//RUS: Создаем checkbox
//	if (m_stCheckBtn.nTypeBtn)
//	{
//		//ENG: Gets a text of the checkbox
//		//RUS: Получаем текст checkbox
////		CPPString sCheckText = GetButtonText(m_stCheckBtn.nTypeBtn);
//		
//		//Creates a checkbox control
//		rcBtn = rcClient;
//		rcBtn.bottom -= m_dwSizes [PPMSGBOX_SIZE_CTRLY];
//		if (!(m_MsgParam.dwStyle & MB_CHECKBOXUNDERBUTTONS))
//			rcBtn.bottom -= m_dwSizes [PPMSGBOX_SIZE_CTRLY] + m_dwSizes [PPMSGBOX_SIZE_BTNY];
//		rcBtn.top = rcBtn.bottom - m_dwSizes [PPMSGBOX_SIZE_CHKY];
//		rcBtn.left += m_dwSizes [PPMSGBOX_SIZE_CHKX];
//
//		::MoveWindow(m_stCheckBtn.hWnd, rcBtn.left, rcBtn.top, m_dwSizes [PPMSGBOX_SIZE_CHKY], // + m_dwSizes [PPMSGBOX_SIZE_CHKY] / 2 + sz.cx, 
//			m_dwSizes [PPMSGBOX_SIZE_CHKY], TRUE);
//
//		//ENG: To the transparency output of the text of the checkbox
//		//RUS: Текст для checkbox выводим отдельно для прозрачного вывода
//		rcBtn.left += m_dwSizes [PPMSGBOX_SIZE_CHKY] + m_dwSizes [PPMSGBOX_SIZE_CHKY] / 2;
//		RECT rcCheck;
//		::GetWindowRect(m_stCheckText.hWnd, &rcCheck);
//		::MoveWindow(m_stCheckText.hWnd, rcBtn.left, rcBtn.top, rcCheck.right - rcCheck.left, 
//			rcCheck.bottom - rcCheck.top, TRUE);
//	} //if
//
//	//ENG: The message box is created with the WS_EX_TOPMOST window style
//	if (m_MsgParam.dwStyle & MB_TOPMOST)
//		::SetWindowPos(hwnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
//
//	//ENG: The message box becomes the foreground window
//	if (m_MsgParam.dwStyle & MB_SETFOREGROUND)
//		::SetForegroundWindow(hwnd);

} 


void QuestionDlg::RemoveAllButtons()
{
	////ENG: Removes an existing control buttons

	//for (UINT i = 0; i < m_vecButtons.size(); ++i)
	//{
	//	STRUCT_BUTTONS & sb = m_vecButtons [i];
	//	if (NULL != sb.hWnd)
	//	{
	//		::DestroyWindow(sb.hWnd);
	//		sb.hWnd = NULL;
	//	}
	//} //for
	//m_vecButtons.clear();

	////ENG: Removes a checkbox

	//if (NULL != m_stCheckBtn.hWnd)
	//{
	//	::DestroyWindow(m_stCheckBtn.hWnd);
	//	m_stCheckBtn.hWnd = NULL;
	//} //if

	////ENG: Removes a text of the checkbox
	//
	//if (NULL != m_stCheckText.hWnd)
	//{
	//	::DestroyWindow(m_stCheckText.hWnd);
	//	m_stCheckText.hWnd = NULL;
	//} //if
}

CSize QuestionDlg::PrepareMessageBox(CWnd* pDlgWnd)
{
	//RemoveAllButtons();

	

	
	CSize sz(0, 0);
	//SIZE szTemp = {0, 0};

	////---------------- Header area -----------------------

	//m_dwHeaderHeight = 0;
	//if (m_MsgParam.nHeaderHeight)
	//{
	//	//ENG: Stores a minimal height of the header area
	//	//RUS: Сохраняем минимальную высоту области заголовка
	//	m_dwHeaderHeight = m_MsgParam.nHeaderHeight;
	//	szTemp.cx = 0;
	//	m_sHeader = GetString(m_MsgParam.lpszHeaderText);
	//	if (!m_sHeader.IsEmpty())
	//	{
	//		//ENG: If header text is available then prepare him to output
	//		//RUS: Если текст заголовка существует, то подготавливаем его к выводу
	//		m_pHeader.PrepareOutput(hDC, m_sHeader, &szTemp);
	//		szTemp.cx += m_dwSizes[PPMSGBOX_SIZE_MSGX] * 2;
	//		
	//		//ENG: Updates a height of the header area
	//		//RUS: Обновляем высоту области заголовка
	//		m_dwHeaderHeight = max(m_dwHeaderHeight, szTemp.cy + m_dwSizes[PPMSGBOX_SIZE_MSGX] * 2);
	//	} //if
	//	sz.cx = max(sz.cx, szTemp.cx);

	//	//ENG: Header separator will be drawn
	//	//RUS: Разделитель заголовка будет выведен
	//	m_sHeaderSepText = GetString(m_MsgParam.pHeaderBk.lpszSepText);
	//	if (m_MsgParam.pHeaderBk.nSepType != PPMSGBOX_SEP_NONE)
	//	{
	//		::GetTextExtentPoint32(hDC, m_sHeaderSepText, m_sHeaderSepText.GetLength(), &szTemp);
	//		sz.cx = max(sz.cx, (int)(szTemp.cx + 2 * m_dwSizes [PPMSGBOX_SIZE_SEPX]));
	//	} //if
	//} //if
	//sz.cy += m_dwHeaderHeight;

	////--------------- Message area -----------------------

	CString sMsgText = m_pDlgTemplate->GetQuestion();
	int messageHeight = 2*MESSAGE_PADDING_Y;
	int messageWidth = 2*BUTTON_ROW_MARGIN + BUTTON_WIDTH;

	if (!sMsgText.IsEmpty())
	{
		//ENG: If message text is available then prepare him to output
		CSize messageTextSize(120,40);
		//::GetTextExtentPoint32(m_hDC, sText, sText.GetLength(), &sz);
		//m_pDrawer.PrepareOutput(hDC, m_sText, &szTemp);
		
		//ENG: Updates a height of the message area
		messageHeight = messageTextSize.cy + 2 * MESSAGE_PADDING_Y;
		messageWidth = messageTextSize.cx + 2*MESSAGE_PADDING_X;
		//sz.cx = max(sz.cx, (int)(szTemp.cx + 2 * m_dwSizes [PPMSGBOX_SIZE_MSGX]));
	} //if

	sz.cy += messageHeight;
	sz.cx += messageWidth;
	

	//m_dwControlHeight = m_dwSizes [PPMSGBOX_SIZE_CTRLY];
	//if ((m_MsgParam.pControlBk.nEffectBk >= 0) || (m_MsgParam.pControlBk.nSepType != PPMSGBOX_SEP_NONE))
	//{
	//	m_dwControlHeight += m_dwSizes [PPMSGBOX_SIZE_CTRLY];
	//	
	//	//ENG: Comtrol separator will be drawn
	//	//RUS: Разделитель области управления будет выведен
	//	m_sControlSepText = GetString(m_MsgParam.pControlBk.lpszSepText);
	//	if (m_MsgParam.pControlBk.nSepType != PPMSGBOX_SEP_NONE)
	//	{
	//		::GetTextExtentPoint32(hDC, m_sControlSepText, m_sControlSepText.GetLength(), &szTemp);
	//		sz.cx = max(sz.cx, (int)(szTemp.cx + 2 * m_dwSizes [PPMSGBOX_SIZE_SEPX]));
	//	} //if
	//} //if

	////ENG: Get a checkbox type
	////RUS: Получаем тип checkbox
	//m_stCheckBtn.hWnd = NULL;
	//m_stCheckBtn.nTypeBtn = (m_MsgParam.dwStyle & MB_CHECKBOX);
	//m_stCheckText = m_stCheckBtn;
	//if (m_stCheckBtn.nTypeBtn)
	//{
	//	//ENG: Checkbox will drawn
	//	//RUS: Флажок присутсвует в диалоге
	//	m_dwControlHeight += m_dwSizes [PPMSGBOX_SIZE_CTRLY] + m_dwSizes [PPMSGBOX_SIZE_CHKY];

	//	//ENG: Gets the sizes of the text of the checkbox
	//	//RUS: Получаем размеры текста флажка
	//	CPPString sCheckBoxText = GetString(m_MsgParam.lpszCheckBoxText);
	//	if (sCheckBoxText.IsEmpty()) 
	//		sCheckBoxText = _T("Do not show again");
	//	CPPString str = GetLocalizedButtonText(MB_CHECKBOX, sCheckBoxText);
	//	::GetTextExtentPoint32(hDC, str, str.GetLength(), &szTemp);

	//	//ENG: Check a max width of the dialog
	//	//RUS: Проверяем максимальную ширину диалога
	//	sz.cx = max(sz.cx, (int)(szTemp.cx + 2 * m_dwSizes [PPMSGBOX_SIZE_CHKX] + 20));

	//	m_stCheckBtn.hWnd = ::CreateWindow(_T("BUTTON"), NULL /*sCheckText*/, 
	//		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX, 
	//		0, 0, m_dwSizes [PPMSGBOX_SIZE_CHKY], // + m_dwSizes [PPMSGBOX_SIZE_CHKY] / 2 + sz.cx, 
	//		m_dwSizes [PPMSGBOX_SIZE_CHKY], hwnd, NULL, NULL, NULL);
	//	if (m_MsgParam.dwStyle & MB_CHECKBOXCHECKED)
	//		::SendMessage(m_stCheckBtn.hWnd, BM_SETCHECK, (WPARAM)BST_CHECKED, FALSE);

	//	//ENG: To the transparency output of the text of the checkbox
	//	//RUS: Текст для checkbox выводим отдельно для прозрачного вывода
	//	m_stCheckText.hWnd = ::CreateWindow(_T("STATIC"), str, 
	//		WS_VISIBLE | WS_CHILD | SS_LEFT,
	//		0, 0, szTemp.cx, m_dwSizes [PPMSGBOX_SIZE_CHKY],
	//		hwnd, NULL, NULL, NULL);
	//	::SendMessage(m_stCheckText.hWnd, WM_SETFONT, (WPARAM)m_hFont, FALSE);
	//} //if

	////ENG: Creates a list of the buttons 
	////RUS: Создаем список кнопок
	//STRUCT_BUTTONS sb;
	//sb.hWnd = NULL;

	//CPPString sCustomButtons = GetString(m_MsgParam.lpszCustomButtons);
	//if (((m_MsgParam.dwStyle & MB_TYPEMASK) == MB_OK) && sCustomButtons.IsEmpty())
	//	m_bIsCancelExist = TRUE;
	//else
	//	m_bIsCancelExist = FALSE;
	//
	//

	//
	////ENG: Remembers a default button
	////RUS: Запоминаем кнопку по умолчанию
	//m_nDefBtn = (m_MsgParam.dwStyle >> 8) & 0x0f;
	//if (m_nDefBtn >= m_vecButtons.size())
	//	m_nDefBtn = 0;

	////ENG:
	//m_dwSizes [PPMSGBOX_SIZE_BTNY] = 0;
	//m_dwSizes [PPMSGBOX_SIZE_BTNX] = 0;
	//int nCustomIndex = 0;
	UINT nBtnId = MIN_BTN_CTRL_ID;

	const int btnRowHeight = BUTTON_ROW_DIVIDER_HEIGHT + BUTTON_HEIGHT;

	int btnX = BUTTON_ROW_MARGIN,btnY = sz.cy;

	for (int iBtn = 0,nBtnCount = m_pDlgTemplate->GetAnswerButtonCount(); iBtn < nBtnCount; ++iBtn)
	{
		const AnswerButton* pAnswerBtn = m_pDlgTemplate->GetButton(iBtn);
		CButton* pBtn = new CButton();
		m_DlgItems.Add(pBtn);

		
		
		pBtn->Create(pAnswerBtn->GetCaption(),
					 WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
					 CRect(btnX,btnY,btnX+BUTTON_WIDTH,btnY+BUTTON_HEIGHT),
					 pDlgWnd,
					 nBtnId);
		pBtn->SetFont(pDlgWnd->GetFont(),FALSE);


		bool bLastInRow = iBtn > 0 && (iBtn+1)%MAX_BTNS_IN_ROW == 0;
		btnX += BUTTON_WIDTH + (bLastInRow ? 0 : BUTTON_COLUMN_DIVIDER_WIDTH);

		sz.cx = max(sz.cx,btnX + BUTTON_ROW_MARGIN);
		
		if(bLastInRow)
		{
			btnY += btnRowHeight;
			sz.cy += btnRowHeight;
			btnX = BUTTON_ROW_MARGIN; 
		}
	
	
	//	//ENG: Set a focus for default button
	//	//RUS: Для кнопки по умолчанию, передаем ей фокус
	//	if (i == m_nDefBtn)
	//		::SetFocus(sbTemp.hWnd);
	} 
	sz.cy += btnRowHeight;

	
	////ENG: Calculates a width of the all buttons area
	////RUS: Подсчитываем ширину области всех кнопок
	//szTemp.cx = m_dwSizes [PPMSGBOX_SIZE_CTRLX] + m_vecButtons.size() * (m_dwSizes [PPMSGBOX_SIZE_CTRLX] + m_dwSizes [PPMSGBOX_SIZE_BTNX]);
	//sz.cx = max(sz.cx, szTemp.cx);

	//m_dwControlHeight += m_dwSizes [PPMSGBOX_SIZE_BTNY];
	//
	////ENG: Add a height of the control area
	////RUS: Добавляем высоту области контролов
	//sz.cy += m_dwControlHeight;

	//----------------------------------------------------
	//ENG: Add the edges and the caption
	
	sz.cx += 2 * ::GetSystemMetrics(SM_CXFIXEDFRAME);
	sz.cy += ::GetSystemMetrics(SM_CYCAPTION) + 2 * ::GetSystemMetrics(SM_CYFIXEDFRAME);

	
	return sz;
} 

