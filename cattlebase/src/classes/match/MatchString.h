#ifndef _MATCHSTRING_H_
#define _MATCHSTRING_H_

template<class TYPE,class MATCHER>
class MatchString : public Match<TYPE>
{
public:
	MatchString(MATCHER *pMatcher,const CString& string2Match, CString (MATCHER::*getString)(TYPE *pData)) :
	  m_pMatcher(pMatcher),m_string2Match(string2Match),m_getString(getString) {};
	virtual bool DoMatch(TYPE *pData)
	{
		return wildcmpi((LPCSTR)m_string2Match,(LPCSTR)((m_pMatcher->*m_getString)(pData))) != 0;
	}
private:
	CString m_string2Match;
	CString (MATCHER::*m_getString)(TYPE *pData);
	MATCHER *m_pMatcher;
protected:
	int  wildcmpi(const char *wild, const char *string) 
	{
		// Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
		const char *cp = NULL, *mp = NULL;


		while ((*string) && (*wild != '*')) 
		{
			if ((toupper(*wild) != toupper(*string)) && (*wild != '?')) 
			{
				return 0;
			}
			wild++;
			string++;
		}

		while (*string) 
		{
			if (*wild == '*') 
			{
				if (!*++wild) 
				{
					return 1;
				}
				mp = wild;
				cp = string+1;
			} 
			else if ((toupper(*wild)== toupper(*string)) || (*wild == '?')) 
			{
				wild++;
				string++;
			} 
			else 
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == '*') 
		{
			wild++;
		}
		return !*wild;
	}

};

#endif


	
	





