#ifndef __MYRUNTIMECLASS_H__
#define __MYRUNTIMECLASS_H__

#ifdef USE_MY_RUNTIME

#define MY_DECLARE_DYNCREATE(class_name) \
	MY_DECLARE_DYNAMIC(class_name) \
	static CObject* PASCAL CreateObject();

#define MY_DECLARE_DYNAMIC(class_name) \
protected: \
	static CRuntimeClass* PASCAL _GetBaseClass(); \
public: \
	static CRuntimeClass* GetRuntimeClassStatic(); \
	static CRuntimeClass* PASCAL GetThisClass(); \
	virtual CRuntimeClass* GetRuntimeClass() const; \

#define MY_IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	CObject* PASCAL class_name::CreateObject() \
		{ return new class_name; } \
	MY_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
		class_name::CreateObject, NULL)

#define MY_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, class_init) \
	CRuntimeClass* PASCAL class_name::_GetBaseClass() \
		{ return MY_RUNTIME_CLASS(base_class_name); } \
	 CRuntimeClass* class_name::GetRuntimeClassStatic() \
	{ \
		static CRuntimeClass class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			&class_name::_GetBaseClass, NULL, class_init }; \
		return &class##class_name; \
	} \
	CRuntimeClass* PASCAL class_name::GetThisClass() \
		{ return _MY_RUNTIME_CLASS(class_name); } \
	CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return _MY_RUNTIME_CLASS(class_name); } \


#define MY_RUNTIME_CLASS(class_name) (class_name::GetThisClass())

#define _MY_RUNTIME_CLASS(class_name) (class_name::GetRuntimeClassStatic())

#else


#define MY_DECLARE_DYNCREATE(class_name) DECLARE_DYNCREATE(class_name)


#define MY_DECLARE_DYNAMIC(class_name) DECLARE_DYNAMIC


#define MY_IMPLEMENT_DYNCREATE(class_name, base_class_name) IMPLEMENT_DYNCREATE(class_name, base_class_name)


#define MY_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, class_init) \
			IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, class_init)


#define MY_RUNTIME_CLASS(class_name) RUNTIME_CLASS(class_name) 

#define _MY_RUNTIME_CLASS(class_name) _RUNTIME_CLASS(class_name)

#endif

#endif
