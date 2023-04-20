#ifndef __REASONDATA_H__
#define __REASONDATA_H__

#define NULL_REASONID (0)

class Reason 
{
public:
	Reason(UINT Id);

	enum REASON_INOUT_TYPE 
	{
		REASON_IN = 1,
		REASON_OUT = -1,
		REASON_BOTH = 0
	};

	UINT				GetId() const;

	CString				GetReasonCode() const;
	CString				GetReasonName() const;
	REASON_INOUT_TYPE	GetReasonType() const;

	void				SetReasonCode(const CString& sReasonCode);
	void				SetReasonName(const CString& sReasonName);
	void				SetReasonType(REASON_INOUT_TYPE reasonType);

private:
	UINT				reasonid;
	CString				reasoncode;
	CString				reasonname;
	REASON_INOUT_TYPE	inoutreason;

};


#endif