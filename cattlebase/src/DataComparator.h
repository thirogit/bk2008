#ifndef __DATACOMPARATOR_H__
#define __DATACOMPARATOR_H__

#include "classes/types/DateTime.h"
#include "classes/types/Money.h"
#include "classes/types/NullInt.h"

typedef int COMPARE_RESULT;

#define COMPARE_EQUAL (0)
#define COMPARE_LESS (-1)
#define COMPARE_GREATHER (1)

template<class DATATYPE>
class DataComparator
{
public:
	virtual COMPARE_RESULT CompareBy(UINT dataMember,DATATYPE* DataLeft,DATATYPE* DataRight) = 0;

protected:

	template<typename U>
	COMPARE_RESULT CompareNullable(const U &left,const U &right)
	{

		if(!left.IsNull())
		{
			if(!right.IsNull())
			{
				if(left > right)
					return COMPARE_GREATHER;
				else
				{
					if(left < right)
						return COMPARE_LESS;
				}
				return COMPARE_EQUAL;
			}
			return COMPARE_GREATHER;
		}
		else
		{
			if(!right.IsNull())
					return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}


	COMPARE_RESULT CompareDateTime(const CDateTime &left,const CDateTime &right)
	{

		if(!left.IsNull())
		{
			if(!right.IsNull())
			{
				if(left > right)
					return COMPARE_GREATHER;
				else
				{
					if(left < right)
						return COMPARE_LESS;
				}
				return COMPARE_EQUAL;
			}
			return COMPARE_GREATHER;
		}
		else
		{
			if(!right.IsNull())
					return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}

	COMPARE_RESULT CompareMoney(const Money &left,const Money &right)
	{
		return CompareNullable<Money>(left,right);
	}
	COMPARE_RESULT CompareNullInt(const NullInt &left,const NullInt &right)
	{
		if(!left.IsNull())
		{
			if(!right.IsNull())
			{
				if(left > right)
					return COMPARE_GREATHER;
				else
				{
					if(left < right)
						return COMPARE_LESS;
				}
				return COMPARE_EQUAL;
			}
			return COMPARE_GREATHER;
		}
		else
		{
			if(!right.IsNull())
					return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}

	COMPARE_RESULT CompareInt(const int left,const int right)
	{
		
		if(left > right)
			return COMPARE_GREATHER;
		else
		{
			if(left < right)
				return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}

	template<typename TYPE>
	COMPARE_RESULT CompareType(const TYPE& left,const TYPE& right)
	{
		if(left > right)
			return COMPARE_GREATHER;
		else
		{
			if(left < right)
				return COMPARE_LESS;
		}
		return COMPARE_EQUAL;
	}

	COMPARE_RESULT CompareNullDouble(const NullDouble& left,const NullDouble& right)
	{
		return CompareType<NullDouble>(left,right);
	}



	COMPARE_RESULT PolishStringCompare(const CString &str1,const CString &str2)
	{
		
		const static int PolishCharOrder[256] =
		{
			0x00,		0x01,		0x02,		0x03,		0x04,		0x05,		0x06,		0x07,		0x08,		0x09,		0x0A,		0x0B,		0x0C,		0x0D,		0x0E,		0x0F,
			0x10,		0x11,		0x12,		0x13,		0x14,		0x15,		0x16,		0x17,		0x18,		0x19,		0x1A,		0x1B,		0x1C,		0x1D,		0x1E,		0x1F,
			0x20,		'!' ,		'\"',		'#',		'$',		'%',		'&',		'\'',		'(',		')',		'*',		'+',		',',		'-',		'.',		'/',
			'0' ,		'1' ,		'2',		'3',		'4',		'5',		'6',		'7',		'8',		'9',		':',		';',		'<',		'=',		'>',		'?',
			'@' ,		'A' ,		'B'+1,		'C'+1,		'D'+2,		'E'+2,		'F'+3,		'G'+3,		'H'+3,		'I'+3,		'J'+3,		'K'+3,		'L'+3,		'M'+4,		'N'+4,		'O'+5,
			'P'+6,		'Q'+6,		'R'+6,		'S'+6,		'T'+7,		'U'+7,		'V'+7,		'W'+7,		'X'+7,		'Y'+7,		'Z'+7,		'['+9,		'\\'+9,		']'+9,		'^'+9,		'_'+9,
			'`'+9,		'a'+9,		'b'+9+1,	'c'+9+1,	'd'+9+2,	'e'+9+2,	'f'+9+3,	'g'+9+3,	'h'+9+3,	'i'+9+3,	'j'+9+3,	'k'+9+3,	'l'+9+3,	'm'+9+4,	'n'+9+4,	'o'+9+5,
			'p'+9+6,	'q'+9+6,	'r'+9+6,	's'+9+6,	't'+9+7,	'u'+9+7,	'v'+9+7,	'w'+9+7,	'x'+9+7,	'y'+9+7,	'z'+9+7,	'{'+9+8,	'|'+9+9,	'}'+18,		'~'+18,		0x7F+18,
			0x80+18,	0x81+18,	0x82+18,	0x83+18,	0x84+18,	0x85+18,	0x86+18,	0x87+18,	0x88+18,	0x89+18,	0x8A+18,	0x8B+18,	'S'+7,		0x8D+18,	0x8E+18,	'Z'+8,
			0x90+18,	0x91+18,	0x92+18,	0x93+18,	0x94+18,	0x95+18,	0x96+18,	0x97+18,	0x98+18,	0x99+18,	0x9A+18,	0x9B+18,	's'+7,		0x9D+18,	0x9E+18,	'z'+8,
			0xA0+18,	0xA1+18,	0xA2+18,	'L'+4,		0xA4+18,	'A'+1,		0xA6+18,	0xA7+18,	0xA8+18,	0xA9+18,	0xAA+18,	0xAB+18,	0xAC+18,	0xAD+18,	0xAE+18,	'Z'+7,
			0xB0+18,	0xB1+18,	0xB2+18,	'l'+4,		0xB4+18,	0xB5+18,	0xB6+18,	0xB7+18,	0xB8+18,	'a'+1,		0xBA+18,	0xBB+18,	0xBC+18,	0xBD+18,	0xBE+18,	'z'+7,
			0xC0+18,	0xC1+18,	0xC2+18,	0xC3+18,	0xC4+18,	0xC5+18,	'C'+2,		0xC7+18,	0xC8+18,	0xC9+18,	'E'+3,		0xCB+18,	0xCC+18,	0xCD+18,	0xCE+18,	0xCF+18,
			0xD0+18,	'N'+5,		0xD2+18,	'O'+6,		0xD4+18,	0xD5+18,	0xD6+18,	0xD7+18,	0xD8+18,	0xD9+18,	0xDA+18,	0xDB+18,	0xDC+18,	0xDD+18,	0xDE+18,	0xDF+18,
			0xE0+18,	0xE1+18,	0xE2+18,	0xE3+18,	0xE4+18,	0xE5+18,	'c'+2,		0xE7+18,	0xE8+18,	0xE9+18,	'e'+3,		0xEB+18,	0xEC+18,	0xED+18,	0xEE+18,	0xEF+18,
			0xF0+18,	'n'+5,		0xF2+18,	'o'+6,		0xF4+18,	0xF5+18,	0xF6+18,	0xF7+18,	0xF8+18,	0xF9+18,	0xFA+18,	0xFB+18,	0xFC+18,	0xFD+18,	0xFE+18,	0xFF+18
		};
		const unsigned char *s1 = (const unsigned char *)(LPCSTR)str1,
					*s2 = (const unsigned char *)(LPCSTR)str2;
		unsigned char Order1 = PolishCharOrder[*s1],Order2 = PolishCharOrder[*s2];	
		unsigned int i1,i2;
		COMPARE_RESULT result;
		int OrderResult;


		while (Order1 == Order2)
		{
			if (Order1 == 0) return (0);

			i1 = *(++s1);
			Order1 = PolishCharOrder[i1];
			i2 = *(++s2); 
			Order2 = PolishCharOrder[i2];
		}
	   

		OrderResult = Order1 - Order2;
		if(OrderResult < 0) result = COMPARE_LESS;
		else
		{
			if(OrderResult >  0) 
				result = COMPARE_GREATHER;
			else
				result = COMPARE_EQUAL;
		}



		return result;
	}

	COMPARE_RESULT CompareBool(bool left,bool right)
	{	
		if(left > right)
					return COMPARE_GREATHER;
		else
		{
			if(left == right)
					return COMPARE_EQUAL;
		}
		
		return COMPARE_LESS;
	}
	COMPARE_RESULT CompareAnsiString(const CString& left,const CString& right)
	{
		return left.Compare(right);
	}
};


#endif