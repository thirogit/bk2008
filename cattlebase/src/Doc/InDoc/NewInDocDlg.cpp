#include "stdafx.h"
#include "NewInDocDlg.h"
#include "../../Cow/NewCowDlg.h"
#include "../../Cow/EditCowDlg.h"

#include "../../PassportStorage/PassImgBase.h"
#include "../../PassportViewing/InDocUpdatePassImgViewDlg.h"
#include "../../PassportRecognizer/ui/OCRInDocUpdateDlg.h"
#include "../../PassportRecognizer/configuration/PassportRecognizerSettings.h"
#include "../../PassportRecognizer/configuration/PassportRecognizerSettingsStorage.h"
#include "../../PassportRecognizer/configuration/ui/PassportRecognizerCfgDlg.h"

#include "MainCfg.h"
#include "../../dispacher/NewInDocDispatcher.h"
#include "../../classes/match/CowMatcher.h"
#include "../../classes/forallactions/CountAction.h"
#include "../../CattleSQL.h"
#include "../../Utils/StringFormat.h"
#include "../../classes/TempStringLoader.h"
#include "../../hkey/KeyManager.h"
#include "../../cattlebase.h"

#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CNewInDocDlg, CBaseDocDlg )
	ON_COMMAND(IDM_SCANNPASSPORTS_UPDATE,OnBtnMenuScanUpdate)
	ON_COMMAND(IDM_SCANNPASSPORTS_CONFIGURE,OnBtnMenuScanConfigure)
END_MESSAGE_MAP()

CNewInDocDlg::CNewInDocDlg() : m_InCowList(&m_cows,CFGID_ILIST),
					CBaseDocDlg(&m_InCowList,Reason::REASON_IN,&m_InAccessor),
					m_prevOwner(NULL)					
{
	
	
}


void CNewInDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDocDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_ADDCOWS,m_Add);	
	DDX_Control(pDX,IDC_GETCOWS,m_ScanBtn);	
}



BOOL  CNewInDocDlg::OnInitDialog()
{
	CBaseDocDlg::OnInitDialog();

	int reasonid  = AppCfg().GetNum(CFG_PRESELINREASON);
	m_reasonCombo.ReloadCombo(reasonid);
	m_hentCombo.ReloadCombo();
	UpdateCowCount();
	UpdateSummary();

#ifdef SPECIAL_TRIAL_RELEASE
	m_ScanBtn.ShowWindow(SW_HIDE);
#endif

	m_loadStartTm.SetTime(Time::now());

	return TRUE ;
}

void CNewInDocDlg::AddCow()
{
	Cow *pNewCow = new Cow(NULL_COWID);
	CNewCowDlg newCowDlg(pNewCow,m_prevOwner,this);	 
	if(newCowDlg.DoModal() == IDOK)
	{
		m_prevOwner = pNewCow->GetFirstOwner();	
		while(!CheckCowDuplicates(pNewCow,NULL,NULL))
		{
			CEditCowDlg editCowDlg(pNewCow,this);
			if(editCowDlg.DoModal() != IDOK)
			{
				delete pNewCow;
				return;
			}
			else
				pNewCow->CommitChanges();
		}
		AddNewCow(pNewCow);
	}
	else
	{
		delete pNewCow;
	}
}


void CNewInDocDlg::DeleteCow(int iElement)
{
	Cow *pCow = m_cows[iElement];
	CString passImgPath;

	if(m_passImgs.Lookup(pCow,passImgPath))
	{
		if(!passImgPath.IsEmpty())
			DeleteFile((LPCSTR)passImgPath);
		m_passImgs.RemoveKey(pCow);				
	}
	m_cows.RemoveAt(iElement);
	delete pCow;			
}

void CNewInDocDlg::CancelDoc()
{
	m_cowlist->Lock();
	m_cows.FreeAndFlush();
	UpdateCowCount();
	m_cowlist->Unlock();
}

