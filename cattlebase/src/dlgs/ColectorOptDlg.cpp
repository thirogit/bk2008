// ColectorOptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cattlebase.h"
#include "ColectorOptDlg.h"
// CColectorOptDlg dialog

IMPLEMENT_DYNAMIC(CColectorOptDlg, CPropertyPage)
CColectorOptDlg::CColectorOptDlg()
	: CPropertyPage(CColectorOptDlg::IDD)
{
}

CColectorOptDlg::~CColectorOptDlg()
{
}

void CColectorOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_COMPORT,m_port);
	DDX_Control(pDX,IDC_SPEED,m_speed);
	DDX_Control(pDX,IDC_DATABITS,m_databits);
	DDX_Control(pDX,IDC_PARITY,m_parity);
	DDX_Control(pDX,IDC_STOPBITS,m_stopbits);
	DDX_Control(pDX,IDC_FLOW,m_flow);
	
}


BEGIN_MESSAGE_MAP(CColectorOptDlg, CPropertyPage)
END_MESSAGE_MAP()


BOOL CColectorOptDlg::OnInitDialog()
{
	if(!CPropertyPage::OnInitDialog()) return FALSE;

	int AryDataBits[] = {5,6,7,8};

	int ArySpeeds[] = {9600,19200,38400,57600,115200};
	                       /*0		 1            2           3         4 */
	char *szAryParity[] = {"Brak","Nieparzyste","Parzyste","Znacznik","Odstep"};

                        /*  0    1    2  */
	char *szAryStopBits[5] = {"1","1.5","2"};

    //char szFlowConrtol("Xon/Xof","Sprzet","Brak");
//GetCommState

}

void CColectorOptDlg::FillComboInt(CComboBox &combo,int *ary,int nsize)
{
	char buff[50];
	int item;
	for(int i = 0;i < nsize;i++)
	{
		sprintf(buff,"%d",ary[i]);
		item = combo.AddString(buff);
		combo.SetItemData(item,ary[i]);
	}

}

void CColectorOptDlg::FillComboStr(CComboBox &combo,char *ary[],int nsize)
{
	int item;
	for(int i = 0;i < nsize;i++)
	{
		item = combo.AddString(ary[i]);
		combo.SetItemData(item,i);
	}

}


