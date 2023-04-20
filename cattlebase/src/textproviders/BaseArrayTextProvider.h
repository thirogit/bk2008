#ifndef __BASEARRAYTEXTPROVIDER_H__
#define __BASEARRAYTEXTPROVIDER_H__

template<class ARRAY,class BASETEXTPROVIDER>
class BaseArrayTextProvider
{
public:
	BaseArrayTextProvider(ARRAY &dataArray) : m_dataArray(dataArray)
	{
	}

	void GetText(int row,UINT dataID,LPSTR pszText) 
	{
		if(row < m_dataArray.GetSize())
		{
			CString dataText = 	m_textProvider.GetDataText(m_dataArray[row],dataID);
			strcpy(pszText,(LPCSTR)dataText);
		}
	}	
private:
	ARRAY &m_dataArray;
	BASETEXTPROVIDER m_textProvider;

};

#endif