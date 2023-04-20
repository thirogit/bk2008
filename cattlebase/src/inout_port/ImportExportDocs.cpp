#include "stdafx.h"
#include "ImportExportDocs.h"
#include "../PassportStorage/PassImgBase.h"
#include "../missing/util.h"
#include "../missing/paths.h"
#include "../exceptions/StrException.h"
#include "../consts.h"
#include "../classes/TempStringLoader.h"

#include <memory>
#include <set>
#include <boost/regex.hpp>
#include "log.h"
#include "../exceptions/DateTimeException.h"
#include "../datatypes/NullInvoice.h"
#include "../datatypes/CowInvoiceEntryArray.h"
#include "../datatypes/InDoc.h"
#include "ImportExportTasks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////////////////


ExportCowAccessor::ExportCowAccessor(CowAccessor* pAccessor) : m_pAccessor(pAccessor)
{
}

NullInt ExportCowAccessor::GetGroup(const Cow* pCow)
{
	return m_pAccessor->GetGroup(pCow);
}

//////////////////////////////////////////////////////////////////////////////////////////

ExportInDocCowAccessor::ExportInDocCowAccessor(const ExportSettings& exportSettings) : 
	ExportCowAccessor(&m_InAccessor),m_exportSettings(exportSettings)
{
	
}

RRInvoice* ExportInDocCowAccessor::GetRRInvoice(const Cow* pCow)
{
	return pCow->GetRRInvoice();
}

Money ExportInDocCowAccessor::GetExportMyPrice(const Cow* pCow)
{
	bool bExportPriceAsMyPrice = m_exportSettings.bExportSellBuyPrice;
	bool bExportMyPriceAsMyPrice = m_exportSettings.bExportMyPriceAsMyPrice;
	Money exportMyPrice;
	if(!(m_exportSettings.bExportInvoices && pCow->HasRRBuyInvoice()))
	{
		if(pCow->HasAnyBuyInvoice())
		{			
			if(bExportPriceAsMyPrice) 
				exportMyPrice =  pCow->GetBuyPrice();
			else
			{
				if(bExportMyPriceAsMyPrice) exportMyPrice =  pCow->GetMyPrice();
			}								
		}			
	}		
	return exportMyPrice;

}

Stock* ExportInDocCowAccessor::GetExportStock(const Cow* pCow)
{
	bool bExportBuySellValues = m_exportSettings.bExportBuySellValues;
	Stock* exportStock = pCow->GetStock();
	if(!(m_exportSettings.bExportInvoices && pCow->HasRRBuyInvoice()))
	{
		if(pCow->HasAnyBuyInvoice() && bExportBuySellValues)
		{
			exportStock = pCow->GetBuyStock();
		}		
	}
	return exportStock;
}

Weight ExportInDocCowAccessor::GetExportWeight(const Cow* pCow)
{
	bool bExportBuySellValues = m_exportSettings.bExportBuySellValues;

	Weight exportWeight = pCow->GetWeight();	
	if(!(m_exportSettings.bExportInvoices && pCow->HasRRBuyInvoice()))
	{
		if(pCow->HasAnyBuyInvoice() && bExportBuySellValues)
		{
			exportWeight = Weight(pCow->GetBuyWeight().GetDouble());
		}				
	}
	return exportWeight;
}


/////////////////////////////////////////////////////////////////////////////////////////

ExportOutDocCowAccessor::ExportOutDocCowAccessor(const ExportSettings& exportSettings) : 
	ExportCowAccessor(&m_OutAccessor),m_exportSettings(exportSettings)
{

}

RRInvoice* ExportOutDocCowAccessor::GetRRInvoice(const Cow* pCow)
{
	return NULL;
}

Money ExportOutDocCowAccessor::GetExportMyPrice(const Cow* pCow)
{
	bool bExportPriceAsMyPrice = m_exportSettings.bExportSellBuyPrice;
	bool bExportMyPriceAsMyPrice = m_exportSettings.bExportMyPriceAsMyPrice;
	Money exportMyPrice;
	if(pCow->HasVATSellInvoice())
	{
		if(bExportPriceAsMyPrice)  
			exportMyPrice =  pCow->GetSellPrice();
		else
		{
			if(bExportMyPriceAsMyPrice) 
				exportMyPrice =  pCow->GetMyPrice();
		}		
	}		
	return exportMyPrice;
}

Stock* ExportOutDocCowAccessor::GetExportStock(const Cow* pCow)
{
	bool bExportBuySellValues = m_exportSettings.bExportBuySellValues;
	Stock* exportStock = pCow->GetStock();
	if(pCow->HasVATSellInvoice() && bExportBuySellValues)
	{
		exportStock = pCow->GetSellStock();
	}		
	return exportStock;

}

Weight ExportOutDocCowAccessor::GetExportWeight(const Cow* pCow)
{
	bool bExportBuySellValues = m_exportSettings.bExportBuySellValues;

	Weight exportWeight = pCow->GetWeight();	
	if(pCow->HasVATSellInvoice() && bExportBuySellValues)
	{
		exportWeight = Weight(pCow->GetSellWeight().GetDouble());
	}
	return exportWeight;
}

/////////////////////////////////////////////////////////////////////////////////////////
CImportExportDocs::CImportExportDocs()
{
	m_tmpSavePath = CUtil::GetAppDir() + "\\EXPORTTMPDIR";
	paths_CreateIfNeeded(m_tmpSavePath);

	
}

