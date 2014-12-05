#pragma once


// CDockSelect
#include "DlgSelectHTML.h"

class CDockSelect : public CDockablePane
{
	DECLARE_DYNAMIC(CDockSelect)

public:
	CDockSelect();
	virtual ~CDockSelect();
	void SetIsHTML(bool bIsHTML){m_bIsHTML=bIsHTML;};
	void RefreshWindow(){if (m_dlgSelectHTML.GetSafeHwnd()){m_dlgSelectHTML.RefreshWindow();}	};

private:
	CDlgSelectHTML m_dlgSelectHTML;
	bool m_bIsHTML;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


