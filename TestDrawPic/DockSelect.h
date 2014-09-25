#pragma once


// CDockSelect
#include "DlgSelect.h"

class CDockSelect : public CDockablePane
{
	DECLARE_DYNAMIC(CDockSelect)

public:
	CDockSelect();
	virtual ~CDockSelect();

private:
	CDlgSelect m_dlgSelect;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


