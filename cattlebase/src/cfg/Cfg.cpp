// Cfg.cpp: implementation of the CCfg class.
//
//////////////////////////////////////////////////////////////////////

/*****************************************************************************
	CCfg, CCfgNode, CCfgNodeBool, CCfgNodeItem, CCfgNodeNum, CCfgNodeSection
	CCfgNodeString

	Author:		Ruben Jönsson, ruben@pp.sbbs.se
	Version:	1.00
	Date:		2002-01-10
	
	Revision history:
	---------------------------------------------------------------------
	1.00	2002-01-10	Released

*******************************************************************************/


////// Config settings /////////////////////////////////////////////////////////////////////////////////////////////
//
//	The CCfg class handles configuration settings according to an array of Cfg definition structures.
//
//	The CFGDEF is used to connect ID's with cfg string lines in a cfg file. 
//
//	MakeCfgList takes an array of CFGDEF strucutures and builds a CCfgList with one node for every section, 
//	item and parameter specified in the CFGDEF array. Every node is associated with an unique ID. This ID can then
//	be used to set or read parameters (string, long or BOOL) in the CCfgList. The CCfgList can be loaded with 
//	data from the string list (from the txtvar file) and be manipulated (read or changed) and at a later time be
//	written back to the string list (and the txtvar file).
//
//	The CFGDEF array also sets default values for every parameter which is used if the parameter doesn't exist
//	in the string list (which it doesn't the first time it is used or after the cfg file is deleted).
//	It is also possible to set a valid range for a numeric value.
//
//	After the CCfgList is loaded any argument in the cfg file can be accessed with it's ID number and one of the
//	access functions CfgGet...Arg() or CfgSet...Arg().
//
//	When a setting is changed it isn't automatically written back to the string list (and the file) untill Flush()
//	is caled.

#include "stdafx.h"
#include "Cfg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



unsigned int CCfg::hex2int(LPCSTR s) 
{
	unsigned int t = 0;
	for(int i=0; i<2; i++)
		if(s[i]>='0' && s[i]<='9')
			t = (t<<4)+(s[i]-'0');
		else if(s[i]>='a' && s[i]<='f')
			t = (t<<4)+(s[i]-'a'+10);
		else if(s[i]>='A' && s[i]<='F')
			t = (t<<4)+(s[i]-'A'+10);
		else
			break;
		return t ;
}

CCfg::CCfg()
{
	m_bCfgListInitiated=FALSE;
}

CCfg::~CCfg()
{
	if (m_bChanged)
		SaveCfgList();

	DeleteCfgList();	// Do not call Release() since it will clear variables for CTxtVar which is needed when 
						// CTxtVar is destructed.
}

void CCfg::DeleteCfgList()
// Delete all nodes in the Cfg list.
{
	POSITION pos;
	CCfgNode *n;

	pos=m_cCfgList.GetHeadPosition();
	while (pos!=NULL){
		n=m_cCfgList.GetNext(pos);
		delete n;
	}
	m_cCfgList.RemoveAll();
}

int CCfg::LoadStringList(const CString strFileName)
// Read all lines of a CFG text file into m_stringList.
// Note that the text file is only opened during LoadStringList() and Flush() and
// can be shared by others. It should only be written by one instance
// though.
{
	int iRet=CTxtVar::LoadStringList(strFileName);
	if (iRet==TXTVAR_NOERR || iRet==TXTVAR_NOFILE){
		iRet=TXTVAR_NOERR;	// Make sure a non existing cfg file doesn't generate an error
							// since this is a legal case. If the file doesn't exist it will
							// be created with default values.
		LoadCfgList();		// Copy values from file to CfgList
	}
	return iRet;
}

int CCfg::Flush()
// Write back all settings to file. Used after a setting is changed.
{
	SaveCfgList();	// Update string list with config settings
	return CTxtVar::Flush();
}

void CCfg::Release()
// Reset cfg and clear string list.

// Release is not a virtual function since it would mean that the CCfgList would be cleared when the base class 
// CTxtVar calls LoadStringList (calls Release()) which is done after the CCfgList is created. 
{
	CTxtVar::Release();
	DeleteCfgList();
}

