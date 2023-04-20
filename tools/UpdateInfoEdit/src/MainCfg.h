#if !defined(_MAINCFG__INCLUDED_)
#define _MAINCFG__INCLUDED_
#define __CFG_FIRST_RUN__	
#endif

#if defined(__CFG_FIRST_RUN__)
#define	__INCLUDE_CFGDEF__
#elif defined(__CFG_IMPLEMENTATION__)
#define	__INCLUDE_CFGDEF__
#endif

#if defined(__INCLUDE_CFGDEF__)
#include <cfg\Cfg.h>

/*
	This file must be #included twice from the CCfg derived class, once to enumerate
	the ID's (definition) and once to implement the array of CFGDEF structures (declaration).
	The CFG_... macros are redefined when __CFG_IMPLEMENTATION__ is defined.

	When this file is included from the CCfg derived class and __CFG_IMPLEMENTATOIN__
	is defined the following macros generate the array of CFGDEF structures used to
	access the cfg file.

	When this file is included without __CFG_IMPLEMENTATION__ defined (the first time
	it is included in the CCfg derived class and whenever it is included in another 
	cpp file) all ID's are enumerated to generate unique numbers for all cfg ID's.
*/


BEGIN_CFGDEF(MAINCFG)

		CFG_SECTION(CFGID_LISTCOLS,"ListCols")
			CFG_ITEM(CFGID_VERFILELIST,2,"NewVerFileList")
				CFG_NUM(CFGID_VFLCOL1,0)
				CFG_NUM(CFGID_VFLCOL2,0)

		CFG_SECTION(CFGID_PROPERTIES,"ListCols")
			CFG_ITEM(CFGID_BASEDIRITEM,1,"BaseDir")
				CFG_STRING(CFGID_BASEDIR,"")
			CFG_ITEM(CFGID_ARCHIVESDIRITEM,1,"ArchivesDir")
				CFG_STRING(CFGID_ARCHIVESDIR,"")
			CFG_ITEM(CFGID_FTPSETTINGS,6,"FTP")
				CFG_STRING(CFGID_FTPHOST,"localhost")
				CFG_NUM(CFGID_FTPPORT,21)
				CFG_STRING(CFGID_FTPUSERNAME,"user")
				CFG_STRING(CFGID_FTPPASSWORD,"password")
				CFG_BOOL(CFGID_PASSIVEMODE,FALSE)
				CFG_STRING(CFGID_SERVERROOT,"/2010")
		
			
	
	END_CFGDEF

#endif


#if defined(__CFG_FIRST_RUN__)

class CMainCfg : public CCfg  
{
private:
	CMainCfg();
public:
	virtual ~CMainCfg();
	static CMainCfg m_mainCfg;
};


#define AppCfg() (CMainCfg::m_mainCfg)

#endif

#undef __INCLUDE_CFGDEF__
#undef __CFG_FIRST_RUN__
#undef __CFG_IMPLEMENTATION__