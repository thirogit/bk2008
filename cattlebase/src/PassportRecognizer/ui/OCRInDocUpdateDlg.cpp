#include "stdafx.h"
#include "OCRInDocUpdateDlg.h"
#include "../../Cow/EditCowDlg.h"
#include "ChooseCowEANDlg.h"
#include "../../log.h"
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(COCRInDocUpdateDlg, COCRPassDlg)
	ON_BN_CLICKED(IDC_ADDCOW, OnUpdateCow)	
END_MESSAGE_MAP()

COCRInDocUpdateDlg::COCRInDocUpdateDlg(CCowArray &inArray,
									   CCowArray &sesArray,
									   CCowDataPassImgMap &passMap,
									   PassportRecognizerSettings* pSettings,
									   CWnd* pParent/* = NULL*/) :
									   COCRPassDlg(inArray,sesArray,passMap,pSettings,pParent)
{

}




void COCRInDocUpdateDlg::OnUpdateCow()
{
	LRESULT lRes;
	
	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	bool bNoUpdateIfPresent = dataSettings.GetDoNotUpdateValueIfPresent();
	
	if(ValidateDataFields())
	{	
		Cow *pCowFromFields = CreateCowFromFields();
		std::auto_ptr<Cow> autoDeleteCowFromFields(pCowFromFields);
		Cow *pCowToUpdate = m_outArray.GetCowByEAN(pCowFromFields->GetEAN());
		if(!pCowToUpdate)
		{
			AfxMessageBox(IDS_ERRINDOCUPDATECOWNOTFOUND,MB_OK,0);
			return;
		}
		LOG("Updating passimg for %s.",(LPCSTR)pCowToUpdate->GetEAN());

		
		if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE) && (pCowToUpdate->GetPassDate().IsNull() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangePassDate(pCowFromFields->GetPassDate());

		if(dataSettings.IsFieldEnabled(DataSettings::BIRTH) && (pCowToUpdate->GetBirthDate().IsNull() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangeBirthDate(pCowFromFields->GetBirthDate());

		if(dataSettings.IsFieldEnabled(DataSettings::BIRTH) && (pCowToUpdate->GetBirthplace().IsEmpty() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangeBirthPlace(pCowFromFields->GetBirthplace());

		if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN) && (pCowToUpdate->GetMotherEAN().IsEmpty() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangeMotherEAN(pCowFromFields->GetMotherEAN());

		if(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER) && (!pCowToUpdate->HasFirstOwner() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangeFirstOwner(pCowFromFields->GetFirstOwner());

		if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO) && (pCowToUpdate->GetPassNo().IsEmpty() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangePassNo(pCowFromFields->GetPassNo());

		if(dataSettings.IsFieldEnabled(DataSettings::SEX) && (pCowToUpdate->GetSex().IsUNK() || !bNoUpdateIfPresent))
			pCowToUpdate->ChangeSex(pCowFromFields->GetSex());

		if(dataSettings.IsFieldEnabled(DataSettings::STOCK) && !bNoUpdateIfPresent)
			pCowToUpdate->ChangeStock(pCowFromFields->GetStock());		


		SavePassImg(pCowToUpdate);		
		m_showTab.SetCurSel(0);	
		OnSelChange(NULL,&lRes);
		OnSelChanging(NULL,&lRes);			
	}	

}


bool COCRInDocUpdateDlg::DoManualInput()
{
	bool bRet = false;
	Cow *pCowToManualInput = NULL;
	LRESULT lRes;

	pCowToManualInput = CChooseCowEANDlg::ChooseCowEAN(m_outArray,this);
	if(pCowToManualInput)
	{
		CEditCowDlg editCowDlg(pCowToManualInput,this);
		editCowDlg.LockEANEdit();

		if(editCowDlg.DoModal() == IDOK)
		{			
			SavePassImg(pCowToManualInput);	
			m_showTab.SetCurSel(0);	
			OnSelChange(NULL,&lRes);
			OnSelChanging(NULL,&lRes);
			bRet = true;
			
		}
		
	}

	return bRet;
}

 bool COCRInDocUpdateDlg::EditCow(Cow *pCow)
 {
	CEditCowDlg editCowDlg(pCow,this);
	editCowDlg.LockEANEdit();

	return (editCowDlg.DoModal() == IDOK);
 }