void CCfg::MakeCfgList(const CFGDEF *cfg_def)
// Create the cfg list based on a CFGDEF structure array.
{
	int i,iSectionId,iItemId,iLastType,iArgNum,iNumArgs,iNumItems;
	CCfgNode *n;
	
	ASSERT(!m_bCfgListInitiated);				// Can only have one cfg definition
	m_bCfgListInitiated=TRUE;
	ASSERT(cfg_def[0].iType==CFGT_SECTION);		// Must start with type CFGT_SECTION
	iArgNum=iNumArgs=0;

	for(i=0;cfg_def[i].iType!=CFGT_END;i++){
		switch (cfg_def[i].iType){
		case CFGT_SECTION:		// A section header.
			ASSERT(iArgNum==iNumArgs); // Less arguments than iNumArgs for last CFGT_ITEM.
			iSectionId=cfg_def[i].iId;
			n=(CCfgNode*) new CCfgNodeSection(iSectionId,-1,cfg_def[i].strStr);	
			iArgNum=iNumArgs=0;
			iNumItems=0;
			break;
		case CFGT_ITEM:			// An item inside a section.
			ASSERT((iNumItems==0 && iLastType==CFGT_SECTION) || iNumItems!=0); // CFGT_SECTION must be followed by a CFGT_ITEM.
			ASSERT(iArgNum==iNumArgs);// Less arguments than iNumArgs for last CFGT_ITEM.
			iItemId=cfg_def[i].iId;
			iNumArgs=cfg_def[i].iVal1;
			n=(CCfgNode*)new CCfgNodeItem(iItemId,iSectionId,cfg_def[i].strStr,iNumArgs);
			iArgNum=0;
			iNumItems++;
			break;
		case CFGT_ARGSTRING:	// A string argument (parameter) of an item.
			ASSERT(((iArgNum==0 && iLastType==CFGT_ITEM) || iArgNum!=0) && 
				iArgNum<iNumArgs); // Argument nr 0 must be preceded by CFGT_ITEM.
								   // Not more than iNumArgs arguments.
			n=(CCfgNode*)new CCfgNodeString(cfg_def[i].iId,iItemId,cfg_def[i].strStr);
			iArgNum++;
			break;
		case CFGT_ARGNUM:		// A numeric argument (parameter) of an item.
			ASSERT(((iArgNum==0 && iLastType==CFGT_ITEM) || iArgNum!=0) && 
				iArgNum<iNumArgs); // Argument nr 0 must be preceded by CFGT_ITEM.
								   // Not more than iNumArgs arguments.
			n=(CCfgNode*)new CCfgNodeNum(cfg_def[i].iId,iItemId,cfg_def[i].iVal3,cfg_def[i].iVal2,cfg_def[i].iVal1);
			iArgNum++;
			break;
		case CFGT_ARGBOOL:		// A BOOL argument (parameter) of an item.
			ASSERT(((iArgNum==0 && iLastType==CFGT_ITEM) || iArgNum!=0) && 
				iArgNum<iNumArgs); // Argument nr 0 must be preceded by CFGT_ITEM.
								   // Not more than iNumArgs arguments.
			n=(CCfgNode*)new CCfgNodeBool(cfg_def[i].iId,iItemId,cfg_def[i].iVal3);
			iArgNum++;
			break;
		case CFGT_ARGFLOAT:
			ASSERT(((iArgNum==0 && iLastType==CFGT_ITEM) || iArgNum!=0) && iArgNum<iNumArgs);
			n=(CCfgNode*)new CCfgNodeFloat(cfg_def[i].iId,iItemId,cfg_def[i].fVal);
			iArgNum++;
			break;
		case CFGT_ARGRGB:
			ASSERT(((iArgNum==0 && iLastType==CFGT_ITEM) || iArgNum!=0) && iArgNum<iNumArgs);
			n=(CCfgNode*)new CCfgNodeRGB(cfg_def[i].iId,iItemId,cfg_def[i].iVal1);
			iArgNum++;
			break;

		default:
			ASSERT(FALSE);	// Unknown type
		}
		
		m_cCfgList.AddTail(n);
		iLastType=cfg_def[i].iType;
	}
	ASSERT(iArgNum==iNumArgs);// Less arguments than iNumArgs for last CFGT_ITEM.
}

