#ifndef __BUILDERSESSION_H__
#define __BUILDERSESSION_H__

template<class BUILDER>
class BuilderSession
{
public:
	BuilderSession(BUILDER* pBuilder) : m_pBuilder(pBuilder)
	{
	}

	virtual void Commit() = 0;
	virtual void Begin() = 0;
	virtual void Rollback() = 0;
	
	BUILDER* GetBuilder()
	{
		return m_pBuilder;
	}
private:
	BUILDER* m_pBuilder;
};

#endif