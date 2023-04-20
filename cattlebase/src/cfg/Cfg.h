// Cfg.h: interface for the CCfg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFG_H__CF467925_5C23_11D4_8C08_C90BC5C5997A__INCLUDED_)
#define AFX_CFG_H__CF467925_5C23_11D4_8C08_C90BC5C5997A__INCLUDED_

#include "txtvar.h"

// Error codes returned by CFG functions. Same as for base class, no new errors added.

#define CFG_NOERR			TXTVAR_NOERR
#define	CFG_NOFILE			TXTVAR_NOFILE
#define CFG_NOTFOUND		TXTVAR_NOTFOUND
#define	CFG_NOVALIDSECTION	TXTVAR_NOVALIDSECTION
#define	CFG_INDEXOUTOFRANGE	TXTVAR_INDEXOUTOFRANGE
#define	CFG_SYNTAX			TXTVAR_SYNTAX
#define	CFG_OUTOFRANGE		TXTVAR_OUTOFRANGE
#define	CFG_NOVALIDITEM		TXTVAR_NOVALIDITEM
#define	CFG_LASTERR_		TXTVAR_LATERR_

// CFG node types.

#define CFGT_SECTION		0
#define CFGT_ITEM			1
#define	CFGT_ARGSTRING		2
#define	CFGT_ARGNUM			3
#define	CFGT_ARGBOOL		4
#define CFGT_ARGFLOAT       5
#define CFGT_ARGRGB         6
#define	CFGT_END			-1

/*
	Macros for the declaration of the symbols used in the CFGDEF structure array.
	These are inserted in an enumeration table as the macros are expanded in the 
	CCfg derived class's header file as part of the CFGDEF definition.
*/

#define BEGIN_CFGDEF(name)				typedef enum name##_enum{
#define END_CFGDEF						};
#define	CFG_SECTION(id,name)			id,
#define	CFG_ITEM(id,numargs,name)		id,
#define	CFG_NUMLIMIT(id,min,max,def)	id,
#define	CFG_NUM(id,def)					id,
#define	CFG_STRING(id,name)				id,
#define	CFG_BOOL(id,def)				id,
#define CFG_FLOAT(id,def)               id,
#define CFG_RGB(id,r,g,b)               id,

#define	CFG_DLGPOS(id,name,x_pos,y_pos,visible)\
										id,\
										id##_XPOS,\
										id##_YPOS,\
										id##_VISIBLE,

#define	CFG_WINDOWPOS(id,name,x_pos,y_pos,width,height,style)\
										id,\
										id##_XPOS,\
										id##_YPOS,\
										id##_WIDTH,\
										id##_HEIGHT,\
										id##_STYLE,

#define	CFG_FONT(id,name,fontname,height,weight,italic)\
										id,\
										id##_NAME,\
										id##_HEIGHT,\
										id##_WEIGHT,\
										id##_ITALIC,
typedef struct { //tag_CFGDEF {	
	// Used to define and identify entries in a configfile.
	// This structure covers all types of entries, not all are used
	// for every type (String argument, for instance, only 
	// uses the iType field. CFGDEF is used as follows:
	// CFGDEF cfgdef[]={
	//  {CFGT_SECTION,CFGID_SETTINGS,,,,"Settings"},
	//  {CFGT_ITEM,CFGID_FONT,4,,,"Font"},
	//  {CFGT_ARGSTRING,CFGID_FONTNAME,,,,"Arial"},
	//  {CFGT_ARGNUM,CFGID_FONTHEIGHT,40,8,20},
	//  {CFGT_ARGNUM,CFGID_FONTWEIGHT,0,1000,FW_NORMAL},
	//	{CFGT_ARGBOOL,CFGID_FONTITALIC,,,FALSE},
	//	{CFGT_END}
	// }
	int			iType;		// Type as CFGT_... above
	int			iId;		// Identifier.
	long		iVal1;		// MinVal for CFGT_ARGNUM. Number of arguments for CFGT_ITEM.
	long		iVal2;		// MaxVal for CFGT_ARGNUM
	long		iVal3;		// Default value for CFGT_ARGNUM and CFGT_ARGBOOL	
	LPCSTR		strStr;		// Section name or Item name for CFGT_SECTION and
							// CFGT_ITEM. Default string for CFGT_ARGSTRING.
	double      fVal;
}CFGDEF;

class CCfgNode : public CObject
{
	DECLARE_DYNAMIC(CCfgNode)
public:
	int		m_iId;
	int		m_iParentId;
	CCfgNode(int iId=0,int iParentId=0);
	int GetType();

	// The default implementation for the following access functions ASSERT(FALSE). 
	// They are only overloaded in the class types that stores the info for the
	// access function. If an ASSERT fails here it is most likely because an access
	// has tried to be done on a member that doesn't belong to this class. For
	// example trying to do a GetStrArg on a node that isn't of type CCfgNodeString.