bool CNewInDocDlg::CommitDoc()
{
	CPassImgBase *imgBase = CPassImgBase::GetInstance();
	NullDoc *pNullDoc = CreateNewDoc();
	
	InDoc* pNewDoc = new InDoc(NULL_DOCID);
	auto_ptr<InDoc> deleteNewDoc(pNewDoc);
	pNewDoc->CopyFrom(*pNullDoc);
	delete pNullDoc;
	
	pNewDoc->GetCows()->Copy(m_cows);		

	if(!pNewDoc->InsertThisDoc(false)) 
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGDOC);
		SQLDB.Rollback();		
		return false;
	}
	Cow *pCow = NULL;
	CCowArray::ArrayIterator it = m_cows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		pCow->SetInDoc(pNewDoc);
		if(!pCow->InsertThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGCOW);
			SQLDB.Rollback();
			while(!it.isFirst())
				(*(--it))->SetInDoc(NULL);
			
			return false; 
		}
	}
	

	it.reset();
	CString passImgPath;
	while(it.hasNext())
	{
		pCow = *(it++);
		if(m_passImgs.Lookup(pCow,passImgPath))
		{
			if(!imgBase->StorePassImage(pCow,(LPCSTR)passImgPath,true))
			{
				if(AfxMessageBox(CStringFormat(CTempStringLoader(IDS_FAILDTOSTOREPASSIMGASK),(LPCSTR)pCow->GetEAN()),MB_YESNO) != IDYES)
				{
					SQLDB.Rollback();
					while(!it.isFirst())
					{
						pCow = (*(--it));
						if(pCow->HasPassImg())
							imgBase->DeletePassImg(pCow);
						pCow->RollbackChanges();
						pCow->SetInDoc(NULL);
						pCow->ResetId();
					}
					return false; 
				}
				else
				{
					pCow->RollbackChanges();
				}
			}
			else
			{
				if(!pCow->UpdateThisCow(false))
				{
					if(AfxMessageBox(CStringFormat(CTempStringLoader(IDS_FAILDTOASSIGNPASSIMGTOOWASK),
						(LPCSTR)pCow->GetEAN(),(LPCSTR)SQLDB.GetLastError()),MB_YESNO) != IDYES)
					{
						SQLDB.Rollback();
						while(!it.isFirst())
						{
							pCow = (*(--it)); 
							if(pCow->HasPassImg())
								imgBase->DeletePassImg(pCow);
							pCow->RollbackChanges();
							pCow->SetInDoc(NULL);
							pCow->ResetId();
						}
						return false; 
					}
					else
					{
						pCow->RollbackChanges();
					}
				}
			}
		}
	}

	m_cows.CommitChanges();
	SQLDB.Commit();
	deleteNewDoc.release();	
	NewInDocDispatcher::Dispatch(pNewDoc);
	return true;

}

CString CNewInDocDlg::GetDocDlgCaption()
{
	CString wndCaption;
	wndCaption.LoadString(IDS_INDOCNEW);
	return wndCaption;
}

void CNewInDocDlg::SetupBtns() 
{
	CString btnCaption;
	btnCaption.LoadString(IDS_BTNINSERT);
	m_OK.SetWindowText(btnCaption);

	
	btnCaption.LoadString(IDS_SCANBTNCAPTION);
	m_ScanBtn.SetButtonImage( IDB_SCANNER ,RGB(255,0,255));
	m_ScanBtn.SetWindowText(btnCaption);

	btnCaption.LoadString(IDS_ADDCOWBTNCAPTION);
	m_Add.SetButtonImage( IDB_ADDCOW );
	m_Add.SetWindowText(btnCaption);


}

CNewInDocDlg::~CNewInDocDlg()
{
	POSITION pos = m_passImgs.GetStartPosition();
	Cow *pCow = NULL;
	CString passImagePath;

	while(pos)
	{
		m_passImgs.GetNextAssoc(pos,pCow,passImagePath);
		if(!passImagePath.IsEmpty())
			DeleteFile(passImagePath);		
	}

	m_passImgs.RemoveAll();
	

}

bool CNewInDocDlg::CheckCowDuplicates(Cow *pCow,Cow *pSkipInList,Cow *pSkipInAryCows)
{
		
	CCowDataMatcher cowMatchEAN;
	cowMatchEAN.FindEAN(pCow->GetEAN());
	CCountAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> duplicateEANs(&cowMatchEAN);
	if(pSkipInList)
		duplicateEANs.AddSkip(pSkipInList);
	m_cows.ForAllAction(duplicateEANs);
	if(duplicateEANs.GetCount() > 0)
	{
		AfxMessageBox(IDS_DUPLEANONLIST);
		return false;
	}
	duplicateEANs.Reset();
	if(pSkipInAryCows)
		duplicateEANs.AddSkip(pSkipInAryCows);
	ary_cows.ForAllAction(duplicateEANs);
	if(duplicateEANs.GetCount() > 0)
	{

		duplicateEANs.Reset(false);
		ary_incows.ForAllAction(duplicateEANs);
		if(duplicateEANs.GetCount() > 0)
		{
			AfxMessageBox(IDS_DUPLEANNOTSOLD);
			return false;
		}
			return AfxMessageBox(IDS_DUPLEANINDB,MB_YESNO,0) == IDYES;
	}
	
	return true;
}