CCfgNode* CCfg::GetCfgNodeFromId(int iId,POSITION *pos)
// Find the CfgNode from it's ID in the CfgList.
// If pos is not NULL pos is set to the POSITON following the CfgNode found.
{
	CCfgNode *n;
	POSITION localpos;

	localpos=m_cCfgList.GetHeadPosition();
	while(localpos!=NULL){
		n=m_cCfgList.GetNext(localpos);
		if (n->m_iId==iId){
			if (pos!=NULL) *pos=localpos;
			return n;
		}
	}
	ASSERT(FALSE);
	return NULL;
}

CCfgNode* CCfg::GetCfgNodeItemArg(int iItemId, int iArgIndex,POSITION *pos)
// Find a CfgNode fom it's item ID (the ID of the variable) and a argument index number.
// Index number 0 is the first argument (parameter) after the variable name (item).
// If pos is not NULL pos is set to the POSITION following the CfgNode found.
{
	POSITION localpos;
	CCfgNode *n;

	n=GetCfgNodeFromId(iItemId,&localpos);
	ASSERT(n!=NULL);
	ASSERT(n->GetType()==CFGT_ITEM);
	ASSERT(n->GetNumArgs()>iArgIndex);
	
	n=GetCfgNodeItemArg(n,iArgIndex,&localpos);
	if (pos!=NULL) *pos=localpos;
	ASSERT(n!=NULL);
	return n;
}

CCfgNode* CCfg::GetCfgNodeItemArg(CCfgNode *nItem, int iArgIndex,POSITION *pos)
// Find a CfgNode from its item CfgNode (the CfgNode of the variable - item) and a
// argument index number. Index number 0 is the first argument (parameter) after the
// variable name (item).
// pos should be NULL or the POSITION following the CfgNode variable name (item).
// This value is the one obtained by GetCfgNodeFromId() for the variable (item) ID. 
{
	POSITION localpos;
	CCfgNode *n=NULL;
	int i;

	if (pos==NULL)
		localpos=m_cCfgList.Find(nItem);
	else
		localpos=*pos;

	for(i=0;i<iArgIndex+1;i++){
		ASSERT (localpos!=NULL);
		n=m_cCfgList.GetNext(localpos);
	}
	if (pos!=NULL) *pos=localpos;
	ASSERT(n!=NULL);
	return n;
}

CCfgNode* CCfg::GetCfgNodeItem(const CString strItem,const CString strSection,POSITION *pos)
// Find a CfgNode fom it's section name and variable (item) name.
// If pos is not NULL pos is set to the POSITION following the CfgNode for the found item (the first
// argument).
{
	CCfgNode *n;
	POSITION localpos;

	localpos=m_cCfgList.GetHeadPosition();
	while(localpos!=NULL){
		n=m_cCfgList.GetNext(localpos);
		if (n->GetType()==CFGT_SECTION && n->GetSection()==strSection){
			while(localpos!=NULL){
				n=m_cCfgList.GetNext(localpos);
				if (n->GetType()==CFGT_SECTION)
					return NULL;
				else if (n->GetType()==CFGT_ITEM && n->GetItem()==strItem){
					if (pos!=NULL) *pos=localpos;
					return n;
				}
			}
		}
	}
	return NULL;
}