bool CImportExportDocs::WriteDoc(ExportDocContext& outDocContext,ITaskProgres *progress)
{
	CFile docExportFile;

	PtrFlushArray<DOC_FILERECORD> docs;
	PtrFlushArray<DOC_FILERECORD>::AutoArray docsDel(docs);

	PtrFlushArray<HENT_FILERECORD> hents;
	PtrFlushArray<HENT_FILERECORD>::AutoArray hentsDel(hents);

	PtrFlushArray<STOCK_FILERECORD> stocks;
	PtrFlushArray<STOCK_FILERECORD>::AutoArray stocksDel(stocks);

	PtrFlushArray<INVOICE_FILERECORD> rrinvs;
	PtrFlushArray<INVOICE_FILERECORD>::AutoArray rrinvsDel(rrinvs);

	
	int i,s;	
	Doc *pDoc = NULL;
	Cow *pCow = NULL;
	
	if(!docExportFile.Open((LPCSTR)outDocContext.docFilePath,CFile::modeCreate | CFile::modeWrite))
	{
		progress->OnTaskError(CTempStringLoader(IDS_CANTOPENOUTPUTFILE));
		return false;
	}

	

	progress->SetTask(WRITE_PRCSNG_DOCS,outDocContext.docPool.GetSize());
	CDocDataArray::ArrayIterator docIt = outDocContext.docPool.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		DOC_FILERECORD* fileRecord = 
			(DOC_FILERECORD*)(new BYTE[sizeof(DOC_FILERECORD_HEADER) +
			pDoc->GetCowCount()*sizeof(COW_FILERECORD)]);

		DOC_FILERECORD_HEADER* docFileHeader = (DOC_FILERECORD_HEADER*)fileRecord;
		CreateDocFileRecord(docFileHeader,pDoc);

		COW_FILERECORD* cowFileRecord = (COW_FILERECORD*)(docFileHeader+1);
		CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
		while(cowIt.hasNext())
		{
			pCow = *(cowIt++);
			CreateCowFileRecord(cowFileRecord,pCow,outDocContext.pCowAccessor);
			cowFileRecord->haspassimg = outDocContext.cowPassMap.PLookup(pCow->GetId()) != NULL;
			cowFileRecord++;
		}
		docs.Add(fileRecord);
		if(!progress->OnStep(docIt.index())) return false;
	}

	i = 0;
	progress->SetTask(WRITE_PRCSNG_RRIVS,outDocContext.rrInvPool.size()); 
	for(std::set<RRInvoice*>::iterator rrPoolIt = outDocContext.rrInvPool.begin();
		rrPoolIt != outDocContext.rrInvPool.end();
		rrPoolIt++)
	{		
		CowInvoiceEntry *pEntry;
		Invoice* pRRInvoice = (*rrPoolIt);
		INVOICE_FILERECORD* fileRecord = 
			(INVOICE_FILERECORD*)(new BYTE[sizeof(INVOICE_FILERECORD_HEADER) + 
											pRRInvoice->GetCowQty()*sizeof(COWINVOICEENTRY_FILERECORD)]);

		INVOICE_FILERECORD_HEADER* invoiceFileHeader = (INVOICE_FILERECORD_HEADER*)fileRecord;
		CreateInvoiceFileRecord(invoiceFileHeader,pRRInvoice);

		COWINVOICEENTRY_FILERECORD* pCowInvEntry = (COWINVOICEENTRY_FILERECORD*)(invoiceFileHeader+1);
		CCowInvoiceEntryArray::ArrayIterator entryIt = pRRInvoice->invEntries.iterator();
		while(entryIt.hasNext())
		{
			 pEntry = *(entryIt++);
			 CreateCowPriceEntry(pCowInvEntry,pEntry);
			 pCowInvEntry++;
		}

		rrinvs.Add(fileRecord);
		if(!progress->OnStep(i++)) return false;
	}
	i = 0;
	progress->SetTask(WRITE_PRCSNG_HENTS,outDocContext.hentPool.size());
	for(CHentPool::iterator hentPoolIt = outDocContext.hentPool.begin();hentPoolIt != outDocContext.hentPool.end();hentPoolIt++)
	{
		hents.Add(CreateHentFileRecord(*hentPoolIt));
		if(!progress->OnStep(i)) return false;
		i++;
	}

	i = 0;
	progress->SetTask(WRITE_PRCSNG_STOCKS,outDocContext.stockPool.size());
	for(CStockPool::iterator stockPoolIt = outDocContext.stockPool.begin();stockPoolIt != outDocContext.stockPool.end();stockPoolIt++)
	{
		stocks.Add(CreateStockFileRecord(*stockPoolIt));
		if(!progress->OnStep(i)) return false;
		i++;
	}

	int p = 0;
	FILEHEADER fileHeader;
	strncpy(fileHeader.fileid,EXPORTIMPORT_FILE_ID,sizeof(fileHeader.fileid));
	fileHeader.docscount = docs.GetSize();
	fileHeader.stockcount = outDocContext.stockPool.size();
	fileHeader.hentcount = outDocContext.hentPool.size();
	fileHeader.cowpasscount = outDocContext.cowPassMap.GetSize();
	fileHeader.rrinv_count = rrinvs.GetSize();
	fileHeader.checksum = 0;

	
	const Hent *pCompany = &outDocContext.company;
	if(pCompany->GetFarmNo().GetLength() != EAN_LENGTH)
	{
		progress->OnTaskError(CTempStringLoader(IDS_EMPTYCOMPANYFARMNO));
		return false;
	}
	
	HENT_FILERECORD *company = CreateHentFileRecord(pCompany);
	memcpy(&fileHeader.company,company,sizeof(HENT_FILERECORD));
	delete company;

	
	
	docExportFile.Write(&fileHeader,sizeof(FILEHEADER));

	progress->SetTask(WRITE_WRITING_TO_FILE,fileHeader.docscount +
											fileHeader.stockcount +
											fileHeader.hentcount +
											fileHeader.cowpasscount +
											fileHeader.rrinv_count);
	

	for(i = 0,s = stocks.GetSize();i < s;i++,p++)
	{
		docExportFile.Write(stocks[i],sizeof(STOCK_FILERECORD));
		if(!progress->OnStep(p)) return false;
	}
	for(i = 0,s = hents.GetSize();i < s;i++,p++)
	{		
		docExportFile.Write(hents[i],sizeof(HENT_FILERECORD));
		if(!progress->OnStep(p)) return false;
	}

	for(i = 0,s = docs.GetSize();i < s;i++,p++)
	{
		docExportFile.Write(docs[i],sizeof(DOC_FILERECORD_HEADER) + sizeof(COW_FILERECORD)*docs[i]->header.cowcount);
		if(!progress->OnStep(p)) return false;
	}

	for(i = 0,s = rrinvs.GetSize();i < s;i++,p++)
	{
		docExportFile.Write(rrinvs[i],sizeof(INVOICE_FILERECORD_HEADER) + sizeof(COWINVOICEENTRY_FILERECORD)*rrinvs[i]->header.entriescount);
		if(!progress->OnStep(p)) return false;
	}

	BYTE transferBuffer[1024];
	UINT bytesread,totalbytes;

	CFile passImgFile;
	PASSIMG_FILERECORDHEADER imgHeader;
	POSITION mappos = outDocContext.cowPassMap.GetStartPosition();
	int cowid;
	CString passpath;

	while(mappos)
	{
		totalbytes = 0;
		outDocContext.cowPassMap.GetNextAssoc(mappos,cowid,passpath);
		imgHeader.cowid = cowid;
		if(passImgFile.Open(passpath,CFile::modeRead))
		{
			imgHeader.imgsize = passImgFile.GetLength();
			docExportFile.Write(&imgHeader,sizeof(PASSIMG_FILERECORDHEADER));
			do
			{
				bytesread = passImgFile.Read(transferBuffer,sizeof(transferBuffer));
				totalbytes += bytesread;
				docExportFile.Write(transferBuffer,bytesread);

			} while(bytesread > 0);

			ASSERT(totalbytes == imgHeader.imgsize);
			passImgFile.Close();
			DeleteFile(passpath);
		}
		if(!progress->OnStep(p)) return false;
		p++;
	}
		
	docExportFile.Close();

	return true;

}