	virtual CString GetSection(){ASSERT(FALSE);return CString("");}
	virtual CString GetItem(){ASSERT(FALSE);return CString("");}
	virtual int GetNumArgs(){ASSERT(FALSE);return 0;}
	virtual CString GetStrArg(){ASSERT(FALSE);return CString("");}
	virtual void SetStrArg(CString str){ASSERT(FALSE);}
	virtual BOOL GetBoolArg(){ASSERT(FALSE);return FALSE;}
	virtual void SetBoolArg(BOOL b){ASSERT(FALSE);}
	virtual int GetNumArg(){ASSERT(FALSE);return 0;}
	virtual void SetNumArg(int iNum){ASSERT(FALSE);}
	virtual double GetFloatArg(){ASSERT(FALSE);return 0.0;}
	virtual void SetFloatArg(double fNum){ASSERT(FALSE);}
	virtual COLORREF GetRGBArg(){ASSERT(FALSE);return RGB(0,0,0);}
	virtual void SetRGBArg(COLORREF clr){ASSERT(FALSE);}

};

class CCfgNodeSection : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeSection)
public:
	CString m_strSection;
	CCfgNodeSection(int iId=0,int iParentId=0,CString strSection=CString(""));
	virtual CString GetSection(){return m_strSection;}
};

class CCfgNodeItem : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeItem)
public:
	CString m_strItem;
	int m_iNumArgs;
	CCfgNodeItem(int iId=0,int iParentId=0,CString strItem=CString(""),int iNumArgs=0);
	virtual CString GetItem(){return m_strItem;}
	virtual int GetNumArgs(){return m_iNumArgs;}
};

class CCfgNodeString : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeString)
public:
	CString m_strStr;
	CString m_strDef;
	CCfgNodeString(int iId=0,int iParentId=0,CString strDef=CString(""));
	virtual CString GetStrArg(){return m_strStr;}
	virtual void SetStrArg(CString str){m_strStr=str;}
};

class CCfgNodeBool	: public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeBool)
public:
	BOOL	m_bBool;
	BOOL	m_bDefault;
	CCfgNodeBool(int iId=0,int iParentId=0,BOOL bDefault=FALSE);
	virtual BOOL GetBoolArg(){return m_bBool;}
	virtual void SetBoolArg(BOOL b){m_bBool=b;}
};

class CCfgNodeNum : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeNum)
public:
	int	m_iVal;
	int m_iDefault;
	int m_iMax;
	int m_iMin;
	CCfgNodeNum(int iId=0,int iParentId=0,int iDefault=0,int iMax=0,int iMin=0);
	BOOL HasLimits();
	int GetLimitedVal(int iNum);
	virtual int GetNumArg(){return m_iVal;}
	virtual void SetNumArg(int iNum){m_iVal=GetLimitedVal(iNum);}
};

class CCfgNodeFloat : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeFloat)
public:
	double	m_fVal;
	double  m_fDefault;
	CCfgNodeFloat(int iId=0,int iParentId=0,double fDefault = 0.0);
	virtual double GetFloatArg(){return m_fVal;}
	virtual void SetFloatArg(double fNum){m_fVal=fNum;}
};


class CCfgNodeRGB : public CCfgNode
{
	DECLARE_DYNAMIC(CCfgNodeRGB)
public:
	COLORREF	m_Clr;
	COLORREF  m_DefaultClr;
	CCfgNodeRGB(int iId=0,int iParentId=0,COLORREF DefaultClr = RGB(0,0,0));
	virtual COLORREF GetRGBArg(){ return m_Clr;}
	virtual void SetRGBArg(COLORREF clr){ m_Clr = clr;}
};



typedef CTypedPtrList<CPtrList,CCfgNode *> CCfgList;

class CCfg : public CTxtVar
{
public:
	int Init(const CString strCfgFileName);
	void SetNum(int iItemId,int iArgIndex,int iNum);
	void SetNum(int iId,int iNum);
	void SetBool(int iItemId,int iArgIndex,BOOL b);
	void SetBool(int iId,BOOL b);
	void SetString(int iItemId,int iArgIndex,const CString &str);
	void SetString(int iId,const CString &str);
	long GetNum(int iItemId,int iArgIndex);
	long GetNum(int iId);
	BOOL GetBool(int iItemId,int iArgIndex);
	BOOL GetBool(int iId);
	CString GetString(int iItemId,int iArgIndex);
	CString GetString(int iId);

	double GetFloat(int iId);
	double GetFloat(int iItemId, int iArgIndex);

	void SetFloat(int iId, double fNum);
	void SetFloat(int iItemId, int iArgIndex, double fNum);


	COLORREF GetRGB(int iId);
	COLORREF GetRGB(int iItemId, int iArgIndex);

	void SetRGB(int iId, COLORREF Clr);
	void SetRGB(int iItemId, int iArgIndex, COLORREF Clr);


	CCfgNode* GetCfgNodeItem(const CString strItem,const CString strSection,POSITION *pos=NULL);
	CCfgNode* GetCfgNodeItemArg(CCfgNode *nItem,int iArgIndex,POSITION *pos=NULL);
	CCfgNode* GetCfgNodeItemArg(int iItemId,int iArgIndex,POSITION *pos=NULL);
	CCfgNode* GetCfgNodeFromId(int iId,POSITION *pos=NULL);
	int Flush();
	int NumArgs(){return(m_iNumArgs);}
	void Release();
	int LoadStringList(const CString strFileName);
	CCfg();
	virtual ~CCfg();
	unsigned int hex2int(LPCSTR s);
	void MakeCfgList(const CFGDEF *cfg_def);

protected:
	void DeleteCfgList();
	void LoadCfgList();
	void SaveCfgList();
	
