#ifndef __UPDATEDATA_H__
#define __UPDATEDATA_H__

#define CHECKUPDATE_0LEN2NULL(fieldname,fieldVal,eqllist)	if(SQLUpdate.Check(update_##fieldname)) eqllist.Add(#fieldname,SQLString(fieldVal,true)) 
#define CHECKUPDATE(fieldname,fieldVal,eqllist)	if(SQLUpdate.Check(update_##fieldname)) eqllist.Add(#fieldname,SQLString(fieldVal)) 
#define CHECKUPDATE2_0LEN2NULL(field,eqllist) CHECKUPDATE_0LEN2NULL(field,field,eqllist)
#define CHECKUPDATE2(field,eqllist) CHECKUPDATE(field,field,eqllist)

template<int ArySize,class tEnum>
class UpdateData
{
public:

	UpdateData() 
	{
		Reset();
	};
	
	bool NeedUpdate()
	{
		for(int i=0; i< ArySize;i++)
			if(update_ary[i]) return true;
		return false;
	};

	void Reset()
	{
		memset(update_ary,0,sizeof(update_ary));		 
	};

	bool IsOnlyToUpdate(tEnum idx)
	{
		ASSERT((int)idx >= 0 && (int)idx < ArySize);

		bool onlyVal  = Check(idx);
		bool restVal = false;

		for(int i=0; i< ArySize;i++)
		{
			if(idx == i) continue;
			if(update_ary[i])
			{
				restVal = true;
				break;
			}
		}

		return (onlyVal && !restVal);	
	}

	bool IsOnlyToUpdate(tEnum idx1, tEnum idx2)
	{
		ASSERT((int)idx1 >= 0 && (int)idx1 < ArySize);
		ASSERT((int)idx2 >= 0 && (int)idx2 < ArySize);

		bool onlyVal1  = Check(idx1);
		bool onlyVal2  = Check(idx2);

		bool restVal = false;

		for(int i=0; i< ArySize;i++)
		{
			if(idx1 == i || idx2 == i) continue;
			if(update_ary[i])
			{
				restVal = true;
				break;
			}
		}

		return ((onlyVal1 || onlyVal2) && !restVal);	
	}

	bool Check(tEnum idx) const
	{
		ASSERT((int)idx >= 0 && (int)idx < ArySize);
		return update_ary[(int)idx];
	};

	bool Set(tEnum idx ,bool val = true)
	{
		ASSERT((int)idx >= 0 && (int)idx < ArySize);
		update_ary[(int)idx] = val;
		return val; //dont change
	};

	UpdateData& operator=(const UpdateData& rv)
	{
		memcpy(update_ary,rv.update_ary,ArySize*(sizeof(bool)));
		return *this;
	};

protected:
		bool update_ary[ArySize];
};





#endif