void CCfg::LoadCfgList()
/*
	Load m_cCfgList from m_stringList of the CTxtVar (base) class.
	Done after the cfg file is opened and read in into m_strinList.
	Called by Init();
*/
{
	POSITION pos;
	CCfgNode *n;
	CString strSection,strItem,strArg;
	int iNumArgs,iArgNum,iNum;
	
	pos=m_cCfgList.GetHeadPosition();

	while(pos!=NULL){
		n=m_cCfgList.GetNext(pos);
		switch (n->GetType()){
		case CFGT_SECTION:
			strSection=n->GetSection();
			break;
		case CFGT_ITEM:
			strItem=n->GetItem();
			if (FindSectionItem(strSection,strItem)==CFG_NOERR){
				iNumArgs=n->GetNumArgs();	// Item found in file, convert its args.
				iArgNum=0;
			}
			else {
				iNumArgs=0; // Item not found in file, skip following CFGT_ARG...
				m_bChanged=TRUE; // Write back default values to file
				iArgNum=0;
			}
			break;
		case CFGT_ARGSTRING:
		case CFGT_ARGNUM:
		case CFGT_ARGBOOL:
		case CFGT_ARGFLOAT:
		case CFGT_ARGRGB:
			if (iNumArgs>0 && iArgNum<(m_iNumArgs-1)){
				strArg=m_strArgs[iArgNum+1];
				if (n->GetType()==CFGT_ARGSTRING)
					n->SetStrArg(strArg);
				else if (n->GetType()==CFGT_ARGNUM){
					if (sscanf(strArg,"%d",&iNum)==1)
						n->SetNumArg(iNum);
				}
				else if (n->GetType()==CFGT_ARGBOOL){
					strArg.MakeUpper();
					if (sscanf(strArg,"%d",&iNum)==1)
						n->SetBoolArg(iNum!=0);
					else if (strArg=="TRUE" || strArg=="T")
						n->SetBoolArg(TRUE);
					else if (strArg=="FALSE" || strArg=="F")
						n->SetBoolArg(FALSE);
				}
				else if(n->GetType() == CFGT_ARGFLOAT)
				{
					n->SetFloatArg(atof(strArg));
				}
				else if(n->GetType() == CFGT_ARGRGB)
				{
					COLORREF Clr;
					Clr = RGB(hex2int((LPCSTR)strArg),
							  hex2int(((LPCSTR)strArg)+2),
                              hex2int(((LPCSTR)strArg)+4));

					n->SetRGBArg(Clr);
				}
			}
			iArgNum++;
			break;
		default:
			ASSERT(FALSE);
		}
	}
	if (m_bChanged)
		Flush();
}

CString CCfg::GetString(int iId)
// Get the string argument for the ID iId.
// If the ID isn't a string type an ASSERT fails.
{
	return GetCfgNodeFromId(iId)->GetStrArg();
}

CString CCfg::GetString(int iItemId, int iArgIndex)
// Get the string argument from an Item ID and an argument index.
// If the argument isn't a string type an ASSERT fails. 
{
	return GetCfgNodeItemArg(iItemId,iArgIndex)->GetStrArg();
}

BOOL CCfg::GetBool(int iId)
// Get the BOOL argument for the ID iId.
// If the ID isn't a BOOL type an ASSERT fails.
{
	return GetCfgNodeFromId(iId)->GetBoolArg();
}

BOOL CCfg::GetBool(int iItemId, int iArgIndex)
// Get the BOOL argument from an Item ID and an argument index.
// If the argument isn't a BOOL type an ASSERT fails. 
{
	return GetCfgNodeItemArg(iItemId,iArgIndex)->GetBoolArg();
}

long CCfg::GetNum(int iId)
// Get the numeric argument for the ID iId.
// If the ID isn't a numeric type an ASSERT fails.
{
	return GetCfgNodeFromId(iId)->GetNumArg();
}

long CCfg::GetNum(int iItemId, int iArgIndex)
// Get the numeric argument from an Item ID and an argument index.
// If the argument isn't a numeric type an ASSERT fails. 
{
	return GetCfgNodeItemArg(iItemId,iArgIndex)->GetNumArg();
}


double CCfg::GetFloat(int iId)
{
	return GetCfgNodeFromId(iId)->GetFloatArg();
}

double CCfg::GetFloat(int iItemId, int iArgIndex)
{
	return GetCfgNodeItemArg(iItemId,iArgIndex)->GetFloatArg();
}


COLORREF CCfg::GetRGB(int iId)
{
	return GetCfgNodeFromId(iId)->GetRGBArg();
}
COLORREF CCfg::GetRGB(int iItemId, int iArgIndex)
{
	return GetCfgNodeItemArg(iItemId,iArgIndex)->GetRGBArg();
}


void CCfg::SetString(int iId, const CString &str)
// Set the string argument for the ID iID.
// If the ID isn't a string type an ASSERT fails.
{
	GetCfgNodeFromId(iId)->SetStrArg(str);
	m_bChanged=TRUE;
}

void CCfg::SetString(int iItemId, int iArgIndex, const CString &str)
// Set the string argument for an Item ID and an argument index.
// If the ID isn't a string type an ASSERT fails.
{
	GetCfgNodeItemArg(iItemId,iArgIndex)->SetStrArg(str);
	m_bChanged=TRUE;
}

