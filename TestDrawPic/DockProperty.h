#pragma once


// CDockProperty
#include "afxpropertygridctrl.h"

class CDockProperty : public CDockablePane
{
	DECLARE_DYNAMIC(CDockProperty)

public:
	CDockProperty();
	virtual ~CDockProperty();
private:
	CFont m_fntPropList;
	std::shared_ptr<CMFCPropertyGridCtrl> m_propObj;
//	CDlgProperty m_dlgProperty;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


