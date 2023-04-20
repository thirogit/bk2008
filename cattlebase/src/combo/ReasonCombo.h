#ifndef __REASONCOMBO_H__
#define __REASONCOMBO_H__

#include "DataTypes.h"
#include "ExtendedComboBox.h"
#include "BaseCombo.h"

class CReasonCombo : public CExtendedComboBox
{
public:
	CReasonCombo(Reason::REASON_INOUT_TYPE = Reason::REASON_BOTH);
	void ReloadCombo(int initReason = 0);
	Reason *GetSelReason();

protected:
	virtual BOOL IsItemEnabled(UINT) const;
	Reason::REASON_INOUT_TYPE m_inout;

};

class CFindReasonCombo : public CBaseFindCombo<CReasonCombo>
{
public:
	virtual void InitFindCombo(Reason::REASON_INOUT_TYPE inout = Reason::REASON_BOTH);
protected:
	virtual BOOL IsItemEnabled(UINT) const;

};

/*
class CFindReasonCombo : public CReasonCombo
{
public:
	CFindReasonCombo();
	void InitFindReasonCombo(Reason::REASON_INOUT inout);
	bool IsNeutral();
protected:
	virtual BOOL IsItemEnabled(UINT) const;
private:
	int m_neurtalItem;
};*/

#endif