bool CImportExportDocs::SaveCowPassImg(Cow *pCow,CString &passFile)
{
	CPassImgBase * pImgBase = CPassImgBase::GetInstance();
	fipWinImageEx passImg;
	if(pImgBase->GetPassImage(passImg,pCow))
	{
		CString  tmpPassImgPath = m_tmpSavePath + "\\" + pCow->GetEAN() + ".JPG";		
		if(passImg.save(tmpPassImgPath))
		{
			passFile = tmpPassImgPath;
			return true;			
		}
	}
	return false;
}


void CImportExportDocs::SetFlatDate(FLAT_DATE& flatDt,const CDateTime& dt)
{
	memset(&flatDt,0,sizeof(FLAT_DATE));
	flatDt.null = dt.IsNull();
	if(!flatDt.null)
	{
		flatDt.year = dt.GetYear();
		flatDt.month = dt.GetMonth();
		flatDt.day = dt.GetDay();
		flatDt.hour = dt.GetHour();
		flatDt.minutes = dt.GetMinutes();
		flatDt.seconds = dt.GetSeconds();		
	}
}

CDateTime CImportExportDocs::ConvertToDateTime(const FLAT_DATE& flatDt)
{
	CDateTime dt;
	if(!flatDt.null)
	{
		dt.SetYearMonthDay(flatDt.year,flatDt.month,flatDt.day,true);
		dt.SetHourMinSec(flatDt.hour,flatDt.minutes,flatDt.seconds,true);
	}
	else
		dt.MakeNull();

	return dt;
}
 void CImportExportDocs::CreateCowPriceEntry(COWINVOICEENTRY_FILERECORD *fileRecord,CowInvoiceEntry *pEntry)
 {
	 fileRecord->cowid = pEntry->GetCow()->GetId();
	 fileRecord->price = pEntry->GetPrice().GetDouble();
	 fileRecord->stock = pEntry->GetStock()->GetId();
	 fileRecord->weight = pEntry->GetWeight().GetDouble();
 }

void CImportExportDocs::CreateCowFileRecord(COW_FILERECORD* fileRecord,Cow *pCow, ExportCowAccessor* pAccessor)
{

	memset(fileRecord,0,sizeof(COW_FILERECORD));
	
	fileRecord->animalid = pCow->GetId();
	SetFlatDate(fileRecord->birthdate,pCow->GetBirthDate());
	strncpy(fileRecord->birthplace,(LPCSTR)pCow->GetBirthplace(),sizeof(fileRecord->birthplace));
	strncpy(fileRecord->ean,(LPCSTR)pCow->GetEAN(),sizeof(fileRecord->ean));
	strncpy(fileRecord->extras,(LPCSTR)pCow->GetExtras(),sizeof(fileRecord->extras));	
	if(pCow->HasFirstOwner())
		fileRecord->fstownr = pCow->GetFirstOwner()->GetId();
		
	strncpy(fileRecord->motherean,(LPCSTR)pCow->GetMotherEAN(),sizeof(fileRecord->motherean));
	fileRecord->sex = (int)pCow->GetSex().getCOWSEX();
	strncpy(fileRecord->passno,(LPCSTR)pCow->GetPassNo(),sizeof(fileRecord->passno));
	SetFlatDate(fileRecord->passdate,pCow->GetPassDate());


	fileRecord->myprice = pAccessor->GetExportMyPrice(pCow).GetDouble();
	fileRecord->ingrp = pAccessor->GetGroup(pCow).ToInt(NULL_INOUT_GRP);
	

	RRInvoice* pRRInv = pAccessor->GetRRInvoice(pCow);
	if(pRRInv)
	{
		fileRecord->rrinvoiceid = pRRInv->GetId();
	}	

	fileRecord->stock = pAccessor->GetExportStock(pCow)->GetId();
	fileRecord->weight = pAccessor->GetExportWeight(pCow).GetDouble();

	SetFlatGeoCoord(fileRecord->longitude,pCow->GetLongitude());
	SetFlatGeoCoord(fileRecord->latitude,pCow->GetLatitude());

}


void CImportExportDocs::CreateDocFileRecord(DOC_FILERECORD_HEADER* fileRecord,Doc *pDoc)
{
	memset(fileRecord,0,sizeof(DOC_FILERECORD_HEADER));

	fileRecord->hent = pDoc->GetHent()->GetId();
	fileRecord->reason = pDoc->GetReason()->GetId();	
	strncpy(fileRecord->plateno,(LPCSTR)pDoc->GetPlateNo(),sizeof(fileRecord->plateno));
	SetFlatDate(fileRecord->loaddate,pDoc->GetLoadDate());
	if(pDoc->HasInvoice())
		fileRecord->invoice = pDoc->GetInvoice()->GetId();
	strncpy(fileRecord->extras,(LPCSTR)pDoc->GetExtras(),sizeof(fileRecord->extras));
	fileRecord->docid = pDoc->GetId();
	SetFlatDate(fileRecord->docdate,pDoc->GetDocDate());
	fileRecord->cowcount = pDoc->GetCowCount();

	fileRecord->loadstart = pDoc->GetLoadStartTime().ToInt(NULL_TIME_VALUE);
	fileRecord->loadend = pDoc->GetLoadEndTime().ToInt(NULL_TIME_VALUE);
}

STOCK_FILERECORD* CImportExportDocs::CreateStockFileRecord(Stock *pStock)
{
	STOCK_FILERECORD* fileRecord = new STOCK_FILERECORD;
	
	memset(fileRecord,0,sizeof(STOCK_FILERECORD));	
	fileRecord->predefsex = (int)pStock->GetPredefSex().getCOWSEX();
	strncpy(fileRecord->stockcode,(LPCSTR)pStock->GetStockCode(),sizeof(fileRecord->stockcode));
	strncpy(fileRecord->stockname,(LPCSTR)pStock->GetStockName(),sizeof(fileRecord->stockname));
	fileRecord->stockid = pStock->GetId();
	return fileRecord;
}

