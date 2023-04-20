#ifndef __NEWDATA_H__
#define __NEWDATA_H__



#ifdef _AFXDLL
#define BEGIN_MESSAGE_MAP_TEMPLATE_PASS(theClass,inherClass,baseClass,dataType) \
	template<class baseClass,class dataType> const AFX_MSGMAP* PASCAL theClass<baseClass,dataType>::GetThisMessageMap() \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> const AFX_MSGMAP* theClass<baseClass,dataType>::GetMessageMap() const \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP theClass<baseClass,dataType>::messageMap = \
	{ &inherClass::GetThisMessageMap, &theClass<baseClass,dataType>::_messageEntries[0] }; \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<baseClass,dataType>::_messageEntries[] = \
	{ \

#else
#define BEGIN_MESSAGE_MAP_TEMPLATE_PASS(theClass,inherClass,baseClass,dataType) \
	template<class baseClass,class dataType>  AFX_MSGMAP* theClass<baseClass,dataType>::GetMessageMap() const \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP theClass<baseClass,dataType>::messageMap = \
	{ &inherClass::messageMap, &theClass<baseClass,dataType>::_messageEntries[0] }; \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<baseClass,dataType>::_messageEntries[] = \
	{ \

#endif




#endif