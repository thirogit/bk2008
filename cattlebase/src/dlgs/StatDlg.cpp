#include "stdafx.h"
#include "../CattleSQL.h"
#include "StatDlg.h"

// CStatDlg dialog

IMPLEMENT_DYNAMIC(CStatDlg, CDialog)
CStatDlg::CStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatDlg::IDD, pParent)
{
}

CStatDlg::~CStatDlg()
{
}

void CStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_DATA,m_statdata);
	DDX_Control(pDX,IDC_DAYS,m_days);
}


BEGIN_MESSAGE_MAP(CStatDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_DATA, OnChangeData)
	ON_CBN_SELCHANGE(IDC_DAYS, OnChangeDays)
END_MESSAGE_MAP()

BOOL CStatDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_chart.SubclassDlgItem(IDC_CHART,this);
	m_chart.SetChartStyle(NSCS_BAR);
	m_chart.PrepareColors(CNSChartCtrl::SimpleColors);

	char szDaysFmt[] = "%d dni";
	char szBuf[20];


	m_currData = CattleSQL::INDOCS_STAT;
	m_currDays = STAT_START_DAYS;

	int i,item,def = 0;
	for(i = STAT_START_DAYS;i < STATISTICS_DAYS;i++)
	{
		sprintf(szBuf,szDaysFmt,i);
		item = m_days.AddString(szBuf);
		m_days.SetItemData(item,i);
		if(i == m_currDays) def = item;
	}
	
	m_days.SetCurSel(def);

	struct _tagComboStatData
	{
		char szDataName[20];
		CattleSQL::STATDATA eData;
	} sData[] = 
	{
		{"Zwierzêta",CattleSQL::COWS_STAT},
		{"Kontrahenci",CattleSQL::HENTS_STAT},
		{"PZ-tki",CattleSQL::INDOCS_STAT},
		{"WZ-tki",CattleSQL::OUTDOCS_STAT},
		{"Faktury",CattleSQL::INVOICES_STAT}
	};

	def = 0;
	for(i=0;i<(sizeof(sData)/sizeof(_tagComboStatData));i++)
	{
		item = m_statdata.AddString(sData[i].szDataName);
		m_statdata.SetItemData(item,sData[i].eData);
		if(m_currData == sData[i].eData) def = item;
	}

	m_statdata.SetCurSel(def);

	OnChangeData();

	return TRUE;

}

void CStatDlg::ReloadChart(void)
{

	m_chart.ResetChart();
    char buf[17];

	time_t now = time(0);
	struct tm *t;
	
    for(int i = 0; i < m_currDays;i++)
	{
		t = localtime(&now);
	    strftime(buf,sizeof(buf),"%d.%m",t);
		m_chart.AddValue(SQLDB.m_stats.GetDayStat(m_currData,i),buf);
		now -= 24*60*60;
		
	}
	sprintf(buf,"%d",SQLDB.m_stats.GetTotal(m_currData));
	SetDlgItemText(IDC_TOTAL,buf);
	m_chart.RedrawWindow();

}

void CStatDlg::OnChangeData()
{
	int bSel = m_statdata.GetCurSel();
	m_currData = (CattleSQL::STATDATA)m_statdata.GetItemData(bSel);
	CString wtext;
	m_statdata.GetLBText(bSel,wtext);
	m_chart.SetWindowText(wtext);
	ReloadChart();
}

void CStatDlg::OnChangeDays()
{
	m_currDays = m_days.GetItemData(m_days.GetCurSel());
	ReloadChart();
}
