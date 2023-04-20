#ifndef __IMPORTEXPORTDOCS_H__
#define __IMPORTEXPORTDOCS_H__

#include "../DataTypes.h"
#include "FileDataRecords.h"
#include <set>
#include "../taskprogress/TaskProgressDlg.h"
#include "../PassportCommon/fipWinImageEx.h"
#include "../datatypes/NullInvoice.h"
#include "../datatypes/CompanyInfo.h"
#include "FileDeleter.h"
#include "../datatypes/NullDoc.h"
#include "ExportSettings.h"

typedef std::set<Stock*> CStockPool;
typedef std::set<Hent*> CHentPool;
typedef CMap<int,int,CString,CString&> CPassCowMap;


class ExportCowAccessor 
{
public:
	ExportCowAccessor(CowAccessor* pAccessor);
	virtual RRInvoice* GetRRInvoice(const Cow* pCow) = 0;
	virtual Money GetExportMyPrice(const Cow* pCow) = 0;
	virtual Stock* GetExportStock(const Cow* pCow) = 0;
	virtual Weight GetExportWeight(const Cow* pCow) = 0;
	virtual NullInt GetGroup(const Cow* pCow);
protected:
	CowAccessor* m_pAccessor;

};


class ExportInDocCowAccessor : public ExportCowAccessor
{
public:
	ExportInDocCowAccessor(const ExportSettings& exportSettings);
	
	virtual RRInvoice* GetRRInvoice(const Cow* pCow);
	virtual Money GetExportMyPrice(const Cow* pCow);
	virtual Stock* GetExportStock(const Cow* pCow);
	virtual Weight GetExportWeight(const Cow* pCow);
protected:
	const ExportSettings& m_exportSettings;
	CowInAccessor m_InAccessor;
};


class ExportOutDocCowAccessor : public ExportCowAccessor
{
public:
	ExportOutDocCowAccessor(const ExportSettings& exportSettings);
	
	virtual RRInvoice* GetRRInvoice(const Cow* pCow);
	virtual Money GetExportMyPrice(const Cow* pCow);
	virtual Stock* GetExportStock(const Cow* pCow);
	virtual Weight GetExportWeight(const Cow* pCow);
protected:
	CowOutAccessor m_OutAccessor;
	const ExportSettings& m_exportSettings;
};


class ExportDocContext
{
public:
	ExportDocContext() : company(NULL_HENTID) {}
	CString docFilePath;
	CDocDataArray docPool;
	//CCowArray cowPool;
	CPassCowMap cowPassMap;
	std::set<Hent*> hentPool;
	std::set<Stock*> stockPool;
	std::set<RRInvoice*> rrInvPool;
	std::set<VATInvoice*> vatInvPool;
	Hent company;
	ExportCowAccessor *pCowAccessor;
	CFileDeleter passTmpFilesDeleter;
};


class ImportDocContext
{
public:
	ImportDocContext() : company(NULL_HENTID) {}
	CString docFilePath;
	CDocDataArray docPool;
	CPassCowMap cowPassMap;
	CHentArray hentPool;
	CStockDataArray stockPool;
	CInvoiceDataArray rrInvPool;
	Hent company;	
	CFileDeleter passTmpFilesDeleter;
};

class CImportExportDocs 
{
public:

	//enum IMPORTEXPORT_ERROR_CODES 
	//{
	//	CANT_OPEN_INPUT_FILE = 100,
	//	CANT_WRITE_OUTPUT_FILE,
	//	INPUT_FILE_CORRUPTED,
	//	INVALID_COMPANY_INFO,
	//	OTHER_VERSION
	//};
	CImportExportDocs();
	bool WriteInDoc(CDocDataArray &indocsArray,const ExportSettings& exportSettings,const CString &outputFile,ITaskProgres *progress);
	bool WriteOutDoc(CDocDataArray &outdocsArray,const ExportSettings& exportSettings,const CString &outputFile,ITaskProgres *progress);
	bool ReadInDoc(ImportDocContext& docContext,ITaskProgres *progress);

private:
	void CreateDocContext(ExportDocContext &docContext,const ExportSettings& exportSettings,CDocDataArray &docsArray);	
	bool WriteDoc(ExportDocContext& outDocContext,ITaskProgres *progress);

	void CreateDocFileRecord(DOC_FILERECORD_HEADER* fileRecord,Doc *pDoc);
	STOCK_FILERECORD* CreateStockFileRecord(Stock *pStock);
	HENT_FILERECORD* CreateHentFileRecord(const Hent *pHent);
	void CreateCowFileRecord(COW_FILERECORD* fileRecord,Cow *pCow,ExportCowAccessor* pAccessor);
	bool SaveCowPassImg(Cow *pCow,CString &passFile);

	Stock *CreateStockDataRecord(STOCK_FILERECORD *fileRecord);
	Hent *CreateHentRecord(HENT_FILERECORD* fileRecord);
	NullDoc* CreateDocDataRecord(DOC_FILERECORD_HEADER* fileRecord,CHentArray &hents);
	Cow* CreateCowDataRecord(COW_FILERECORD *fileRecord,CHentArray &hents,CStockDataArray &stocks);
	void SetFlatDate(FLAT_DATE& flatDt,const CDateTime& dt);
	CDateTime ConvertToDateTime(const FLAT_DATE& flatDt);

	void CreateInvoiceFileRecord(INVOICE_FILERECORD_HEADER* fileRecord,Invoice *pInvoice);
	void CreateCowPriceEntry(COWINVOICEENTRY_FILERECORD *fileRecord,CowInvoiceEntry *pEntry);
	CowInvoiceEntry* CreateCowInvoiceEntry(COWINVOICEENTRY_FILERECORD *fileRecord,Invoice* pInvoice,
													CCowArray& cows,CStockDataArray &stocks);

	NullInvoice* CreateInvoiceDataRecord(INVOICE_FILERECORD_HEADER* pFileRecord);
	CString CreateCompanyAlias(const CString& companyName);
	//Stock* GetExportStock(Cow *pCow,CowAccessor* pAccessor);
	//NullInt GetExportWeight(Cow* pCow,CowAccessor* pAccessor);
	//Money GetExportMyPrice(Cow* pCow,CowAccessor* pAccessor);
	bool ResolveRRInvoicesForCows(CCowArray& cows,CInvoiceDataArray &rrinvs);
	bool ConvertGeoCoord(const FLAT_GEO_COORD& flatGeoCoord,GeoCoordinate& geoCoord);
	void SetFlatGeoCoord(FLAT_GEO_COORD& flatGeoCoord,const GeoCoordinate& geoCoord);

	CString m_tmpSavePath;


	

};




#endif 
