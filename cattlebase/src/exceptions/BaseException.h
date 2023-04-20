#ifndef __BASEEXCEPTION_H__
#define __BASEEXCEPTION_H__

class CBaseException : public CException  
{
public:
	inline const CString& GetError() { return error; };
protected:
	CString		error;		 

};

#endif