HENT_FILERECORD* CImportExportDocs::CreateHentFileRecord(const Hent *pHent)
{
	HENT_FILERECORD* fileRecord = new HENT_FILERECORD;
	
	memset(fileRecord,0,sizeof(HENT_FILERECORD));

	fileRecord->hentid = pHent->GetId();
	fileRecord->country = pHent->GetCountry()->GetCountryISONumber();
	fileRecord->henttype = pHent->GetHentType()->henttypeid;
	SetFlatDate(fileRecord->issuedate,pHent->GetIssueDate());
	strncpy(fileRecord->name,(LPCSTR)pHent->GetName(),sizeof(fileRecord->name));
	strncpy(fileRecord->alias,(LPCSTR)pHent->GetAlias(),sizeof(fileRecord->alias));
	strncpy(fileRecord->street,(LPCSTR)pHent->GetStreet(),sizeof(fileRecord->street));
	strncpy(fileRecord->pobox,(LPCSTR)pHent->GetPOBox(),sizeof(fileRecord->pobox));
	strncpy(fileRecord->city,(LPCSTR)pHent->GetCity(),sizeof(fileRecord->city));
	strncpy(fileRecord->zip,(LPCSTR)pHent->GetZip(),sizeof(fileRecord->zip));
	strncpy(fileRecord->phone,(LPCSTR)pHent->GetPhone(),sizeof(fileRecord->phone));
	strncpy(fileRecord->nip,(LPCSTR)pHent->GetNIP(),sizeof(fileRecord->nip));
	strncpy(fileRecord->arimrno,(LPCSTR)pHent->GetFarmNo(),sizeof(fileRecord->arimrno));
	strncpy(fileRecord->wetno,(LPCSTR)pHent->GetWetNo(),sizeof(fileRecord->wetno));
	strncpy(fileRecord->extras,(LPCSTR)pHent->GetExtras(),sizeof(fileRecord->extras));	
	strncpy(fileRecord->plate,(LPCSTR)pHent->GetPlate(),sizeof(fileRecord->plate));	
	strncpy(fileRecord->pesel,(LPCSTR)pHent->GetPESEL(),sizeof(fileRecord->pesel));
	strncpy(fileRecord->regon,(LPCSTR)pHent->GetREGON(),sizeof(fileRecord->regon));
	strncpy(fileRecord->idno,(LPCSTR)pHent->GetIdNo(),sizeof(fileRecord->idno));	
	strncpy(fileRecord->issuepost,(LPCSTR)pHent->GetIssuePost(),sizeof(fileRecord->issuepost));
	strncpy(fileRecord->accountno,(LPCSTR)pHent->GetAccountNo(),sizeof(fileRecord->accountno));
	strncpy(fileRecord->bankname,(LPCSTR)pHent->GetBankName(),sizeof(fileRecord->bankname));

	SetFlatGeoCoord(fileRecord->longitude,pHent->GetLongitude());
	SetFlatGeoCoord(fileRecord->latitude,pHent->GetLatitude());

	strncpy(fileRecord->cellphone,pHent->GetCellPhoneNo(),sizeof(fileRecord->cellphone));
	strncpy(fileRecord->emailaddress,pHent->GetEmailAddress(),sizeof(fileRecord->emailaddress));
	strncpy(fileRecord->wetlicenceno,pHent->GetWetLicenceNo(),sizeof(fileRecord->wetlicenceno));

	return fileRecord;

}

bool CImportExportDocs::WriteInDoc(CDocDataArray &indocsArray,const ExportSettings& exportSettings,const CString &outputFile,ITaskProgres *progress)
{
	ExportDocContext docContext;
	Hent companyHent(NULL_HENTID);

	docContext.company.CopyFrom(*Company::GetCompany()->GetAsHent());
	docContext.docFilePath = outputFile;
	docContext.pCowAccessor = new ExportInDocCowAccessor(exportSettings);

	/*Doc* pDoc;
	std::set<VATInvoiceData*> vatInvoices;
	CDocDataArray::ArrayIterator docIt = indocsArray.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		if(pDoc->HasInvoice())
		{
			vatInvoices.insert(pDoc->GetInvoice());	
		}
	}


	VATInvoiceData* pInvoice;
	std::set<Doc*> invoiceDocs;
	std::set<VATInvoiceData*>::iterator vatInvIt = vatInvoices.begin();
	while(vatInvIt != vatInvoices.end())
	{
		pInvoice = *vatInvIt;

		docIt = pInvoice->invoiceDocs.iterator();
		while(docIt.hasNext())
		{
			invoiceDocs.insert(*(docIt++));
		}
		vatInvIt++;
	}

	
	docIt = indocsArray.iterator();
	while(docIt.hasNext())
	{
		invoiceDocs.erase(*(docIt++));		
	}

	if(!invoiceDocs.empty())
	{
		CString sError = CTempStringLoader(IDS_EXPORTWITHVATINVSERROR);
		std::set<Doc*>::iterator invoiceDocsIt = invoiceDocs.begin();
		while(invoiceDocsIt != invoiceDocs.end())
		{
			sError += (*invoiceDocsIt)->GetDocNoStr();
			invoiceDocsIt++;

			if(invoiceDocsIt != invoiceDocs.end())
				sError += ", ";
		}

		progress->OnTaskError(sError);
		return false;
	}*/

	CreateDocContext(docContext,exportSettings,indocsArray);
	
	bool bWriteResult = WriteDoc(docContext,progress);

	delete docContext.pCowAccessor;

	return bWriteResult;
}


void CImportExportDocs::CreateDocContext(ExportDocContext &docContext,const ExportSettings& exportSettings,CDocDataArray &docsArray)
{
	CCowArray cowPool;
	
	Doc* pDoc = NULL;
	CDocDataArray::ArrayIterator docIt = docsArray.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		docContext.docPool.Add(pDoc);
		docContext.hentPool.insert(pDoc->GetHent());
		cowPool.AppendPtr(pDoc->GetCows());
	}

	CString passFile;
	bool bExportPassImgs = exportSettings.bExportPassImages;
	Cow* pCow = NULL;
	RRInvoice* rrInv = NULL;
	CowInvoiceEntry *pEntry = NULL;
	CCowArray::ArrayIterator cowIt = cowPool.iterator();	
	while(cowIt.hasNext())
	{		
		pCow = *(cowIt++);
		
		if(pCow->HasFirstOwner())
			docContext.hentPool.insert(pCow->GetFirstOwner());

		docContext.stockPool.insert(docContext.pCowAccessor->GetExportStock(pCow));

		rrInv = docContext.pCowAccessor->GetRRInvoice(pCow);

		if(rrInv)
		{
			docContext.rrInvPool.insert(rrInv);
			pEntry = rrInv->GetEntryForCow(pCow);
			docContext.stockPool.insert(pEntry->GetStock());			
		}

		if(bExportPassImgs && pCow->HasPassImg())
		{
			if(SaveCowPassImg(pCow,passFile))
			{
				docContext.cowPassMap.SetAt(pCow->GetId(),passFile);
				docContext.passTmpFilesDeleter.AddFile(passFile);
			}
		}
	}
}

bool CImportExportDocs::WriteOutDoc(CDocDataArray &outdocsArray,const ExportSettings& exportSettings,const CString &outputFile,ITaskProgres *progress)
{
	ExportDocContext docContext;
	
	docContext.company.CopyFrom(*Company::GetCompany()->GetAsHent());
	docContext.docFilePath = outputFile;
	docContext.pCowAccessor = new ExportOutDocCowAccessor(exportSettings);

	CreateDocContext(docContext,exportSettings,outdocsArray);
	
	bool bWriteResult = WriteDoc(docContext,progress);

	delete docContext.pCowAccessor;

	return bWriteResult;
}

CString CImportExportDocs::CreateCompanyAlias(const CString& companyName)
{
	CString companyAlias,companyNameUpper = CUtil::UnPolishString(companyName);
	companyNameUpper.MakeUpper();
	for(int i = 0,s = companyNameUpper.GetLength();i < s && i < MAXALIAS;i++)
	{
		char aliasChar = companyNameUpper.GetAt(i);
		if(isalpha(aliasChar))
			companyAlias += aliasChar;
	}
	return companyAlias;
}

