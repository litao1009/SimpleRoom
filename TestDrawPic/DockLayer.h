#pragma once


// CDockLayer
#include "DlgLayer.h"

class CDockLayer : public CDockablePane
{
	DECLARE_DYNAMIC(CDockLayer)

public:
	CDockLayer();
	virtual ~CDockLayer();

public:
	
	void Refresh(CView* pActiveView, CBaseODLSPtr spObj){m_dlgLayer.Refresh(pActiveView, spObj);};
private:
	CDlgLayer m_dlgLayer;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


