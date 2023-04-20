#ifndef __PROPERTYSET_H__
#define __PROPERTYSET_H__

typedef struct tagSECTIONHEADER
{
	DWORD       cbSection ;
	DWORD       cProperties ;  // Number of props.
} SECTIONHEADER, *LPSECTIONHEADER ;

typedef struct tagPROPERTYIDOFFSET
{
	DWORD       propertyID;
	DWORD       dwOffset;
} PROPERTYIDOFFSET, *LPPROPERTYIDOFFSET;

typedef struct tagPROPHEADER
{
	WORD        wByteOrder ;    // Always 0xFFFE
	WORD        wFormat ;       // Always 0
	DWORD       dwOSVer ;       // System version
	CLSID       clsID ;         // Application CLSID
	DWORD       cSections ;     // Number of sections (must be at least 1)
} PROPHEADER, *LPPROPHEADER ;

typedef struct tagFORMATIDOFFSET
{
	GUID        formatID;
	DWORD       dwOffset;
} FORMATIDOFFSET, *LPFORMATIDOFFSET;


class CDocProperty : public CObject
{
	friend class CDocPropertySet ;
	friend class CDocPropertySection ;

public:
	CDocProperty( void ) ;
	CDocProperty( DWORD dwID, const LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( DWORD dwID, const LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( const LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( const LPVOID pValue ) ;
	LPVOID  Get( DWORD* pcb ) ;     // Returns pointer to actual value
	LPVOID  Get( void ) ;           // Returns pointer to actual value
	DWORD   GetType( void ) ;       // Returns property type
	void    SetType( DWORD dwType ) ;
	DWORD   GetID( void ) ;
	void    SetID( DWORD dwPropID ) ;

	LPVOID  GetRawValue( void ) ;   // Returns pointer internal value (may
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream ) ;

private:
	DWORD       m_dwPropID ;
	DWORD       m_dwType ;
	LPVOID      m_pValue ;

	LPVOID  AllocValue(ULONG cb);
	void    FreeValue();

public:
	~CDocProperty() ;
} ;


class CDocPropertySection : public CObject
{
	friend class CDocPropertySet ;
	friend class CDocProperty ;

public:
// Construction
	CDocPropertySection( void ) ;
	CDocPropertySection( CLSID FormatID ) ;

// Attributes
	CLSID   GetFormatID( void ) ;
	void    SetFormatID( CLSID FormatID ) ;

	BOOL    Set( DWORD dwPropID, LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( DWORD dwPropID, LPVOID pValue ) ;
	LPVOID  Get( DWORD dwPropID, DWORD* pcb ) ;
	LPVOID  Get( DWORD dwPropID ) ;
	void    Remove( DWORD dwPropID ) ;
	void    RemoveAll() ;

	CDocProperty* GetProperty( DWORD dwPropID ) ;
	void AddProperty( CDocProperty* pProp ) ;

	DWORD   GetSize( void ) ;
	DWORD   GetCount( void ) ;
	CObList* GetList( void ) ;

	BOOL    GetID( LPCTSTR pszName, DWORD* pdwPropID ) ;
	BOOL    SetName( DWORD dwPropID, LPCTSTR pszName ) ;

	BOOL    SetSectionName( LPCTSTR pszName );
	LPCTSTR GetSectionName( void );

// Operations
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream, LARGE_INTEGER liPropSet ) ;
	BOOL    WriteNameDictToStream( IStream* pIStream ) ;
	BOOL    ReadNameDictFromStream( IStream* pIStream ) ;

private:
// Implementation
	CLSID           m_FormatID ;
	SECTIONHEADER   m_SH ;
	// List of properties (CDocProperty)
	CObList         m_PropList ;
	// Dictionary of property names
	CMapStringToPtr m_NameDict ;
	CString         m_strSectionName;

public:
	~CDocPropertySection();
} ;


class CDocPropertySet : public CObject
{
	friend class CDocPropertySection ;
	friend class CDocProperty ;

public:
// Construction
	CDocPropertySet( void ) ;
	CDocPropertySet( CLSID clsID )  ;

// Attributes
	BOOL    Set( CLSID FormatID, DWORD dwPropID, LPVOID pValue, DWORD dwType ) ;
	BOOL    Set( CLSID FormatID, DWORD dwPropID, LPVOID pValue ) ;
	LPVOID  Get( CLSID FormatID, DWORD dwPropID, DWORD* pcb ) ;
	LPVOID  Get( CLSID FormatID, DWORD dwPropID ) ;
	void    Remove( CLSID FormatID, DWORD dwPropID ) ;
	void    Remove( CLSID FormatID ) ;
	void    RemoveAll( ) ;

	CDocProperty* GetProperty( CLSID FormatID, DWORD dwPropID ) ;
	void AddProperty( CLSID FormatID, CDocProperty* pProp ) ;
	CDocPropertySection* GetSection( CLSID FormatID ) ;
	CDocPropertySection* AddSection( CLSID FormatID ) ;
	void AddSection( CDocPropertySection* psect ) ;

	WORD    GetByteOrder( void ) ;
	WORD    GetFormatVersion( void ) ;
	void    SetFormatVersion( WORD wFmtVersion ) ;
	DWORD   GetOSVersion( void ) ;
	void    SetOSVersion( DWORD dwOSVer ) ;
	CLSID   GetClassID( void ) ;
	void    SetClassID( CLSID clsid ) ;
	DWORD   GetCount( void ) ;
	CObList* GetList( void ) ;

// Operations
	BOOL    WriteToStream( IStream* pIStream ) ;
	BOOL    ReadFromStream( IStream* pIStream ) ;

// Implementation
private:
	PROPHEADER      m_PH ;
	CObList         m_SectionList ;

public:
	~CDocPropertySet();
} ;

#endif 