bool CImportExportDocs::ReadInDoc(ImportDocContext& docContext,ITaskProgres *progress)
{	
	CDocDataArray docs;
	CCowArray cows;
	CHentArray hents;
	CStockDataArray stocks;
	CInvoiceDataArray rrinvs;
	CPassCowMap cowPassMap;
	
	CDocDataArray::AutoArray delDocs(docs);
	CCowArray::AutoArray delCows(cows);
	CHentArray::AutoArray delHents(hents);
	CStockDataArray::AutoArray delStocks(stocks);
	CInvoiceDataArray::AutoArray delRRInvs(rrinvs);

	UINT bytesRead;
	COW_FILERECORD FileCow;
	HENT_FILERECORD FileHent;
	STOCK_FILERECORD FileStock;
	
	PASSIMG_FILERECORDHEADER FilePass;

	FILEHEADER fileHeader;

	CFile docImportFile;
	CFileDeleter tmpFileDeleter;
	
	if(!docImportFile.Open((LPCSTR)docContext.docFilePath,CFile::modeRead))
	{
		progress->OnTaskError(CTempStringLoader(IDS_ERRORCANTOPENINPUTFILE));
		return false;
	}

	progress->SetTask(READ_READING_HEADER,1);	
	bytesRead  = docImportFile.Read(&fileHeader,sizeof(FILEHEADER));
	if(bytesRead != sizeof(FILEHEADER) ||
	   strncmp(EXPORTIMPORT_FILE_ID,fileHeader.fileid,sizeof(fileHeader.fileid)) != 0)
	{
		progress->OnTaskError(CTempStringLoader(IDS_OTHERVERSIONFILE));
		return false;
	}	
	if(!progress->OnStep(1)) return false;


	Hent* pCompany = CreateHentRecord(&fileHeader.company);
	
	if(!pCompany)
	{
		LOG("INVALID COMPANY INFO."); 
		progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
		return false;
	}

	pCompany->SetAlias(CreateCompanyAlias(pCompany->GetName()));
	std::auto_ptr<Hent> companyDelete(pCompany);

	progress->SetTask(READ_PRCSNG_STOCKS,fileHeader.stockcount);
	Stock *pStock = NULL;
	for(int iStock = 0;iStock < fileHeader.stockcount;iStock++)
	{
		bytesRead  = docImportFile.Read(&FileStock,sizeof(STOCK_FILERECORD));
		if(bytesRead != sizeof(STOCK_FILERECORD))
		{
			LOG("CANT READ STOCK FILE RECORD."); 
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}

		pStock = CreateStockDataRecord(&FileStock);
		
		if(!pStock) 
		{
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}

		stocks.Add(pStock);
		if(!progress->OnStep(iStock)) return false;
	}

	progress->SetTask(READ_PRCSNG_HENTS,fileHeader.hentcount);
	Hent *pHent;
	for(int iHent = 0;iHent < fileHeader.hentcount;iHent++)
	{
		bytesRead  = docImportFile.Read(&FileHent,sizeof(HENT_FILERECORD));
		if(bytesRead != sizeof(HENT_FILERECORD))
		{
			LOG("CANT READ HENT FILE RECORD.");
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}
		pHent = CreateHentRecord(&FileHent);
		if(!pHent)
		{
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}

		hents.Add(pHent);
		if(!progress->OnStep(iHent)) return false;
	}


	progress->SetTask(READ_PRCSNG_DOCS,fileHeader.docscount);
	Doc *pInDoc;
	Cow *pCow;
	DOC_FILERECORD_HEADER FileDoc;
	for(int iDoc = 0;iDoc < fileHeader.docscount;iDoc++)
	{
		bytesRead  = docImportFile.Read(&FileDoc,sizeof(DOC_FILERECORD_HEADER));
		if(bytesRead != sizeof(DOC_FILERECORD_HEADER))
		{
			LOG("CANT READ DOC FILE RECORD.");
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}

		Doc* pDoc = CreateDocDataRecord(&FileDoc,hents);
		if(!pDoc)
		{
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}

		pInDoc = new InDoc(pDoc->GetId());
		pInDoc->CopyFrom(*pDoc);
		delete pDoc;
		
		for(int iCow = 0;iCow < FileDoc.cowcount;iCow++)
		{
			bytesRead  = docImportFile.Read(&FileCow,sizeof(COW_FILERECORD));
			if(bytesRead != sizeof(COW_FILERECORD))
			{
				LOG("CANT READ COW FILE RECORD.");
				progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
				return false;
			}
			pCow = CreateCowDataRecord(&FileCow,hents,stocks);
			
			if(!pCow)
			{
				progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
				return false;
			}
			pCow->SetInDoc(pInDoc);
			cows.Add(pCow);
			pInDoc->GetCows()->Add(pCow);
		}

		
		docs.Add(pInDoc);
		if(!progress->OnStep(iDoc)) return false;
	}



	progress->SetTask(READ_PRCSNG_RRINVS,fileHeader.rrinv_count);
	RRInvoice *pRRInvoice;
	INVOICE_FILERECORD_HEADER FileRRInv;
	COWINVOICEENTRY_FILERECORD FileInvEntry;
	CowInvoiceEntry *pEntry;
	for(int iInv = 0;iInv < fileHeader.rrinv_count;iInv++)
	{
		bytesRead  = docImportFile.Read(&FileRRInv,sizeof(INVOICE_FILERECORD_HEADER));
		if(bytesRead != sizeof(INVOICE_FILERECORD_HEADER))
		{
			LOG("CANT READ RR INV FILE RECORD.");
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}
		Invoice* pNullInvoice = CreateInvoiceDataRecord(&FileRRInv);
		if(!pNullInvoice)
		{
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}
		pRRInvoice = new RRInvoice(pNullInvoice);
		delete pNullInvoice;

		for(int iEntry = 0;iEntry < FileRRInv.entriescount;iEntry++)
		{
			bytesRead  = docImportFile.Read(&FileInvEntry,sizeof(COWINVOICEENTRY_FILERECORD));
			if(bytesRead != sizeof(COWINVOICEENTRY_FILERECORD))
			{
				LOG("CANT READ RR COW INVOICE ENTRY RECORD.");
				progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
				return false;
			}
			pEntry = CreateCowInvoiceEntry(&FileInvEntry,pRRInvoice,cows,stocks);
			if(!pEntry)
			{
				progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
				return false;
			}
			pRRInvoice->invEntries.Add(pEntry);
		}

		rrinvs.Add(pRRInvoice);
		if(!progress->OnStep(iInv)) return false;
	}

	if(!ResolveRRInvoicesForCows(cows,rrinvs))
	{
		progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
		return false;
	}

	/*CString  tmpPassImgPath; 		
	CFile passTmpJPGFile;
	ULONGLONG totalBytesLeft2Read;
	UINT bytes2Read;;
	BYTE readBuffer[1024];
	progress->SetTask(READ_PRCSNG_PASSIMGS,fileHeader.cowpasscount);
	for(int iPass = 0;iPass < fileHeader.cowpasscount;iPass++)
	{
		bytesRead = docImportFile.Read(&FilePass,sizeof(PASSIMG_FILERECORDHEADER));
		if(bytesRead != sizeof(PASSIMG_FILERECORDHEADER))
		{
			progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
			return false;
		}
		
		tmpPassImgPath.Format("%s\\%u.JPG",(LPCSTR)m_tmpSavePath,FilePass.cowid);
		if(passTmpJPGFile.Open((LPCSTR)tmpPassImgPath,CFile::modeCreate | CFile::modeWrite))
		{
			totalBytesLeft2Read = FilePass.imgsize;
			while(totalBytesLeft2Read > 0)
			{
				if(totalBytesLeft2Read > sizeof(readBuffer))
				{
					bytes2Read = sizeof(readBuffer);
				}
				else
				{
					bytes2Read = (UINT)totalBytesLeft2Read;
				}

				bytesRead = docImportFile.Read(readBuffer,bytes2Read);

				if(bytesRead != bytes2Read)
				{
					passTmpJPGFile.Close();
					DeleteFile((LPCSTR)tmpPassImgPath);
					progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
					return false;
				}
				passTmpJPGFile.Write(readBuffer,bytesRead);				
				totalBytesLeft2Read -= bytesRead;
			}
			
			passTmpJPGFile.Close();
			cowPassMap.SetAt(FilePass.cowid,tmpPassImgPath);
			tmpFileDeleter.AddFile(tmpPassImgPath);
		}
		else
		{
			progress->OnTaskError(CTempStringLoader(IDS_CANTOPENOUTPUTFILE));
			return false;
		}
		progress->OnStep(iPass);
	}*/

	delDocs.Release();
	delCows.Release();
	delHents.Release();
	delStocks.Release();
	delRRInvs.Release();

	tmpFileDeleter.Release();


	docContext.company.CopyFrom(*pCompany);
	
	cowPassMap;

	docContext.docPool.Copy(docs);
	docContext.hentPool.Copy(hents);
	docContext.stockPool.Copy(stocks);
	docContext.rrInvPool.Copy(rrinvs);
	return true;
}