void CCfg::SetBool(int iId, BOOL b)
// Set the BOOL argument for the ID iID.
// If the ID isn't a BOOL type an ASSERT fails.
{
	GetCfgNodeFromId(iId)->SetBoolArg(b);
	m_bChanged=TRUE;
}

void CCfg::SetBool(int iItemId, int iArgIndex, BOOL b)
// Set the BOOL argument for an Item ID and an argument index.
// If the ID isn't a BOOL type an ASSERT fails.
{
	GetCfgNodeItemArg(iItemId,iArgIndex)->SetBoolArg(b);
	m_bChanged=TRUE;
}

void CCfg::SetNum(int iId, int iNum)
// Set the numeric argument for the ID iID.
// If the ID isn't a numeric type an ASSERT fails.
{
	GetCfgNodeFromId(iId)->SetNumArg(iNum);
	m_bChanged=TRUE;
}

void CCfg::SetNum(int iItemId, int iArgIndex, int iNum)
// Set the numeric argument for an Item ID and an argument index.
// If the ID isn't a numeric type an ASSERT fails.
{
	GetCfgNodeItemArg(iItemId,iArgIndex)->SetNumArg(iNum);
	m_bChanged=TRUE;
}

void CCfg::SetFloat(int iId, double fNum)
{
	GetCfgNodeFromId(iId)->SetFloatArg(fNum);
	m_bChanged=TRUE;
}

void CCfg::SetFloat(int iItemId, int iArgIndex, double fNum)
{
	GetCfgNodeItemArg(iItemId,iArgIndex)->SetFloatArg(fNum);
	m_bChanged=TRUE;
}

void CCfg::SetRGB(int iId, COLORREF Clr)
{
	GetCfgNodeFromId(iId)->SetRGBArg(Clr);
	m_bChanged=TRUE;
}
void CCfg::SetRGB(int iItemId, int iArgIndex, COLORREF Clr)
{
	GetCfgNodeItemArg(iItemId,iArgIndex)->SetRGBArg(Clr);
	m_bChanged=TRUE;
}



void CCfg::SaveCfgList()
/*
	Convert arguments from m_cCfgList to strings in m_stringList of the CTxtVar (base) class.
	Should be done before the cfg file is updated after changes are done in
	the settings by CfgSet... functions.
	Called by Flush();
*/
{
	CString strSection,strItem,strArg;
	CCfgNode *n;
	int iNumArgs,ret,i;
	POSITION pos;

	pos=m_cCfgList.GetHeadPosition();
	while(pos!=NULL){
		n=m_cCfgList.GetNext(pos);
		if (n->GetType()==CFGT_SECTION)
			strSection=n->GetSection();	// Set the section string if this CfgNode is a section header.
		else if (n->GetType()==CFGT_ITEM){
										// If this CfgNode is an Item (variable) go through all it's arguments
										// and build a text string for it.
			strItem=n->GetItem();
			iNumArgs=n->GetNumArgs();
			ret=FindSectionItem(strSection,strItem); // Does this item already exist in the CTxtVar string list?
			for(i=0;i<iNumArgs && pos!=NULL;i++){
				n=m_cCfgList.GetNext(pos);
				switch (n->GetType()){
				case CFGT_ARGSTRING:
					strArg.Format("\"%s\"",n->GetStrArg());
					break;
				case CFGT_ARGBOOL:
					strArg.Format("%d",n->GetBoolArg());
					break;
				case CFGT_ARGNUM:
					strArg.Format("%d",n->GetNumArg());
					break;
				case CFGT_ARGFLOAT:
					strArg.Format("%f",n->GetFloatArg());
					break;
				case CFGT_ARGRGB:
				{
					COLORREF Clr = n->GetRGBArg();
					strArg.Format("%02X%02X%02X",GetRValue(Clr),GetGValue(Clr),GetBValue(Clr));
					break;
				}
				}
				strItem+=(i==0)?" ":",";
				strItem+=strArg;
			}
			i++;
					// If the item already exists in the CTxtVar sting list add coments and not included
					// arguments.
			if (ret==CFG_NOERR){
				for(;i<m_iNumArgs;i++){
					if (m_strArgs[i][0]==';'){
						strItem+=" ";
						strItem+=m_strArgs[i];
						break;
					}
					strItem+=",";
					strItem+=m_strArgs[i];
				}
			}
			SetSectionItem(strSection,strItem);
		}
	}
}

