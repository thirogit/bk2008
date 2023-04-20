#ifndef __RECOGNITIONOPERATIONRESULT_H__
#define __RECOGNITIONOPERATIONRESULT_H__



enum RecognitionOperationStatus
{
	RecognitionStatus_Success,
	RecognitionStatus_Failure
};


class RecognitionOperationResult
{
public:
	RecognitionOperationResult(RecognitionOperationStatus status);
	RecognitionOperationResult(RecognitionOperationStatus status,const CString& message);
	RecognitionOperationResult(const RecognitionOperationResult& src);

	RecognitionOperationResult& operator=(const RecognitionOperationResult& right);
	void CopyFrom(const RecognitionOperationResult& src);

	const CString& GetOperationMessage() const;
	RecognitionOperationStatus GetStatus() const;

private:
	RecognitionOperationStatus m_Status;
	CString m_Message;
};



#endif 