bool CImportExportDocs::ResolveRRInvoicesForCows(CCowArray& cows,CInvoiceDataArray &rrinvs)
{
	CCowArray::ArrayIterator cowIt = cows.iterator();
	while(cowIt.hasNext())
	{
		Cow* pCow = *(cowIt++);
		RRInvoice* pRRInvoice = pCow->GetFirstRRInvoice();
		if(pRRInvoice)
		{
			pRRInvoice = (RRInvoice*)rrinvs.GetPtrByInvId((int)pRRInvoice);
			if(!pRRInvoice)
				return false;

			pCow->SetRRInvoice(pRRInvoice);
		}
	}
	return true;
}

CowInvoiceEntry* CImportExportDocs::CreateCowInvoiceEntry(COWINVOICEENTRY_FILERECORD *fileRecord,Invoice* pInvoice,
														  CCowArray& cows,CStockDataArray &stocks)
{
	Cow* pCow = cows.GetCowByCowId(fileRecord->cowid);
	Stock* pStock = stocks.GetStockDataByStockId(fileRecord->stock);

	if(pCow && pStock)
	{
		CowInvoiceEntry *pNewEntry = new CowInvoiceEntry(pCow,pInvoice);

		pNewEntry->SetPrice(fileRecord->price);
		pNewEntry->SetStock(pStock);
		pNewEntry->SetWeight(Weight(fileRecord->weight));

		return pNewEntry;
	}
	return NULL;
}

Stock* CImportExportDocs::CreateStockDataRecord(STOCK_FILERECORD *fileRecord)
{
	Stock *pStock = new Stock(fileRecord->stockid);
	std::auto_ptr<Stock> deleteStock(pStock);

	try
	{
		pStock->SetPredefSex(CowSex((COWSEX)fileRecord->predefsex));
	}
	catch(CStrException *e)
	{
		e->Delete();
		LOG("%d | STOCK'S PREDEFINED SEX IS INVALID",fileRecord->stockid);
		return NULL;
	}

	if(strlen(fileRecord->stockcode) > MAXSTOCKCODE)
	{
		LOG("%d | STOCK'S CODE TO LONG",fileRecord->stockid);
		return NULL;
	}
	pStock->SetStockCode(fileRecord->stockcode);

	if(strlen(fileRecord->stockname) > MAXSTOCKNAME)
	{
		LOG("%d | STOCK'S NAME TO LONG",fileRecord->stockid);
		return NULL;
	}

	pStock->SetStockName(fileRecord->stockname);	
	
	deleteStock.release();
	return pStock;
}

void CImportExportDocs::SetFlatGeoCoord(FLAT_GEO_COORD& flatGeoCoord,const GeoCoordinate& geoCoord)
{
	if(geoCoord.IsNull())
	{
		flatGeoCoord.null = true;
		flatGeoCoord.degrees = 0;
		flatGeoCoord.minutes = 0;
		flatGeoCoord.seconds = 0;
	}
	else
	{
		flatGeoCoord.null = false;
		flatGeoCoord.degrees = geoCoord.GetDegrees();
		flatGeoCoord.minutes = geoCoord.GetMinutes();
		flatGeoCoord.seconds = geoCoord.GetSeconds();
	}
}

bool CImportExportDocs::ConvertGeoCoord(const FLAT_GEO_COORD& flatGeoCoord,GeoCoordinate& geoCoord)
{
	if(flatGeoCoord.null)
	{
		geoCoord.MakeNull();
		return true;
	}

	return geoCoord.Set(flatGeoCoord.degrees,flatGeoCoord.minutes,flatGeoCoord.seconds,HemisphereFactory::GetHemisphere(flatGeoCoord.hemisphere));
}


