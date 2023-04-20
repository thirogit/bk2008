#ifndef __RECOGNITIONEXCEPTION_H__
#define __RECOGNITIONEXCEPTION_H__

class RecognitionException : public CException  
{
	DECLARE_DYNAMIC(RecognitionException)
public:
	RecognitionException(const RecognitionException& src);
	RecognitionException(const CString& sError);
	const CString& GetError() const;
private:
	CString m_sError;
};

#endif