	BOOL m_bCfgListInitiated;
	CCfgList m_cCfgList;
};

#elif defined __CFG_IMPLEMENTATION__

// CFGDEF macros that defines the CFGDEF structure array.
// All macros are first #undefed beacause at this point they are already defined
// to build the enum variable for the CFGDEF.


// BEGIN_CFGDEF declares the name of the CFGDED structure array.
#undef	BEGIN_CFGDEF
#define BEGIN_CFGDEF(name)				static CFGDEF (name)[]={

// END_CFGDEF closes the CFGDEF structure array.
#undef	END_CFGDEF
#define END_CFGDEF						{CFGT_END}};

// CFGT_SECTION declares a section name and it's ID.
#undef	CFG_SECTION
#define CFG_SECTION(id,name)			{CFGT_SECTION,id,0,0,0,name},

// CFGT_ITEM declares an item name, numer of arguments for the item and it's ID.
#undef	CFG_ITEM
#define CFG_ITEM(id,numargs,name)		{CFGT_ITEM,id,numargs,0,0,name},

// CFGT_NUMLIMIT declares a numeric limited argument and sets the id, min value, max value and
// a default valiue.
#undef	CFG_NUMLIMIT
#define	CFG_NUMLIMIT(id,min,max,def)	{CFGT_ARGNUM,id,min,max,def},

// CFGT_NUM declares a numeric unlimited argument and sets the id and default value,
#undef	CFG_NUM
#define	CFG_NUM(id,def)					{CFGT_ARGNUM,id,0,0,def},

// CFGT_STRING declares a string argument and sets the id and the default value.
#undef	CFG_STRING
#define CFG_STRING(id,name)				{CFGT_ARGSTRING,id,0,0,0,name},

// CFGT_BOOL declares a BOOL argument and sets the id and the default value.
#undef	CFG_BOOL
#define	CFG_BOOL(id,def)				{CFGT_ARGBOOL,id,0,0,def!=0},	


// CFG_DLGPOS is a combined macro that sets position and visibility for a modeless
// dialog box.
// The id of the Item is set to the base id. The other id's are combined with the base id
// appended with _XPOS, _YPOS and _VISIBLE.
#undef	CFG_DLGPOS
#define	CFG_DLGPOS(id,name,x_pos,y_pos,visible)	\
										{CFGT_ITEM,id,3,0,0,name},\
										{CFGT_ARGNUM,id##_XPOS,0,2000,x_pos},\
										{CFGT_ARGNUM,id##_YPOS,0,2000,y_pos},\
										{CFGT_ARGBOOL,id##_VISIBLE,0,0,visible},

// CFG_WINDOWPOS is a combined macro that sets position, width, height, style and base id 
// for a window.
// The id of the Item is set to the base id. The other id's are combined with the base id 
// appended with _XPOS, _YPOS, _WIDTH, _HEIGHT and _STYLE
#undef	CFG_WINDOWPOS
#define	CFG_WINDOWPOS(id,name,x_pos,y_pos,width,height,style)\
										{CFGT_ITEM,id,5,0,0,name},\
										{CFGT_ARGNUM,id##_XPOS,0,2000,x_pos},\
										{CFGT_ARGNUM,id##_YPOS,0,2000,y_pos},\
										{CFGT_ARGNUM,id##_WIDTH,0,10,width},\
										{CFGT_ARGNUM,id##_HEIGHT,0,10,height},\
										{CFGT_ARGNUM,id##_STYLE,0,0,style},


// CFG_FONT is a combined macro that sets name, height, weight, italic style and id for
// a font. 
// The id of the Item is set to the base id. The other id's are combined with the base id
// appended with _NAME, _HEIGHT, _WEIGHT and _ITALIC.
#undef	CFG_FONT
#define CFG_FONT(id,name,fontname,height,weight,italic)\
										{CFGT_ITEM,id,4,0,0,name},\
										{CFGT_ARGSTRING,id##_NAME,0,0,0,fontname},\
										{CFGT_ARGNUM,id##_HEIGHT,0,0,12},\
										{CFGT_ARGNUM,id##_WEIGHT,0,0,FW_NORMAL},\
										{CFGT_ARGBOOL,id##_ITALIC,0,0,italic!=0},

#undef CFG_FLOAT
#define CFG_FLOAT(id,def) {CFGT_ARGFLOAT,id,0,0,0,NULL,def},

#undef CFG_RGB
#define CFG_RGB(id,r,g,b) {CFGT_ARGRGB,id,RGB(r,g,b),0,0,NULL,0.0},

#endif // !defined(AFX_CFG_H__CF467925_5C23_11D4_8C08_C90BC5C5997A__INCLUDED_)
