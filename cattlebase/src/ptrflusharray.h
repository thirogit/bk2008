#ifndef __PTRFLUSHARRAY_H__
#define __PTRFLUSHARRAY_H__

template<class ARG>
class PtrFlushArray : public CTypedPtrArray<CPtrArray,ARG*>
{
public:

	class AutoArray
	{
	public:
		AutoArray(PtrFlushArray<ARG> &arrayToFlush) : m_array(arrayToFlush),bAutoDelete(true)
		{
		}
		~AutoArray()
		{
			if(bAutoDelete) m_array.FreeAndFlush();
		}
		void Release(bool bDelete = false)
		{
			bAutoDelete = bDelete;
		}
	private:
		PtrFlushArray<ARG> &m_array;

		bool bAutoDelete;
	};

	class ArrayAction
	{
	public:
		virtual bool operator()(ARG* pElement)
		{	
			return false;
		}
		int GetCurrentIndex()
		{
			return m_currIndex;
		}
	
		friend class PtrFlushArray;
	
	private:
		INT_PTR m_currIndex;		
	};


	class ArrayIterator
	{
	protected:
		ArrayIterator(PtrFlushArray<ARG> &array): m_array(&array),m_index(0)
		{      
		}

		ArrayIterator() : m_array(NULL),m_index(0)
		{      
		}


		PtrFlushArray<ARG> *m_array;
    	INT_PTR m_index;

		friend class PtrFlushArray;
	public:
  
    const INT_PTR index() const
    {
      return m_index;
    }

    inline ARG* operator*()
    {
		return m_array->ElementAt(m_index);
    }

   
    inline bool hasNext() const
    {
      return (m_index < m_array->GetSize());
    }

	inline bool isFirst() const
	{
		return m_index == 0;
	}

	void remove()
	{
		m_array->RemoveAt(m_index);
	}
   
    virtual ArrayIterator& operator=(const ArrayIterator& it)
    {
		m_array = it.m_array;
		m_index = it.m_index;
        return *this;
    }

   

    inline ArrayIterator operator++(int)
    {
      ArrayIterator it = *this;
      ++(*this);
      return it;
    }

    inline ArrayIterator operator++()
    {
		if(hasNext())
			m_index++;
      return *this;
    }

	inline ArrayIterator operator--()
    {
		if(!isFirst())
			m_index--;
      return *this;
    }

	inline ArrayIterator operator--(int)
    {
      ArrayIterator it = *this;
      --(*this);
      return it;
    }

	void reset()
	{
		m_index = 0;
	}

	static ArrayIterator nullIterator()
	{
		return ArrayIterator();
	}
    

    ArrayIterator(const ArrayIterator& it)
		: m_index(it.m_index),m_array(it.m_array)
    {
      
    }

    virtual ~ArrayIterator()
    {
      
    }
  };


	ArrayIterator iterator()
	{
		return ArrayIterator(*this);
	}

	virtual void FreeAndFlush()
	{
		for(INT_PTR i = 0,s = GetSize();i<s;i++)
			delete GetAt(i);
		RemoveAll();
	};

	virtual void ForAllAction(ArrayAction &action)
	{
		INT_PTR i,s;
		for(i = 0,s = GetSize();i<s;i++)
		{
			action.m_currIndex = i;
			if(!action(GetAt(i))) break;
		}
	}

	bool RemovePtr(const ARG *Ptr)
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

	int RemoveArray(PtrFlushArray<ARG> &array,bool bDelete = false)
	{
		int ii,ss,id,sd,removed = 0;
		ARG *aryPtr;

		PtrFlushArray<ARG> copy;
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