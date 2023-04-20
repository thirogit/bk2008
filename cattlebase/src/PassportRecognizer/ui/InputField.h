#ifndef __INPUTFIELD_H__
#define __INPUTFIELD_H__

class CInputField 
{
public:
	CInputField(bool bEnabled,CWnd* pFieldWnd)  : m_bEnbaled(bEnabled),m_bInvalid(false),m_pFieldWnd(pFieldWnd)
	{
	}
	void SetInvalid(bool bInvalid)
	{
		m_bInvalid = bInvalid;
	}

	void SetEnabled(bool bEnabled)
	{
		m_bEnbaled = bEnabled;
	}

	bool IsInvalid()
	{
		return m_bInvalid;
	}

	bool IsFieldEnabledForInput()
	{		
		return m_bEnbaled;
	}
	CWnd* GetFieldWindow()
	{
		return m_pFieldWnd;
	}
private:
	bool m_bEnbaled;
	bool m_bInvalid;
	CWnd* m_pFieldWnd;
};

template<class CTRLCLASS>
class CInputFieldCtrl : public CInputField
{
public:
	CInputFieldCtrl(bool bEnbaled = true) : CInputField(bEnbaled,&m_Ctrl)
	{
	}
	CTRLCLASS& GetCtrl()
	{
		return m_Ctrl;
	}
	CTRLCLASS* operator->()
	{
		return &m_Ctrl;
	}
private:
	CTRLCLASS m_Ctrl;

};



#endif