int CCfg::Init(const CString strCfgFileName)
// Call Init to associate the CfgNode list with a .CFG file and load the list with values from the file.
// If variables (items) are missing from the file these will be created and it's arguments will be set to 
// the default values. If the file doesn't exist it is created.
// 
// A good place to put the call for Init is in InitInstance() for the main app.
{
	int iRet;
	if ((iRet=LoadStringList(strCfgFileName))!=CFG_NOERR)
		return iRet;
	Flush();
	return CFG_NOERR;
}

/////////// Cfg list node classes ///////////////////////////////////////////////////////////////////////////////////////////

///// CCfgNode /////////////////
// 
// CCfgNode is the base class for all node types.

// Most of the CfgNode functions are inline and are defined in the .h file.

	IMPLEMENT_DYNAMIC(CCfgNode,CObject)
CCfgNode::CCfgNode(int iId,int iParentId)
{
	m_iId=iId;
	m_iParentId=iParentId;
}

int CCfgNode::GetType()
{
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeSection)))	return CFGT_SECTION;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeItem)))		return CFGT_ITEM;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeString)))	return CFGT_ARGSTRING;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeNum)))		return CFGT_ARGNUM;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeBool)))		return CFGT_ARGBOOL;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeFloat)))		return CFGT_ARGFLOAT;
	if (IsKindOf(RUNTIME_CLASS(CCfgNodeRGB)))		return CFGT_ARGRGB;
	return -1;
}

//// CCfgNodeSection

	IMPLEMENT_DYNAMIC(CCfgNodeSection,CCfgNode)
CCfgNodeSection::CCfgNodeSection(int iId,int iParentId,CString strSection)
	: CCfgNode(iId,iParentId)
{
	m_strSection=strSection;
}

//// CCfgNodeItem

	IMPLEMENT_DYNAMIC(CCfgNodeItem,CCfgNode)
CCfgNodeItem::CCfgNodeItem(int iId,int iParentId,CString strItem,int iNumArgs)
	: CCfgNode(iId,iParentId)
{
	m_iNumArgs=iNumArgs;
	m_strItem=strItem;
}

//// CCfgNodeString

	IMPLEMENT_DYNAMIC(CCfgNodeString,CCfgNode)
CCfgNodeString::CCfgNodeString(int iId,int iParentId, CString strDef)
	: CCfgNode(iId,iParentId)
{
	m_strDef=m_strStr=strDef;
}

//// CCfgNodeBool

	IMPLEMENT_DYNAMIC(CCfgNodeBool,CCfgNode)
CCfgNodeBool::CCfgNodeBool(int iId,int iParentId,BOOL bDefault)
	: CCfgNode(iId,iParentId)
{
	m_bBool=m_bDefault=bDefault;
}

//// CCfgNodeNum

	IMPLEMENT_DYNAMIC(CCfgNodeNum,CCfgNode)
CCfgNodeNum::CCfgNodeNum(int iId,int iParentId,int iDefault,int iMax,int iMin)
	: CCfgNode(iId,iParentId)
{
	m_iMax=iMax;
	m_iMin=iMin;
	m_iVal=m_iDefault=GetLimitedVal(iDefault);
}

BOOL CCfgNodeNum::HasLimits()
{
	return (m_iMax!=0 && m_iMin!=0);
}

int CCfgNodeNum::GetLimitedVal(int iNum)
{
	if(HasLimits()){
		iNum=__max(m_iMin,iNum);
		iNum=__min(m_iMax,iNum);
	}
	return iNum;
}

//// CCfgNodeFloat

IMPLEMENT_DYNAMIC(CCfgNodeFloat,CCfgNode)

CCfgNodeFloat::CCfgNodeFloat(int iId,int iParentId,double fDefault/*=0.0*/)
	: CCfgNode(iId,iParentId)
{
	m_fVal=m_fDefault=fDefault;
}


//// CCfgNodeRGB

IMPLEMENT_DYNAMIC(CCfgNodeRGB,CCfgNode)

CCfgNodeRGB::CCfgNodeRGB(int iId,int iParentId,COLORREF DefaultClr/* = RGB(0,0,0)*/)
	: CCfgNode(iId,iParentId)
{
	m_Clr = m_DefaultClr = DefaultClr;
}
