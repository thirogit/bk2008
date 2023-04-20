#ifndef __NEWPTRFLUSHARRAY_H__
#define __NEWPTRFLUSHARRAY_H__

#include <vector>

template<class ARG>
class NewPtrFlushArray : public std::vector<ARG*>
{
private:
	bool m_bDeleteOnDestroy;

	void DeleteAll()
	{
		for(INT_PTR i = 0,s = GetSize();i<s;i++)
			delete GetAt(i);
	}

public:

	NewPtrFlushArray(bool bDeleteOnDestroy = false) : m_bDeleteOnDestroy(bDeleteOnDestroy)
	{
	}

	~NewPtrFlushArray()
	{
		if(m_bDeleteOnDestroy)
		{
			DeleteAll();
		}
	}
	
	virtual void FreeAndFlush()
	{
		DeleteAll();
		RemoveAll();
	};

	int GetSize() const
	{
		return size();
	}

	ARG* GetAt(int iElementIndex)
	{
		return at(iElementIndex);
	}

	void RemoveAll()
	{
		clear();
	}

	void RemoveAt(int iIndex)
	{
		if(iIndex >= 0 && iIndex < size())
			erase(begin() + iIndex);
	}

	void Add(ARG* pElement)
	{
		push_back(pElement);
	}

	bool RemovePtr(ARG *Ptr)
	{
		int i,s;
		for(i = 0,s = GetSize();i<s;i++)
		{
			if(Ptr == GetAt(i))
			{
				RemoveAt(i);
				return true;
			}
		}
		return false;
	}

	void Append(NewPtrFlushArray<ARG>& src)
	{
		NewPtrFlushArray<ARG>::iterator it = src.begin();
		while(it != src.end())
			push_back(*(it++));
	}

	int RemoveArray(NewPtrFlushArray<ARG> &array,bool bDelete = false)
	{
		int ii,ss,id,sd,removed = 0;
		ARG *aryPtr;

		NewPtrFlushArray<ARG> copy;
		copy.Append(array);

		for(ii=0,ss = GetSize(),sd = copy.GetSize();ii<ss;ii++)
		{
			if(sd <= 0) break;
			aryPtr = GetAt(ii);			
			for(id=0;id < sd;id++)
			{
				if(copy[id] == aryPtr)
				{
					copy.RemoveAt(id);
					RemoveAt(ii);
					removed++;
									
					if(bDelete) delete aryPtr;
					
					ii--;
					ss--;
					sd--;
					id--;
					break;
				}
			}
		}
		return removed;
	}

};

#endif