void CNewInDocDlg::EditCow(int nElement)
{
	Cow *pCow = m_cows[nElement];
	ASSERT(!pCow->IsUncommited());
	
	CEditCowDlg editCowDlg(pCow,this);
	while(true)
	{		
		if(editCowDlg.DoModal() == IDOK)
		{	
			if(CheckCowDuplicates(pCow,pCow,NULL))
			{
				pCow->CommitChanges();
				break;
			}			
		}
		else
		{
			pCow->RollbackChanges();
			break;
		}
	}
}

void CNewInDocDlg::GetCows()
{
#ifndef SPECIAL_TRIAL_RELEASE

	if(!GetApplicationKeyManager()->IsFeatureEnabled(FEATURE_OCR))
	{
		AfxMessageBox(IDS_FEATUREDISABLED);
		return;
	}

	CCowArray aScannedCows;
	CCowDataPassImgMap passMap;

	PassportRecognizerSettings recognizerSettings;
	PassportRecognizerSettingsStorage settingsStorage;
	settingsStorage.Load(recognizerSettings);

	COCRPassDlg scanPass(aScannedCows,m_cows,passMap,&recognizerSettings,this);
	
	Cow *pCow = NULL;
	CString passImgPath;

	if(scanPass.DoModal() == IDOK)
	{
		CCowArray::ArrayIterator it = aScannedCows.iterator();
		while(it.hasNext())
		{
			pCow = *(it++);
			AddNewCow(pCow);
			if(passMap.Lookup(pCow,passImgPath))
			{
				m_passImgs.SetAt(pCow,passImgPath);
			}			
		}
		UpdateCowCount();
	} 

	CHentCombo::NotifyReload();
#endif
}



void CNewInDocDlg::ShowPass(int nItem)
{	
	CInDocUpdatePassImgViewDlg passView(m_cows,nItem,m_passImgs,this);
	passView.DoModal();

}

void CNewInDocDlg::OnBtnMenuScanConfigure()
{
	PassportRecognizerSettings recognizerSettings;
	PassportRecognizerSettingsStorage settingsStorage;
	settingsStorage.Load(recognizerSettings);
	CPassportRecognizerCfgDlg recognizerCfgDlg(&recognizerSettings,this);
	if(recognizerCfgDlg.DoModal() == IDOK)
	{
		settingsStorage.Save(recognizerSettings);
	}
}

void CNewInDocDlg::OnBtnMenuScanUpdate()
{	
#ifndef SPECIAL_TRIAL_RELEASE
	if(!GetApplicationKeyManager()->IsFeatureEnabled(FEATURE_OCR))
	{
		AfxMessageBox(IDS_FEATUREDISABLED);
		return;
	}

	
	CCowDataPassImgMap passMap;		
	CString passImgPath;
	Cow* pCow;
	CCowArray updateCows;

	PassportRecognizerSettings recognizerSettings;
	PassportRecognizerSettingsStorage settingsStorage;
	settingsStorage.Load(recognizerSettings);


	CCowArray::ArrayIterator it = m_cows.iterator();
	while(it.hasNext()) //find cows without passport
	{
		pCow = *(it++);
		if(!m_passImgs.PLookup(pCow) && !pCow->HasPassImg())
		{
			updateCows.Add(pCow);
		}
	}
	
	

	COCRInDocUpdateDlg upddateInDoc(updateCows,m_cows,passMap,&recognizerSettings,this);

	if(upddateInDoc.DoModal() == IDOK)
	{
		it = m_cows.iterator();
		while(it.hasNext())
		{
			pCow = *(it++);
			if(passMap.Lookup(pCow,passImgPath))
			{				
				m_passImgs.SetAt(pCow,passImgPath);
			}
		}			
	}	
	CHentCombo::NotifyReload();

#endif
}

void CNewInDocDlg::AddNewCow(Cow *pCow)
{
	ChangeGrp4Cow(pCow,GetActiveGroup());
	m_cows.Add(pCow);
	UpdateSummary();

}
void CNewInDocDlg::ChangeGrp4Cow(Cow *pCow,int grp)
{
	pCow->SetInGrp(grp);
}

void CNewInDocDlg::FindPass()
{
	CPassSearchDlg passSearch(m_cows,&m_InAccessor,this);
	passSearch.DoModal();
}