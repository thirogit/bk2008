#ifndef __CHECKOPTION_H__
#define __CHECKOPTION_H__


class CCheckOption 
{
public:
	CCheckOption(const CString& optionDesc,bool *bOptionValue);
	const CString& GetOptionDesc() const;
	bool GetOptionValue() const;
	void SetOptionDesc(const CString& sOptDesc);
	void SetOptionValue(bool bValue);
private:
	CString m_sOptionDesc;
	bool* m_bOptionValue;

};

#endif 
