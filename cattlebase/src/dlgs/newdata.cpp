#include "stdafx.h"
#include "newdata.h"


BEGIN_MESSAGE_MAP_TEMPLATE(CNewDataDlg,BASE_CLASS,DATA_TYPE)
	//{{AFX_MSG_MAP(COptionsSheet)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	//ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


template<class BASE_CLASS,class DATA_TYPE>
CNewDataDlg<BASE_CLASS,DATA_TYPE>::CNewDataDlg(DATA_TYPE *data) :
     BASE_CLASS(data)
{
}

template<class BASE_CLASS,class DATA_TYPE>
CNewDataDlg<BASE_CLASS,DATA_TYPE>::~CNewDataDlg()
{
}

template<class BASE_CLASS,class DATA_TYPE>
void CNewDataDlg<BASE_CLASS,DATA_TYPE>::OnOK()
{
	BASE_CLASS::OnOK();
	GetParent()->SendMessage(WM_COMMAND,IDOK);
	
}

template<class BASE_CLASS,class DATA_TYPE>
void CNewDataDlg<BASE_CLASS,DATA_TYPE>::OnCancel()
{
	BASE_CLASS::OnCancel();
	GetParent()->SendMessage(WM_COMMAND,IDCANCEL);
}


