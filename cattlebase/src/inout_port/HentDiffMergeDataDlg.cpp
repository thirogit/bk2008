#include "stdafx.h"
#include "HentDiffMergeDataDlg.h"
#include "../datatypes/datacopy/StringDataMemberCopy.h"
#include "../datatypes/datacopy/DateTimeDataMemberCopy.h"
#include "../datatypes/datacopy/PtrDataMemberCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CHentDiffMergeDataDlg, CDiffMergeDataDlg)

CHentDiffMergeDataDlg::CHentDiffMergeDataDlg(Hent* pHent1,Hent *pHent2,CWnd* pParent /*=NULL*/)
	: CDiffMergeDataDlg(m_hentDiffArray, pParent)
{	

	
	
	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_NAME,	IDS_HENTDATA_MEMBER_NAME,		
		new CStringDataMemberCopy<Hent>(&Hent::GetName,&Hent::ChangeName),
							pHent1,	pHent2));	
	
	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_STREET, IDS_HENTDATA_MEMBER_STREET,		
							new CStringDataMemberCopy<Hent>(&Hent::GetStreet,&Hent::ChangeStreet),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_POBOX, IDS_HENTDATA_MEMBER_POBOX,
							new CStringDataMemberCopy<Hent>(&Hent::GetPOBox,&Hent::ChangePOBox),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_CITY,	IDS_HENTDATA_MEMBER_CITY,
							new CStringDataMemberCopy<Hent>(&Hent::GetCity,&Hent::ChangeCity),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_ZIP, IDS_HENTDATA_MEMBER_ZIP,
							new CStringDataMemberCopy<Hent>(&Hent::GetZip,&Hent::ChangeZip),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTS_SORT_BY_PLATE, IDS_HENTDATA_MEMBER_PLATE,
							new CStringDataMemberCopy<Hent>(&Hent::GetPlate,&Hent::ChangePlate),
							pHent1,	pHent2));
	
	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_PHONE, IDS_HENTDATA_MEMBER_PHONE,
							new CStringDataMemberCopy<Hent>(&Hent::GetPhone,&Hent::ChangePhone),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_NIP, IDS_HENTDATA_MEMBER_NIP,
							new CStringDataMemberCopy<Hent>(&Hent::GetNIP,&Hent::ChangeNIP),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_WETNO, IDS_HENTDATA_MEMBER_WETNO,
							new CStringDataMemberCopy<Hent>(&Hent::GetWetNo,&Hent::ChangeWetNo),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_EXTRAS, IDS_HENTDATA_MEMBER_EXTRAS,
							new CStringDataMemberCopy<Hent>(&Hent::GetExtras,&Hent::ChangeExtras),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_COUNTRY, IDS_HENTDATA_MEMBER_COUNTRY,
							new CPtrDataMemberCopy<Hent,Country>(&Hent::GetCountry,&Hent::ChangeCountry),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_PESEL, IDS_HENTDATA_MEMBER_PESEL,
							new CStringDataMemberCopy<Hent>(&Hent::GetPESEL,&Hent::ChangePESEL),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_REGON, IDS_HENTDATA_MEMBER_REGON,
							new CStringDataMemberCopy<Hent>(&Hent::GetREGON,&Hent::ChangeREGON),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_IDNO, IDS_HENTDATA_MEMBER_IDNO,
							new CStringDataMemberCopy<Hent>(&Hent::GetIdNo,&Hent::ChangeIdNo),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_ISSUEDATE, IDS_HENTDATA_MEMBER_ISSUEDATE,
							new CDateTimeDataMemberCopy<Hent>(&Hent::GetIssueDate,&Hent::ChangeIssueDate),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_ISSUEPOST, IDS_HENTDATA_MEMBER_ISSUEPOST,
							new CStringDataMemberCopy<Hent>(&Hent::GetIssuePost,&Hent::ChangeIssuePost),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_ACCOUNTNO, IDS_HENTDATA_MEMBER_ACCOUNTNO,
							new CStringDataMemberCopy<Hent>(&Hent::GetAccountNo,&Hent::ChangeAccountNo),
							pHent1,	pHent2));

	m_AllToDiff.Add(new CHentDiffMerge(	HENTDATA_MEMBER_BANKNAME, IDS_HENTDATA_MEMBER_BANKNAME,
							new CStringDataMemberCopy<Hent>(&Hent::GetBankName,&Hent::ChangeBankName),
							pHent1,	pHent2));
	m_AllToDiff.Add(new CHentDiffMerge( HENTDATA_MEMBER_WETLICENCENO, IDS_HENTDATA_MEMBER_WETLICENCE,
							new CStringDataMemberCopy<Hent>(&Hent::GetWetLicenceNo,&Hent::ChangeWetLicenceNo),
							pHent1,	pHent2));
	m_AllToDiff.Add(new CHentDiffMerge( HENTDATA_MEMBER_CELLPHONENO, IDS_HENTDATA_MEMBER_CELLPHONENO,
							new CStringDataMemberCopy<Hent>(&Hent::GetCellPhoneNo,&Hent::ChangeCellPhoneNo),
							pHent1,	pHent2));
	m_AllToDiff.Add(new CHentDiffMerge( HENTDATA_MEMBER_EMAILADDRESS, IDS_HENTDATA_MEMBER_EMAILADDRESS,
							new CStringDataMemberCopy<Hent>(&Hent::GetEmailAddress,&Hent::ChangeEmailAddress),
							pHent1,	pHent2));
	m_AllToDiff.Add(new CHentDiffMerge( HENTDATA_MEMBER_LATITUDE, IDS_HENTDATA_MEMBER_LATITUDE,
							new CDataMemberCopy<Hent,Latitude>(&Hent::GetLatitude,&Hent::ChangeLatitude),
							pHent1,	pHent2));
	m_AllToDiff.Add(new CHentDiffMerge( HENTDATA_MEMBER_LONGITUDE, IDS_HENTDATA_MEMBER_LONGITUDE,
							new CDataMemberCopy<Hent,Longitude>(&Hent::GetLongitude,&Hent::ChangeLongitude),
							pHent1,	pHent2));

	DiffMemberArray::ArrayIterator diffIt = m_AllToDiff.iterator();
	CBaseDataDiffMerge *pBDDM = NULL;
	while(diffIt.hasNext())
	{
		pBDDM = *diffIt;

		if(pBDDM->AreDiffrent())
			m_hentDiffArray.Add(pBDDM);
		diffIt++;
	}
}

CHentDiffMergeDataDlg::~CHentDiffMergeDataDlg()
{
	m_AllToDiff.FreeAndFlush();
}

void CHentDiffMergeDataDlg::OnOK()
{
	DiffMemberArray::ArrayIterator diffIt = m_hentDiffArray.iterator();
	while(diffIt.hasNext())
		(*(diffIt++))->DoCopyIfSet();		
		
	EndDialog(IDOK);
}

bool CHentDiffMergeDataDlg::AreDiffrent()
{
	return m_hentDiffArray.GetSize() > 0;
}
