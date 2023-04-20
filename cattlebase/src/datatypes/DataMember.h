#ifndef __DATAMEMBER_H__
#define __DATAMEMBER_H__

#include "TransactedDataMember.h"

template<class MEMBERTYPE,class BASICTYPE>
class DataMember : public TransactedDataMember
{
public:
	DataMember(BASICTYPE initValue) : m_bChanged(false),m_Value(initValue)
	{
	}
	DataMember() : m_bChanged(false)
	{
	}

	virtual void CopyFrom(const DataMember<MEMBERTYPE,BASICTYPE>& copySource)
	{
		m_Value = copySource.m_Value;
		m_bChanged = copySource.m_bChanged;
		if(m_bChanged)
			m_previousValue = copySource.m_previousValue;
	}

	MEMBERTYPE GetValue() const
	{
		return m_Value;
	}

	void SetTo(MEMBERTYPE Value)
	{
		m_Value = Value;
		m_previousValue = Value;
		m_bChanged = false;
	}

	virtual void ChangeTo(MEMBERTYPE NewValue)
	{
		if(NewValue != m_Value)
		{
			if(!m_bChanged)
			{
				m_previousValue = m_Value;
				m_bChanged = true;
				m_Value = NewValue;
			}
			else
			{
				if(m_previousValue == NewValue)
				{
					m_Value = m_previousValue;
					m_bChanged = false;
				}
				else
				{
					m_Value = NewValue;
				}
			}
		}
	}
	bool IsUncommited()
	{
		return IsChanged();
	}
	bool IsChanged()
	{
		return m_bChanged;
	}

	virtual void Rollback()
	{
		if(m_bChanged)
		{
			m_Value = m_previousValue;
			m_bChanged = false;
		}
	}
	virtual void Commit()
	{
		m_bChanged = false;
	}
protected:
	MEMBERTYPE	m_Value;
	MEMBERTYPE	m_previousValue;
	bool		m_bChanged;
};


#endif