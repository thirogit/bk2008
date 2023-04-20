#ifndef __COWSEX_H__
#define __COWSEX_H__

#define MAX_SEX_NAME 5

#define COWSEXES_SIZE 3

enum COWSEX
{
	SEX_XX = 1, //female
	SEX_UNK = 0, //unknown
	SEX_XY = -1 //male
};

typedef struct _tagCowSexes
{
	const char szCowSexName[MAX_SEX_NAME];
	COWSEX nSexCode;
} COWSEXES;

extern const COWSEXES CowSexes[COWSEXES_SIZE];

class CowSex
{
public:
	CowSex(COWSEX sex = SEX_UNK);
	void MakeXX();
	bool IsXX() const;
	void MakeXY();
	bool IsXY() const;
	void MakeUNK();
	bool IsUNK() const;
	void Make(COWSEX eSex);
	operator LPCTSTR() const;
	CowSex& operator=(const CowSex &right);
	CowSex& operator=(COWSEX rSex);
	CowSex& operator =(LPCSTR cowSexStr);
	COWSEX getCOWSEX() const;
	bool IsCowSex(COWSEX eSex) const;	


	bool operator>=(const CowSex& right);
	bool operator<=(const CowSex& right);
	bool operator>(const CowSex& right);
	bool operator<(const CowSex& right);
	bool operator==(const CowSex& right);


private:
	COWSEX m_sex;
	
};




#endif
