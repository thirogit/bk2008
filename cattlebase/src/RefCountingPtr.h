#ifndef __REFCOUNTINGPTR_H__
#define __REFCOUNTINGPTR_H__

template <class T> class RefCountingPtr 
{
private:
    
    template<class PTR>
    class __RefCounter 
	{
		friend class RefCountingPtr;
        private:
			PTR *__m_ptr;
			int __m_counter;
        protected:


			virtual PTR * GetPtr() const
			{
				return __m_ptr;
			}
			virtual void __DestroyRef() 
			{
				delete this;
			}

			virtual void __IncRefCount() 
			{
				__m_counter++; 
			}
			virtual void __DecRefCount()
			{
				__m_counter--;
				if(__m_counter == 0)
				{
					delete this;
				}
			}

        public:

			__RefCounter(PTR *ptr)
			{
				__m_ptr = ptr;
				__m_counter = 0;
			}
			virtual ~__RefCounter()
			{				
			}
    };

	__RefCounter<T> *__m_refcount;
    
    void __Assign(void *ptr)
    {
        if(ptr == NULL && __m_refcount != NULL)
            __Deassign();
        else
        {
			if(ptr != NULL)
				__Assign(new __RefCounter<T>(static_cast<T *>(ptr)));
        }
    }
    
    void __Assign(__RefCounter<T> *refcount)
    {
		if(refcount)
		{
			refcount->__IncRefCount();
		}
        __RefCounter<T> *oldref = __m_refcount;
        __m_refcount = refcount;
        if(oldref != NULL) oldref->__DecRefCount();
    }

	void __Deassign()
    {
		if(__m_refcount)
		{
			__m_refcount->__DecRefCount();
			__m_refcount = NULL;
		}
    }
public:
    RefCountingPtr()
    {
        __m_refcount = NULL;
    }
    RefCountingPtr(T * ptr)
    {
        __m_refcount = NULL;
        __Assign(ptr);
    }
    RefCountingPtr(const RefCountingPtr &sp)
    {
        __m_refcount = NULL;
        __Assign(sp.__m_refcount);
    }
    virtual ~RefCountingPtr()
    {
        __Deassign();
    }

	bool IsNull() const
	{
		return __m_refcount == NULL;
	}

    T *GetPtr() const
    {
        if(__m_refcount == NULL) return NULL;
        return __m_refcount->GetPtr();
    }

   
    RefCountingPtr & operator= (const RefCountingPtr &sp) 
	{
		__Assign(sp.__m_refcount); 
		return *this;
	}
    
    RefCountingPtr & operator= (T * ptr) 
	{
		__Assign(ptr); 
		return *this;
	}
    
    T * operator ->()
    {
#ifdef _ASSERT
        _ASSERT(GetPtr()!=NULL);
#endif
        return GetPtr();
    }
    

    operator T* () const
    {
        return GetPtr();
    }

    
    bool operator !()
    {
        return GetPtr()==NULL;
    }
    bool operator ==(const RefCountingPtr &sp)
    {
        return GetPtr()==sp.GetPtr();
    }
    bool operator !=(const RefCountingPtr &sp)
    {
        return GetPtr()!=sp.GetPtr();
    }

	int Count()
	{
		if(__m_refcount)
			return __m_refcount->__m_counter;

		return 0;
	}
};
#endif