Hent *CImportExportDocs::CreateHentRecord(HENT_FILERECORD* fileRecord)
{
	boost::regex numericRegEx("[0-9]*");
	boost::regex IDNoRegEx(g_szIDNoRegEx);
	boost::regex EANNoRegEx(g_szEANMatchRegEx);
	Hent *aHent = new Hent(fileRecord->hentid);
	std::auto_ptr<Hent> autoDeleteHent(aHent);
	int l;

	if(strlen(fileRecord->arimrno) > EAN_LENGTH || !boost::regex_match(fileRecord->arimrno, EANNoRegEx))
	{
		LOG("%s | HENT FARM No IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetFarmNo(fileRecord->arimrno);

	Country* hentCountry = ary_countries.GetCountryByCountryNum(fileRecord->country);
	if(!hentCountry) 
	{
		LOG("%s | INVALID HENT COUNTRY CODE %d",fileRecord->arimrno,fileRecord->country);
		return NULL;
	}
	aHent->SetCountry(hentCountry);

	HentType* hentType = ary_htypes.GetHentTypeByHentTypeId(fileRecord->henttype);
	if(!hentType)
	{
		LOG("%s | INVALID HENTTYPE",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetHentType(hentType);


	try
	{
		aHent->SetIssueDate(ConvertToDateTime(fileRecord->issuedate));
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("%s | INVALID HENT ID ISSUEDATE",fileRecord->arimrno);
		return NULL;	
	}

			
	if(strlen(fileRecord->name) > MAXNAME)
	{
		LOG("%s | HENT NAME TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetName(fileRecord->name);

	if(strlen(fileRecord->alias) > MAXALIAS)
	{
		LOG("%s | HENT ALIAS TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetAlias(fileRecord->alias);

	if(strlen(fileRecord->street) > MAXSTREET)
	{
		LOG("%s | HENT STREET TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetStreet(fileRecord->street);
	
	if(strlen(fileRecord->pobox) > MAXPOBOX)
	{
		LOG("%s | HENT POBOX TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetPOBox(fileRecord->pobox);

	if(strlen(fileRecord->city) > MAXCITY)
	{
		LOG("%s | HENT CITY TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetCity(fileRecord->city);

	if(strlen(fileRecord->zip) > MAXZIP)
	{
		LOG("%s | HENT ZIP TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetZip(fileRecord->zip);

	if(strlen(fileRecord->phone) > MAXPHONE)
	{
		LOG("%s | HENT PHONE TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetPhone(fileRecord->phone);

	CString hentNIP(fileRecord->nip);
	hentNIP.Trim();
	if(hentNIP.GetLength() > NIP_LENGTH || 
		!boost::regex_match((LPCSTR)hentNIP, numericRegEx))
	{
		LOG("%s | HENT NIP No IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetNIP(hentNIP);

	

	if(strlen(fileRecord->wetno) > WETNO_LENGTH || !boost::regex_match(fileRecord->wetno, numericRegEx))
	{
		LOG("%s | HENT WET No IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetWetNo(fileRecord->wetno);

	if(strlen(fileRecord->extras) > MAXEXTRAS)
	{
		LOG("%s | HENT EXTRAS TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetExtras(fileRecord->extras);
	
	if(strlen(fileRecord->plate) > MAXPLATE)
	{
		LOG("%s | HENT PLATE TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetPlate(fileRecord->plate);
	
	if(strlen(fileRecord->pesel) > PESEL_LENGTH || !boost::regex_match(fileRecord->pesel, numericRegEx))
	{
		LOG("%s | HENT PESEL IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetPESEL(fileRecord->pesel);

	CString hentREGON(fileRecord->regon);
	hentREGON.Trim();
	if(hentREGON.GetLength() > REGON_LENGTH || !boost::regex_match((LPCSTR)hentREGON, numericRegEx))
	{
		LOG("%s | HENT REGON IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetREGON(hentREGON);
	
	l = strlen(fileRecord->idno); 
	if(l > IDNO_LENGTH || !(l == 0 || boost::regex_match(fileRecord->idno, IDNoRegEx)))
	{
		LOG("%s | HENT ID No IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetIdNo(fileRecord->idno);
	
	if(strlen(fileRecord->issuepost) > MAXISSUEPOST)
	{
		LOG("%s | HENT ID ISSUEPOST TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetIssuePost(fileRecord->issuepost);


	CString accountNo(fileRecord->accountno);
	accountNo.Trim();
	if(accountNo.GetLength() > ACCOUNTNO_LENGTH || !boost::regex_match((LPCSTR)accountNo, numericRegEx))
	{
		LOG("%s | HENT ACCOUNTNO IS INVALID OR TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetAccountNo(accountNo);

	if(strlen(fileRecord->bankname) > MAXBANKNAME)
	{
		LOG("%s | HENT BANKNAME TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetBankName(fileRecord->bankname);

	if(strlen(fileRecord->wetlicenceno) > MAXWETLICENCENO)
	{
		LOG("%s | HENT WETLICENCENO TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetWetLicenceNo(fileRecord->wetlicenceno);

	if(strlen(fileRecord->cellphone) > MAXCELLPHONE)
	{
		LOG("%s | HENT CELLPHONE TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetCellPhoneNo(fileRecord->cellphone);	

	if(strlen(fileRecord->emailaddress) > MAXEMAILADDRESS)
	{
		LOG("%s | HENT EMAILADDRESS TO LONG",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetEmailAddress(fileRecord->emailaddress);

	Latitude latitude;
	if(!ConvertGeoCoord(fileRecord->latitude,latitude) )
	{
		LOG("%s | INVALID LATITUDE",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetLatitude(latitude);	

	Longitude longitude;
	if(!ConvertGeoCoord(fileRecord->longitude,longitude))
	{
		LOG("%s | INVALID LONGITUDE",fileRecord->arimrno);
		return NULL;
	}
	aHent->SetLongitude(longitude);

	autoDeleteHent.release();
	return aHent;
}


NullDoc* CImportExportDocs::CreateDocDataRecord(DOC_FILERECORD_HEADER* fileRecord,CHentArray &hents)
{
	NullDoc aDoc(fileRecord->docid);

	Hent* pDocHent = hents.GetHentByHentId(fileRecord->hent);
	
	if(pDocHent == NULL)
	{
		LOG("%d | DOC HENT WITH ID %d NOT FOUND",fileRecord->docid,fileRecord->hent);
		return NULL;
	}

	aDoc.SetHent(pDocHent);

	Reason* pDocReason = ary_reasons.GetReasonDataByReasonId(fileRecord->reason);
	
	if(pDocReason == NULL)
	{
		LOG("%d | DOC REASON WITH ID %d NOT FOUND",fileRecord->docid,fileRecord->reason);
		return NULL;
	}
	aDoc.SetReason(pDocReason); 

	try
	{
		aDoc.SetDocDate(ConvertToDateTime(fileRecord->docdate));
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("%d | INVALID DOC DATE",fileRecord->docid);
		return NULL;	
	}

	try
	{
		aDoc.SetLoadDate(ConvertToDateTime(fileRecord->loaddate));
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("%d | INVALID DOC LOAD DATE",fileRecord->docid);
		return NULL;	
	}

	if(strlen(fileRecord->extras) > MAXEXTRAS)
	{
		LOG("%d | DOC EXTRAS TO LONG",fileRecord->docid);
		return NULL;
	}
	aDoc.SetExtras(fileRecord->extras);
	
	if(strlen(fileRecord->plateno) > MAXPLATE)
	{
		LOG("%d | DOC PLATE No TO LONG",fileRecord->docid);
		return NULL;
	}
	aDoc.SetPlateNo(fileRecord->plateno);

	aDoc.SetLoadStartTime(Time(fileRecord->loadstart));
	aDoc.SetLoadEndTime(Time(fileRecord->loadend));

	NullDoc* pNullDoc = new NullDoc();
	pNullDoc->CopyFrom(aDoc);
	return pNullDoc;
}


Cow* CImportExportDocs::CreateCowDataRecord(COW_FILERECORD *fileRecord,CHentArray &hents,
									CStockDataArray &stocks)
{

	boost::regex EANNoRegEx(g_szEANMatchRegEx);
	boost::regex PassNoRegEx("[a-zA-Z]{2}[0-9]{8}");

	Cow* pNewCow = new Cow(fileRecord->animalid);
	std::auto_ptr<Cow> autoDeleteNewCow(pNewCow);

	int l;

	if(strlen(fileRecord->ean) > EAN_LENGTH || !boost::regex_match(fileRecord->ean, EANNoRegEx))
	{
		LOG("INVALID COW EAN '%s'",fileRecord->ean);
		return NULL;
	}
	pNewCow->SetEAN(fileRecord->ean);
	

	try
	{
		pNewCow->SetBirthDate(ConvertToDateTime(fileRecord->birthdate));
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("%s | INVALID BIRTH DATE",fileRecord->ean);
		return NULL;	
	}
	
	
	try
	{
		CowSex sex = (COWSEX)fileRecord->sex;
		pNewCow->SetSex(sex);
	}
	catch(CStrException *e)
	{
		e->Delete();
		LOG("%s | INVALID COW SEX",fileRecord->ean);
		return NULL;
	}

	Stock* pCowStock = stocks.GetStockDataByStockId(fileRecord->stock);
	
	if(!pCowStock)
	{
		LOG("%s | CANT FIND COW STOCK WITH ID = %d",fileRecord->ean,fileRecord->stock);
		return NULL;	
	}

	pNewCow->SetStock(pCowStock);
	pNewCow->SetWeight(Weight(fileRecord->weight));
	

	try
	{
		pNewCow->SetPassDate(ConvertToDateTime(fileRecord->passdate));
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("%s | INVALID PASSS DATE",fileRecord->ean);
		return NULL;	
	}
			
	pNewCow->SetMyPrice(Money(fileRecord->myprice));
	
	if( fileRecord->ingrp == NULL_INOUT_GRP || (fileRecord->ingrp >= 0 && fileRecord->ingrp <= MAX_INOUT_GRP))
	{
		pNewCow->SetInGrp(fileRecord->ingrp);
	}
	else
	{
		LOG("%s | INVALID INGROUP %d",fileRecord->ean,fileRecord->ingrp);
		return NULL;
	}


	if(fileRecord->fstownr)
	{
		Hent* pFstOwner = hents.GetHentByHentId(fileRecord->fstownr);
		
		if(!pFstOwner)
		{
			LOG("%s | CANT FIND COW FST OWNER WITH ID = %d",fileRecord->ean,fileRecord->fstownr);
			return NULL;
		}
		pNewCow->SetFirstOwner(pFstOwner);
	}

	if(strlen(fileRecord->birthplace) > MAXBIRTHPLACE)
	{
		LOG("%s | BIRTHPLACE EXTRAS TO LONG",fileRecord->ean);
		return NULL;
	}
	pNewCow->SetBirthPlace(fileRecord->birthplace);

	if(strlen(fileRecord->extras) > MAXEXTRAS)
	{
		LOG("%s | COW EXTRAS TO LONG",fileRecord->ean);
		return NULL;
	}
	pNewCow->SetExtras(fileRecord->extras);	

	l = strlen(fileRecord->motherean); 
	if(l > EAN_LENGTH || !(l == 0 || boost::regex_match(fileRecord->motherean, EANNoRegEx)))
	{
		LOG("%s | MOTHEREAN INVALID OR TO LONG",fileRecord->ean);
		return NULL;
	}
	pNewCow->SetMotherEAN(fileRecord->motherean);

	l = strlen(fileRecord->passno); 
	if(l > PASSNO_LENGTH || !(l == 0 || boost::regex_match(fileRecord->passno, PassNoRegEx)))
	{
		LOG("%s | PASSNO INVALID OR TO LONG",fileRecord->ean);
		return NULL;
	}
	pNewCow->SetPassNo(fileRecord->passno);

	if(fileRecord->rrinvoiceid)
	{
		pNewCow->AssignToRRInvoice((RRInvoice*)fileRecord->rrinvoiceid);
	}

	autoDeleteNewCow.release();
	return pNewCow;

}

void CImportExportDocs::CreateInvoiceFileRecord(INVOICE_FILERECORD_HEADER* fileRecord,Invoice *pInvoice)
{
	memset(fileRecord,0,sizeof(INVOICE_FILERECORD_HEADER));
	strncpy(fileRecord->custom_number,(LPCSTR)pInvoice->GetCustomNumber(),MAXCUSTOMNUMBER);
	strncpy(fileRecord->extras,(LPCSTR)pInvoice->GetExtras(),MAXEXTRAS);
	SetFlatDate(fileRecord->invoice_date,pInvoice->GetInvoiceDate());
	fileRecord->payway = (char)pInvoice->GetPayWay();
	fileRecord->invoiceid = pInvoice->GetId();
	fileRecord->vat_rate = pInvoice->GetVATRate();
	fileRecord->invoicevalue = pInvoice->GetInvoiceNetValue().GetDouble();
	fileRecord->payduedays = pInvoice->GetPayDueDays();
	fileRecord->entriescount = pInvoice->GetCowQty();
}

NullInvoice* CImportExportDocs::CreateInvoiceDataRecord(INVOICE_FILERECORD_HEADER* pFileRecord)
{
	NullInvoice *pInvoice = new NullInvoice(pFileRecord->invoiceid);
	
	if(strlen(pFileRecord->custom_number) > MAXCUSTOMNUMBER)
	{
		LOG("%d | INV CUSTOM NUMBER TO LONG",pFileRecord->invoiceid);
		return NULL;
	}

	pInvoice->SetCustomNumber( pFileRecord->custom_number );
	
	pInvoice->SetPayWay( (Invoice::PAYWAYS)pFileRecord->payway );
	
	pInvoice->SetVATRate( pFileRecord->vat_rate );
	
	try
	{
		pInvoice->SetInvoiceDate( ConvertToDateTime(pFileRecord->invoice_date) );
	}
	catch(CDateTimeException *e)
	{
		e->Delete();
		LOG("INVALID INVOICE DATE FOR INVOICE WITH ID %d",pInvoice->GetId());
		return NULL;	
	}

	
	if(strlen(pFileRecord->extras) > MAXEXTRAS)
	{
		LOG("%d | INV EXTRAS TO LONG",pFileRecord->invoiceid);
		return NULL;
	}
	pInvoice->SetExtras( pFileRecord->extras );
	
	pInvoice->SetInvoiceType( IN_INV_RR );
	
	pInvoice->SetInvoiceNetValue( Money(pFileRecord->invoicevalue) );
	
	pInvoice->SetPayDueDays( pFileRecord->payduedays );
	
	return pInvoice;
}

